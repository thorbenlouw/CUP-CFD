/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#include "HDF5Record.h"

#include <stdexcept>

using namespace cupcfd::io::hdf5;

namespace cupcfd::io::hdf5 {
HDF5Record::HDF5Record(const std::string& groupName,
                       const std::string& dataName, const bool attr) {
  if (dataName == std::string("")) {
    throw(std::invalid_argument(
        "HDF5Record: HDF5Record: groupName must not be empty string"));
  }
  this->groupName = groupName;
  this->dataName = dataName;
  this->attr = attr;
}

HDF5Record::HDF5Record(const std::string& dataName, const bool attr) {
  this->dataName = dataName;
  this->attr = attr;
}

HDF5Record::~HDF5Record() = default;

cupcfd::error::eCodes HDF5Record::openGroup(HDF5Access& access) const {
  cupcfd::error::eCodes status;

  // Close if already open
  // ToDo - Should close other containers below this
  if (access.groupID >= 0) {
    status = closeGroup(access);
    CHECK_ECODE(status)
  }

  hid_t groupID = H5Gopen(access.fileID, this->groupName.c_str(), H5P_DEFAULT);
  if (groupID < 0) {
    throw(std::invalid_argument("HDF5Record: openGroup: Unable to Open Group " +
                                this->groupName));
  }
  access.groupID = groupID;
  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Record::closeGroup(const HDF5Access& access) const {
  if (access.groupID < 0) {
    throw(
        std::invalid_argument("HDF5Record: closeGroup: Invalid groupID - must "
                              "be greater than zero."));
  }
  auto err = H5Gclose(access.groupID);
  if (err < 0) {
    throw(std::invalid_argument(
        "HDF5Record: closeGroup: HDF5 unable to close group with ID " +
        std::to_string(access.groupID)));
  }
  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Record::openDataSet(HDF5Access& access) const {
  cupcfd::error::eCodes status;

  hid_t datasetID;
  hid_t pathID;

  if (this->attr == false) {
    // Close if already open
    // ToDo - Should close other containers below this
    if (access.datasetID >= 0) {
      status = closeDataSet(access);
      CHECK_ECODE(status)
    }

    if (access.groupID >= 0) {
      pathID = access.groupID;
    } else if (access.fileID >= 0) {
      pathID = access.fileID;
    } else {
      throw(std::invalid_argument(
          "HDF5Record: openDataSet: Do not have file or group ID"));
    }

    datasetID = H5Dopen(pathID, this->dataName.c_str(), H5P_DEFAULT);
    if (datasetID < 0) {
      throw(std::invalid_argument(
          "HDF5Record: openDataSet: Unable to Open Data Set " +
          this->dataName));
    } else {
      access.datasetID = datasetID;
    }
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Record::closeDataSet(const HDF5Access& access) const {
  hid_t err = 0;
  if (access.datasetID > 0) {
    err = H5Dclose(access.datasetID);
  }
  if (err < 0) {
    throw(std::invalid_argument("HDF5Record: closeDataSet: H5Dclose() failed"));
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Record::openDataSpace(HDF5Access& access) const {
  hid_t dataspaceID = -1;

  if (this->attr) {
    if (access.attrID <= 0) {
      throw(std::invalid_argument(
          "HDF5Record: openDataSpace: invalid access.attrID value"));
    }
    dataspaceID = H5Aget_space(access.attrID);
    if (dataspaceID < 0) {
      throw(std::invalid_argument(
          "HDF5Record: openDataSpace: H5Aget_space() failed"));
    }
  } else {
    // Opening a Dataspace for a Dataset
    if (access.datasetID <= 0) {
      throw(std::invalid_argument(
          "HDF5Record: openDataSpace: invalid access.datasetID value"));
    }
    dataspaceID = H5Dget_space(access.datasetID);
    if (dataspaceID < 0) {
      throw(std::invalid_argument(
          "HDF5Record: openDataSpace: H5Dget_space() failed"));
    }
  }
  access.dataspaceID = dataspaceID;
  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Record::closeDataSpace(const HDF5Access& access) {
  hid_t err = 0;

  if (access.dataspaceID > 0) {
    err = H5Sclose(access.dataspaceID);
  }

  if (err < 0) {
    throw(
        std::invalid_argument("HDF5Record: closeDataSpace: H5Sclose() failed"));
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Record::openAttribute(HDF5Access& access) const {
  hid_t attrID;
  hid_t pathID;

  if (this->attr == true) {
    if (access.groupID > 0) {
      pathID = access.groupID;
    } else if (access.fileID > 0) {
      pathID = access.fileID;
    } else {
      throw(std::invalid_argument(
          "HDF5Record: openAttribute: Do not have file or group ID"));
    }

    attrID = H5Aopen(pathID, this->dataName.c_str(), H5P_DEFAULT);

    if (attrID < 0) {
      throw(
          std::invalid_argument("HDF5Record: openAttribute: H5Aopen() failed"));
    }
    access.attrID = attrID;
  }

  return cupcfd::error::E_SUCCESS;
}

cupcfd::error::eCodes HDF5Record::closeAttribute(const HDF5Access& access) {
  hid_t err = 0;

  if (access.attrID > 0) {
    err = H5Aclose(access.attrID);
  }

  if (err < 0) {
    throw(
        std::invalid_argument("HDF5Record: closeAttribute: H5Aclose() failed"));
  }

  return cupcfd::error::E_SUCCESS;
}
}  // namespace cupcfd::io::hdf5
