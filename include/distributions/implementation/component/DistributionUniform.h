/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Contains declarations for the DistributionUniform Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_UNIFORM_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_UNIFORM_INCLUDE_H

// C++ Library
#include <random>

// Base Class
#include "Distribution.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace distributions
	{
		/**
		 * Provides an interface to generate uniformly distributed numbers.
		 *
		 * @tparam I The type of the index scheme.
		 * @tparam T The type of the values generated by the distribution.
		 */
		template <class I, class T>
		class DistributionUniform : public Distribution<I,T>
		{
			public:
				// === Members ===

				/** C++ Library Distribution used to generate numbers **/
				std::uniform_real_distribution<T> * dist;
				std::default_random_engine * rEngine;

				// === Constructors/Deconstructors

				/**
				 * Build a Uniform Distribution with a inclusive lower bound
				 * and a non-inclusive upper bound.
				 *
				 * @param lbound Inclusive lower bound of the range of generated numbers.
				 * @param ubound Non-inclusive upper bound of the range of generated numbers.
				 */
				DistributionUniform(T lbound, T ubound);

				/**
				 * Copy Constructor
				 *
				 */
				DistributionUniform(DistributionUniform<I,T>& source);

				/**
				 * Deconstructor
				 */
				virtual ~DistributionUniform();

				// === Concrete Methods ===

				// === Overloaded Inherited Methods ===

				cupcfd::error::eCodes getValues(T * values, I nValues);

				DistributionUniform<I,T> * clone();

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "DistributionUniform.ipp"

#endif