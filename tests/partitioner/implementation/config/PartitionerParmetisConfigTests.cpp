/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the PartitionerParmetisConfig class.
 */

#define BOOST_TEST_MODULE PartitionerConfig

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <stdexcept>

#include "PartitionerParmetisConfig.h"
#include "Error.h"
#include "PartitionerInterface.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::partitioner;

// ======= Setup ========
// Some tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructor ===
// Test 1: Create using default constructor
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	PartitionerParmetisConfig<int,int> config;
}

// === buildPartitioner ===
// Test 1: Build a partitioner of a naive partitioner type
BOOST_AUTO_TEST_CASE(buildPartitioner_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	PartitionerParmetisConfig<int,int> config;
	PartitionerInterface<int,int> * part;

	status = config.buildPartitioner(&part);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	delete part;
}

// ======= Cleanup ========
// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
