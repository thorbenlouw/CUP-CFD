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
 * Contains declarations for the DistributionConfigSourceFixed Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_JSON_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_JSON_INCLUDE_H

// Error Codes
#include "Error.h"

#include "DistributionConfigSourceNormal.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace distributions
	{
		/**
		 * General Interface for retrieving information from a Distribution config data source
		 * for a DistributionConfigNormal.
		 *
		 * @tparam The datatype of any indexing scheme used by the distribution
		 * @tparam The datatype of the values generated by the distribution
		 */
		template <class I, class T>
		class DistributionConfigSourceNormalJSON : public DistributionConfigSourceNormal<I,T>
		{
			public:
				// === Members ===

				/**
				 * Json Storage of Data
				 */
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Default Constructor
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				DistributionConfigSourceNormalJSON(std::string configFilePath, std::string * topLevel, int nTopLevel);

				/**
				 * Parse the JSON record provided for values belonging to a NormalDistribution entry
				 *
				 * @param parseJSON The contents of a "NormalDistribution" JSON record (not the level above.)
				 */
				DistributionConfigSourceNormalJSON(Json::Value& parseJSON);

				/**
				 * Constructor:
				 * Copy values from source to this newly constructed object
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				DistributionConfigSourceNormalJSON(const DistributionConfigSourceNormalJSON<I,T>& source);

				/**
				 * Deconstructor:
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				virtual ~DistributionConfigSourceNormalJSON();

				// === Overloaded Inherited Methods ===
				virtual void operator=(const DistributionConfigSourceNormalJSON<I,T>& source);
				virtual DistributionConfigSourceNormalJSON<I,T> * clone();

				cupcfd::error::eCodes getLBound(T * lBound);
				cupcfd::error::eCodes getUBound(T * uBound);
				cupcfd::error::eCodes getMean(T * mean);
				cupcfd::error::eCodes getStDev(T * stDev);
		};
	}
}

// Include Header Level Definitions
#include "DistributionConfigSourceNormalJSON.ipp"

#endif
