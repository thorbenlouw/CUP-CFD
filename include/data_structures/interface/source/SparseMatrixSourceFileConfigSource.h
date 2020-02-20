/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSource class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_FILE_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_FILE_CONFIG_SOURCE_INCLUDE_H

#include "Error.h"
#include "SparseMatrixSourceConfigSource.h"
#include "SparseMatrixSourceFileConfig.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Interface class for retrieving data from a configuration data source
		 * for SparseMatrix Source Configurations that are specifically for
		 * Sparse Matrix Sources that are file-based.
		 */
		template <class I, class T>
		class SparseMatrixSourceFileConfigSource : public SparseMatrixSourceConfigSource<I,T>
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Default Constructor
				 *
				 * @tparam The datatype of any indexing scheme
				 * @tparam The datatype of the floating point values
				 */
				SparseMatrixSourceFileConfigSource();

				/**
				 * Constructor:
				 * Copy values from source to this newly constructed object
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				SparseMatrixSourceFileConfigSource(const SparseMatrixSourceFileConfigSource<I,T>& source);

				/**
				 * Deconstructor:
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				virtual ~SparseMatrixSourceFileConfigSource();

				// === Concrete Methods ===

				// === Virtual Methods ===

				/**
				 * Get the file path where the Sparse Matrix Source File can be found
				 */
				virtual cupcfd::error::eCodes getFilePath(std::string& filePath) = 0;

				/**
				 * Get the file format of the Sparse Matrix Source File
				 */
				virtual cupcfd::error::eCodes getFileFormat(SparseMatrixFileFormat * format) = 0;

				virtual void operator=(const SparseMatrixSourceFileConfigSource<I,T>& source);
				virtual SparseMatrixSourceFileConfigSource<I,T> * clone() = 0;
				virtual cupcfd::error::eCodes buildSparseMatrixSourceConfig(SparseMatrixSourceConfig<I,T> ** matrixSourceConfig) = 0;
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceFileConfigSource.ipp"

#endif