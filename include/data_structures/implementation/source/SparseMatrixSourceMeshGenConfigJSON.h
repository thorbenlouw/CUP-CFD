/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSourceMeshGenConfigJSON class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_MESH_GEN_CONFIG_JSON_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_MESH_GEN_CONFIG_JSON_INCLUDE_H

#include "Error.h"
#include "SparseMatrixSourceMeshGenConfigSource.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

#include <vector>

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Defines access for retrieving data from a JSON format for SparseMatrix Source Configurations
		 * that are specific to file based SparseMatrixSources.
		 */
		template <class I, class T>
		class SparseMatrixSourceMeshGenConfigJSON : public SparseMatrixSourceMeshGenConfigSource<I,T>
		{
			public:
				// === Members ===

				/**
				 * Define the nested fields to look under for the JSON data in the source file.
				 * The field "FixedDistribution" is always appended to this.
				 *
				 * E.g. If the contents are "FieldA", "FieldB", then
				 * any JSON data lookup for "FieldC" in other methods is done
				 * using [FieldA][FieldB][FixedDistribution] as the root (i.e. the data lookup would
				 * be [FieldA][FieldB][FixedDistribution][FieldC]
				 */
				std::vector<std::string> topLevel;

				/**
				 * Json Storage of Data
				 */
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Default Constructor
				 *
				 * @tparam The datatype of any indexing scheme
				 * @tparam The datatype of the floating point values
				 */
				SparseMatrixSourceMeshGenConfigJSON(std::string configFilePath, std::string * topLevel, int nTopLevel);

				/**
				 * Parse the JSON record provided for values belonging to a FixedDistribution entry
				 *
				 * @param parseJSON The contents of a JSON record with the appropriate fields
				 */
				SparseMatrixSourceMeshGenConfigJSON(Json::Value& parseJSON);

				/**
				 * Constructor:
				 * Copy values from source to this newly constructed object
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				SparseMatrixSourceMeshGenConfigJSON(const SparseMatrixSourceMeshGenConfigJSON<I,T>& source);

				/**
				 * Deconstructor:
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				virtual ~SparseMatrixSourceMeshGenConfigJSON();

				// === Overloaded Methods ===

				virtual void operator=(const SparseMatrixSourceMeshGenConfigJSON<I,T>& source);
				virtual SparseMatrixSourceMeshGenConfigJSON<I,T> * clone();
				virtual cupcfd::error::eCodes buildSparseMatrixSourceConfig(SparseMatrixSourceConfig<I,T> ** matrixSourceConfig);
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceMeshGenConfigJSON.ipp"

#endif