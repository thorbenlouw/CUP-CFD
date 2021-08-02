/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#ifndef CUPCFD_IO_HDF5_RECORD_INCLUDE_H
#define CUPCFD_IO_HDF5_RECORD_INCLUDE_H

#include <string>
#include <vector>

#include "Error.h"
#include "HDF5Access.h"
#include "hdf5.h"

namespace cupcfd::io::hdf5 {
class HDF5Access;

class HDF5Record {
 public:
  // Variables
  std::string groupName;  // Name of group containing the dataset
  std::string dataName;   // Name of the dataset
  bool attr;              // True = attribute, False = DataSet

  // Constructors/Deconstructors
  HDF5Record(const std::string& groupName, const std::string& dataName,
             bool attr);
  HDF5Record(const std::string& dataName, bool attr);
  ~HDF5Record();

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes openGroup(
      HDF5Access& access) const;

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes closeGroup(
      const HDF5Access& access) const;

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes openDataSet(
      HDF5Access& access) const;

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes closeDataSet(
      const HDF5Access& access) const;

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes openDataSpace(
      HDF5Access& access) const;

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes closeDataSpace(
      const HDF5Access& access);

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes openAttribute(
      HDF5Access& access) const;

  /**
   *
   */
  __attribute__((warn_unused_result)) cupcfd::error::eCodes closeAttribute(
      const HDF5Access& access);
};
}  // namespace cupcfd::io::hdf5

#endif
