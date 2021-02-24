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

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_JSON_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_JSON_INCLUDE_H

// Error Codes
#include "Error.h"

#include "DistributionConfigSourceFixed.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace distributions
	{
		/**
		* Defines a interface for accessing DistributionConfigFixed options from a data source
		*
		* Format of JSON is as follows:
		*
		* TopLevel1 :
		* {
		* 		TopLevel2 :
		* 		...
		* 		{
		* 			FixedDistribution :
		* 			{
		* 				value: <Value>	#Required
		* 			}
		* 		}
		* }
		*
		* where TopLevels are any nested sections that desire a partitioner (e.g. Mesh)
		* and one of the three options should be selected.
		*
		 * @tparam The datatype of any indexing scheme used by the distribution
		 * @tparam The datatype of the values generated by the distribution
		 */
		template <class I, class T>
		class DistributionConfigSourceFixedJSON : public DistributionConfigSourceFixed<I,T>
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
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				DistributionConfigSourceFixedJSON(std::string configFilePath, std::string * topLevel, int nTopLevel);

				/**
				 * Parse the JSON record provided for values belonging to a FixedDistribution entry
				 *
				 * @param parseJSON The contents of a "FixedDistribution" JSON record (not the level above.)
				 */
				DistributionConfigSourceFixedJSON(Json::Value& parseJSON);

				/**
				 * Constructor:
				 * Copy values from source to this newly constructed object
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				DistributionConfigSourceFixedJSON(const DistributionConfigSourceFixedJSON<I,T>& source);

				/**
				 * Deconstructor:
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				~DistributionConfigSourceFixedJSON();

				// === Concrete Methods ===

				void operator=(const DistributionConfigSourceFixedJSON<I,T>& source);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getFixedValue(T * value);

				__attribute__((warn_unused_result))
				DistributionConfigSourceFixedJSON<I,T> * clone();
		};
	}
}

// Include Header Level Definitions
#include "DistributionConfigSourceFixedJSON.ipp"

#endif
