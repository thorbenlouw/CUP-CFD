/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the LinearSolverConfig class
 */

#define BOOST_TEST_MODULE LinearSolverConfigPETSc
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <stdexcept>

#include "LinearSolverConfigPETSc.h"
#include "Error.h"

using namespace cupcfd::linearsolvers;

// === Constructors ===
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	LinearSolverConfigPETSc config;

	// Check Defaults
}
