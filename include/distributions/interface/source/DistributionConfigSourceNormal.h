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
 * Contains declarations for the DistributionConfigSourceNormal Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_INCLUDE_H

// Error Codes
#include "Error.h"

#include "DistributionConfigSource.h"

namespace cupcfd
{
	namespace distributions
	{
		/**
		 * General Interface for retrieving information from a Distribution config data source
		 * for DistributionConfigNormal objects.
		 *
		 * @tparam The datatype of any indexing scheme used by the distribution
		 * @tparam The datatype of the values generated by the distribution
		 */
		template <class I, class T>
		class DistributionConfigSourceNormal : public DistributionConfigSource<I,T>
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Default Constructor
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				DistributionConfigSourceNormal();

				/**
				 * Constructor:
				 * Copy values from source to this newly constructed object
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				DistributionConfigSourceNormal(const DistributionConfigSourceNormal<I,T>& source);

				/**
				 * Deconstructor:
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				virtual ~DistributionConfigSourceNormal();

				// === Concrete Methods ===
				virtual cupcfd::error::eCodes buildDistributionConfig(DistributionConfig<I,T> ** distConfig);

				// === Pure Virtual Methods ===
				virtual void operator=(const DistributionConfigSourceNormal<I,T>& source);
				virtual DistributionConfigSourceNormal<I,T> * clone() = 0;

				/**
				 *
				 */
				virtual cupcfd::error::eCodes getLBound(T * lBound) = 0;

				/**
				 *
				 */
				virtual cupcfd::error::eCodes getUBound(T * uBound) = 0;

				/**
				 *
				 */
				virtual cupcfd::error::eCodes getMean(T * mean) = 0;

				/**
				 *
				 */
				virtual cupcfd::error::eCodes getStDev(T * stDev) = 0;
		};
	}
}

// Include Header Level Definitions
#include "DistributionConfigSourceNormal.ipp"

#endif