/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the Statistics kernels of the utility operations
 */

#define BOOST_TEST_MODULE StatisticsKernels
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>
#include <stdexcept>

#include "StatisticsKernels.h"

using namespace cupcfd::utility::kernels;

// === sum ===

// Test 1: Test sum of arbitrary values, integer
BOOST_AUTO_TEST_CASE(sum_test1) {
  int data[10] = {5, 6, 1, 7, 2, 61, 51, 32, 13, 1041};
  int sumCmp = 1219;
  int result;

  result = sum(data, 10);

  BOOST_CHECK_EQUAL(result, sumCmp);
}

// Test 2: Test sum of arbitrary values, doubles
BOOST_AUTO_TEST_CASE(sum_test2) {
  double data[10] = {434.2,  213.1, 52.2,   14.1,   0.000434,
                     2315.2, 413.2, 754.52, 5267.2, 173543.2};
  double sumCmp = 183006.920434;
  double result;

  result = sum(data, 10);

  BOOST_CHECK_EQUAL(result, sumCmp);
}