/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definitions for HDF5Access class.
 */

#include "HDF5Access.h"

#include <stdexcept>

#include "HDF5Properties.h"
#include "HDF5Record.h"

namespace cupcfd::io::hdf5 {
/* From
 * https://support.hdfgroup.org/HDF5/doc/Advanced/UsingIdentifiers/index.html In
 * other words, a successful operation will return a non-negative identifier
 * which will
 *
 * never be 0 (zero) and will always be a positive value.
 */

HDF5Access::HDF5Access(const std::string& fileName, HDF5Record& record)
    : record(record) {
  this->fileID = -1;
  this->groupID = -1;
  this->datasetID = -1;
  this->dataspaceID = -1;
  this->memspaceID = -1;
  this->attrID = -1;

  this->fileName = fileName;

  cupcfd::error::eCodes status;

  status = this->openFile();
  HARD_CHECK_ECODE(status)

  // Open Group (If Applicable)
  status = record.openGroup(*this);
  HARD_CHECK_ECODE(status)

  // Open DataSet / Attribute (Combine method?)
  status = record.openDataSet(*this);
  HARD_CHECK_ECODE(status)
  status = record.openAttribute(*this);
  HARD_CHECK_ECODE(status)

  // Open DataSpace
  status = record.openDataSpace(*this);
  HARD_CHECK_ECODE(status)
}

HDF5Access::~HDF5Access() {
  cupcfd::error::eCodes status;

  status = record.closeDataSpace(*this);
  DBG_PRINT_BAD_ECODE(status)
  status = record.closeDataSet(*this);
  DBG_PRINT_BAD_ECODE(status)
  status = record.closeGroup(*this);
  DBG_PRINT_BAD_ECODE(status)
  status = this->closeFile();
  DBG_PRINT_BAD_ECODE(status)
}

cupcfd::error::eCodes HDF5Access::openFile() {
  cupcfd::error::eCodes status;
  hid_t tmpFileID;

  // If a file is already open in this access, close it.
  // ToDo - Should close other containers
  if (this->fileID > 0) {
    status = this->closeFile();
    CHECK_ECODE(status)
  }

  tmpFileID = H5Fopen(this->fileName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
  if (tmpFileID < 0) {
    throw(std::invalid_argument(
        "HDF5Interface: openFile: HDF5 unable to open file '" + this->fileName +
        "'"));
  }
  this->fileID = tmpFileID;
  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::closeFile() {
  hid_t err;

  if (this->fileID < 0) {
    throw(
        std::invalid_argument("HDF5Interface: closeFile: Invalid fileID - must "
                              "be greater than zero."));
  }

  err = H5Fclose(this->fileID);
  this->fileID = -1;

  if (err < 0) {
    throw(std::invalid_argument(
        "HDF5Interface: closeFile: HDF5 unable to close file '" +
        this->fileName + "'"));
  }

  return cupcfd::error::E_SUCCESS;
}

// Important ToDo: The handling of type checking for these vs the file storage
// type should be improved to prevent errors.

cupcfd::error::eCodes HDF5Access::readData(int* sink) const {
  hid_t err;

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    err = H5Dread(this->datasetID, H5T_NATIVE_INT, H5S_ALL, this->dataspaceID,
                  H5P_DEFAULT, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_INT, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(float* sink) const {
  hid_t err;

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    err = H5Dread(this->datasetID, H5T_NATIVE_FLOAT, H5S_ALL, this->dataspaceID,
                  H5P_DEFAULT, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_FLOAT, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(double* sink) const {
  hid_t err;

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    err = H5Dread(this->datasetID, H5T_NATIVE_DOUBLE, H5S_ALL,
                  this->dataspaceID, H5P_DEFAULT, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_DOUBLE, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(int* sink,
                                           HDF5Properties& properties) {
  hid_t err;

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    if (properties.nidx > 0) {
      // Indexed Read

      err = H5Sselect_elements(this->dataspaceID, H5S_SELECT_SET,
                               properties.nidx, &properties.idx[0]);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_elements() failed"));
      }

      // ToDo - Move this out and associate it with the properties objects?
      hid_t tmpMemspaceID = H5Screate_simple(1, &(properties.nidx), NULL);
      if (tmpMemspaceID < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Screate_simple() failed"));
      }

      err = H5Dread(this->datasetID, H5T_NATIVE_INT, tmpMemspaceID,
                    this->dataspaceID, H5P_DEFAULT, sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }

      err = H5Sclose(tmpMemspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sclose() failed"));
      }

      // Cleanup Indexing into Dataspace.
      err = H5Sselect_none(this->dataspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_none() failed"));
      }
    } else {
      // Full Read
      err = H5Dread(this->datasetID, H5T_NATIVE_INT, H5S_ALL, this->dataspaceID,
                    H5P_DEFAULT, sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_INT, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(float* sink,
                                           HDF5Properties& properties) {
  hid_t err;

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    if (properties.nidx > 0) {
      // Indexed Read

      err = H5Sselect_elements(this->dataspaceID, H5S_SELECT_SET,
                               properties.nidx, &properties.idx[0]);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_elements() failed"));
      }

      // ToDo - Move this out and associate it with the properties objects?
      hid_t tmpMemspaceID = H5Screate_simple(1, &(properties.nidx), NULL);
      if (tmpMemspaceID < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Screate_simple() failed"));
      }

      err = H5Dread(this->datasetID, H5T_NATIVE_FLOAT, tmpMemspaceID,
                    this->dataspaceID, H5P_DEFAULT, sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }

      err = H5Sclose(tmpMemspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sclose() failed"));
      }

      // Cleanup Indexing into Dataspace.
      err = H5Sselect_none(this->dataspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_none() failed"));
      }
    } else {
      // Full Read
      err = H5Dread(this->datasetID, H5T_NATIVE_FLOAT, H5S_ALL,
                    this->dataspaceID, H5P_DEFAULT, sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_FLOAT, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(double* sink,
                                           HDF5Properties& properties) {
  hid_t err;

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    if (properties.nidx > 0) {
      // Indexed Read

      err = H5Sselect_elements(this->dataspaceID, H5S_SELECT_SET,
                               properties.nidx, &properties.idx[0]);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_elements() failed"));
      }

      // ToDo - Move this out and associate it with the properties objects?
      hid_t tmpMemspaceID = H5Screate_simple(1, &(properties.nidx), NULL);
      if (tmpMemspaceID < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Screate_simple() failed"));
      }

      err = H5Dread(this->datasetID, H5T_NATIVE_DOUBLE, tmpMemspaceID,
                    this->dataspaceID, H5P_DEFAULT, sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }

      err = H5Sclose(tmpMemspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sclose() failed"));
      }

      // Cleanup Indexing into Dataspace.
      err = H5Sselect_none(this->dataspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_none() failed"));
      }
    } else {
      // Full Read
      err = H5Dread(this->datasetID, H5T_NATIVE_DOUBLE, H5S_ALL,
                    this->dataspaceID, H5P_DEFAULT, sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_DOUBLE, sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(int** sink,
                                           HDF5Properties& properties) {
  hid_t err;

  int cap = 1;
  for (int i = 0; i < properties.ndim; i++) {
    cap = cap * properties.dim[i];
  }

  *sink = (int*)malloc(sizeof(int) * cap);

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    if (properties.nidx > 0) {
      // Indexed Read

      err = H5Sselect_elements(this->dataspaceID, H5S_SELECT_SET,
                               properties.nidx, &properties.idx[0]);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_elements() failed"));
      }

      // ToDo - Move this out and associate it with the properties objects?
      hid_t tmpMemspaceID = H5Screate_simple(1, &(properties.nidx), NULL);
      if (tmpMemspaceID < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Screate_simple() failed"));
      }

      err = H5Dread(this->datasetID, H5T_NATIVE_INT, tmpMemspaceID,
                    this->dataspaceID, H5P_DEFAULT, *sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }

      err = H5Sclose(tmpMemspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sclose() failed"));
      }

      // Cleanup Indexing into Dataspace.
      err = H5Sselect_none(this->dataspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_none() failed"));
      }
    } else {
      // Full Read
      err = H5Dread(this->datasetID, H5T_NATIVE_INT, H5S_ALL, this->dataspaceID,
                    H5P_DEFAULT, *sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_INT, *sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(float** sink,
                                           HDF5Properties& properties) {
  hid_t err;

  int cap = 1;
  for (int i = 0; i < properties.ndim; i++) {
    cap = cap * properties.dim[i];
  }

  *sink = (float*)malloc(sizeof(int) * cap);

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    if (properties.nidx > 0) {
      // Indexed Read

      err = H5Sselect_elements(this->dataspaceID, H5S_SELECT_SET,
                               properties.nidx, &properties.idx[0]);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_elements() failed"));
      }

      // ToDo - Move this out and associate it with the properties objects?
      hid_t tmpMemspaceID = H5Screate_simple(1, &(properties.nidx), NULL);
      if (tmpMemspaceID < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Screate_simple() failed"));
      }

      err = H5Dread(this->datasetID, H5T_NATIVE_FLOAT, tmpMemspaceID,
                    this->dataspaceID, H5P_DEFAULT, *sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }

      err = H5Sclose(tmpMemspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sclose() failed"));
      }

      // Cleanup Indexing into Dataspace.
      err = H5Sselect_none(this->dataspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_none() failed"));
      }
    } else {
      // Full Read
      err = H5Dread(this->datasetID, H5T_NATIVE_FLOAT, H5S_ALL,
                    this->dataspaceID, H5P_DEFAULT, *sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_FLOAT, *sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Access::readData(double** sink,
                                           HDF5Properties& properties) {
  hid_t err;

  int cap = 1;
  for (int i = 0; i < properties.ndim; i++) {
    cap = cap * properties.dim[i];
  }

  *sink = (double*)malloc(sizeof(int) * cap);

  if (this->record.attr == false) {
    // Begin Data Read into arrays
    if (properties.nidx > 0) {
      // Indexed Read

      err = H5Sselect_elements(this->dataspaceID, H5S_SELECT_SET,
                               properties.nidx, &properties.idx[0]);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_elements() failed"));
      }

      // ToDo - Move this out and associate it with the properties objects?
      hid_t tmpMemspaceID = H5Screate_simple(1, &(properties.nidx), NULL);
      if (tmpMemspaceID < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Screate_simple() failed"));
      }

      err = H5Dread(this->datasetID, H5T_NATIVE_DOUBLE, tmpMemspaceID,
                    this->dataspaceID, H5P_DEFAULT, *sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }

      err = H5Sclose(tmpMemspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sclose() failed"));
      }

      // Cleanup Indexing into Dataspace.
      err = H5Sselect_none(this->dataspaceID);
      if (err < 0) {
        throw(std::invalid_argument(
            "HDF5Interface: readData: H5Sselect_none() failed"));
      }
    } else {
      // Full Read
      err = H5Dread(this->datasetID, H5T_NATIVE_DOUBLE, H5S_ALL,
                    this->dataspaceID, H5P_DEFAULT, *sink);
      if (err < 0) {
        throw(
            std::invalid_argument("HDF5Interface: readData: H5Dread() failed"));
      }
    }
  } else if (this->record.attr == true) {
    err = H5Aread(this->attrID, H5T_NATIVE_DOUBLE, *sink);
    if (err < 0) {
      throw(std::invalid_argument("HDF5Interface: readData: H5Aread() failed"));
    }
  }

  return cupcfd::error::E_SUCCESS;
}
}  // namespace cupcfd::io::hdf5
