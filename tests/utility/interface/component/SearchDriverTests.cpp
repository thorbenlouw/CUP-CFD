/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the drivers of the utility search operations
 */

#define BOOST_TEST_MODULE SearchDrivers
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>
#include <stdexcept>

#include "SearchDrivers.h"

// ============================================================
// ==================== Driver Tests ==========================
// ============================================================
using cupcfd::utility::drivers::binarySearch;
using cupcfd::utility::drivers::linearSearch;

template <typename T>
auto actualPosition(std::vector<T> src, int target) -> int {
  auto found = std::find(src.begin(), src.end(), target);
  if (found != std::end(src)) {
    return std::distance(src.begin(), found);
  } else {
    return -1;
  }
}

// ================== Binary Search ===========================
BOOST_AUTO_TEST_CASE(driver_binarySearch_index_first) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  auto target = 1;
  int index;

  auto result = binarySearch(source.data(), (int)source.size(), target, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
  BOOST_CHECK_EQUAL(index, actualPosition(source, target));
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_last) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  int index;
  auto target = 20;

  auto result = binarySearch(source.data(), (int)source.size(), target, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
  BOOST_CHECK_EQUAL(index, actualPosition(source, target));
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_mid) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  int index;

  auto target = 20;
  auto result = binarySearch(source.data(), (int)source.size(), target, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
  BOOST_CHECK_EQUAL(index, actualPosition(source, target));
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_error_unsorted) {
  auto source = std::vector<int>{{4, 1, 7, 9, 7, 11, 13, 20}};
  int index;
  auto target = 9;

  auto result = binarySearch(source.data(), (int)source.size(), target, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_UNSORTED);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_error_notfound) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  int index;
  auto target = 1024;

  auto result = binarySearch(source.data(), (int)source.size(), target, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_NOT_FOUND);
}

// ================== Linear Search ===========================

BOOST_AUTO_TEST_CASE(driver_linearSearch_index_first) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  int index;

  auto result = linearSearch(source.data(), (int)source.size(), 1, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_index_last) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  int index;

  auto result = linearSearch(source.data(), (int)source.size(), 20, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_index_mid) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  int index;

  auto result = linearSearch(source.data(), (int)source.size(), 9, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_index_error_notfound) {
  auto source = std::vector<int>{{1, 4, 7, 8, 9, 11, 13, 20}};
  int index;

  auto result = linearSearch(source.data(), (int)source.size(), 1024, &index);
  BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_NOT_FOUND);
}
