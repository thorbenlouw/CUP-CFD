/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the kernel declarations for search operations.
 * Note: these operations could likely be replaced with C++
 * search functionality.
 *
 */

#ifndef CUPCFD_UTILITY_SEARCH_KERNELS_INCLUDE_H
#define CUPCFD_UTILITY_SEARCH_KERNELS_INCLUDE_H

#include "Error.h"

namespace cupcfd::utility::kernels {
/**
 * Performs a linear search for an element within an array.
 *
 * @param source The source array to search through
 * @param nEle The size of source in the number of elements of type T
 * @param target The element to search for in the array
 *
 * @tparam I The type of the indexing scheme (integer based)
 * @tparam T The type of the stored array data
 *
 * @return An error status indicating the success or failure of the operation
 */
template <class I, class T>
__attribute__((warn_unused_result)) cupcfd::error::eCodes linearSearch(
    T* source, I nEle, T target, I* index);

/**
 * Performs a binary search for an element within an array.
 * It requires that the array is sorted from in ascending value
 * based on the < and == operators of type T.
 *
 * @param source The source array to search through
 * @param nEle The size of source in the number of elements of type T
 * @param target The element to search for in the array
 *
 * @tparam I The type of the indexing scheme (integer based)
 * @tparam T The type of the stored array data
 *
 * @return An error status indicating the success or failure of the operation
 */
template <class I, class T>
__attribute__((warn_unused_result)) cupcfd::error::eCodes binarySearch(
    T* source, I nEle, T target, I* index);

}  // namespace cupcfd

// Definitions for the declarations
#include "SearchKernels.ipp"

#endif
