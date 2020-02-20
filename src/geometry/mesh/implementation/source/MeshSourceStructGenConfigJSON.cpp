/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the MeshSourceStructGenConfigJSON class.
 */

#include "MeshSourceStructGenConfigJSON.h"
#include <fstream>
#include "MeshSourceStructGenConfig.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			MeshSourceStructGenConfigJSON<I,T>::MeshSourceStructGenConfigJSON(std::string configFilePath, std::string * topLevel, int nTopLevel)
			{
				// Copy the top level strings
				for(int i = 0; i < nTopLevel; i++)
				{
					this->topLevel.push_back(topLevel[i]);
				}

				this->topLevel.push_back("MeshSourceStructGen");

				std::ifstream source(configFilePath, std::ifstream::binary);
				source >> this->configData;

				for(int i = 0; i < this->topLevel.size(); i++)
				{
					this->configData = this->configData[this->topLevel[i]];
				}
			}

			template <class I, class T>
			MeshSourceStructGenConfigJSON<I,T>::MeshSourceStructGenConfigJSON(MeshSourceStructGenConfigJSON<I,T>& source)
			{
				*this = source;
			}

			template <class I, class T>
			MeshSourceStructGenConfigJSON<I,T>::~MeshSourceStructGenConfigJSON()
			{

			}

			template <class I, class T>
			void MeshSourceStructGenConfigJSON<I,T>::operator=(MeshSourceStructGenConfigJSON<I,T>& source)
			{
				this->topLevel = source.topLevel;
				this->configData = source.configData;
			}

			template <class I, class T>
			MeshSourceStructGenConfigJSON<I,T> * MeshSourceStructGenConfigJSON<I,T>::clone()
			{
				return new MeshSourceStructGenConfigJSON<I,T>(*this);
			}


			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshCellSizeX(I * cellX)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["CellX"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just largest int for now and presume it converts down without loss of precision (not ideal)
					// ToDo: Should check it is actual an integer value, else it is an invalid value
					*cellX = I(dataSourceType.asLargestInt());
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshCellSizeY(I * cellY)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["CellY"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just largest int for now and presume it converts down without loss of precision (not ideal)
					// ToDo: Should check it is actual an integer value, else it is an invalid value
					*cellY = I(dataSourceType.asLargestInt());
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshCellSizeZ(I * cellZ)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["CellZ"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just largest int for now and presume it converts down without loss of precision (not ideal)
					// ToDo: Should check it is actual an integer value, else it is an invalid value
					*cellZ = I(dataSourceType.asLargestInt());
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshSpatialXMin(T * sMinX)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["SpatialXMin"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just use double for now and presume it converts down OK
					// ToDo: Should check it is actually a number and not a string
					*sMinX = T(dataSourceType.asDouble());

					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshSpatialYMin(T * sMinY)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["SpatialYMin"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just use double for now and presume it converts down OK
					// ToDo: Should check it is actually a number and not a string
					*sMinY = T(dataSourceType.asDouble());

					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshSpatialZMin(T * sMinZ)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["SpatialZMin"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just use double for now and presume it converts down OK
					// ToDo: Should check it is actually a number and not a string
					*sMinZ = T(dataSourceType.asDouble());

					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshSpatialXMax(T * sMaxX)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["SpatialXMax"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just use double for now and presume it converts down OK
					// ToDo: Should check it is actually a number and not a string
					*sMaxX = T(dataSourceType.asDouble());

					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshSpatialYMax(T * sMaxY)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["SpatialYMax"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just use double for now and presume it converts down OK
					// ToDo: Should check it is actually a number and not a string
					*sMaxY = T(dataSourceType.asDouble());

					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::getMeshSpatialZMax(T * sMaxZ)
			{
				cupcfd::error::eCodes status;

				const Json::Value dataSourceType = this->configData["SpatialZMax"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					// ToDo: Need template type checking or specialisation to check for correct type conversion
					// Should be reading it in as same type as I to prevent conversion errors/loss of precision
					// Will just use double for now and presume it converts down OK
					// ToDo: Should check it is actually a number and not a string
					*sMaxZ = T(dataSourceType.asDouble());

					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfigJSON<I,T>::buildMeshSourceConfig(MeshSourceConfig<I,T,I> ** meshSourceConfig)
			{
				cupcfd::error::eCodes status;
				I cellX, cellY, cellZ;
				T sMinX, sMaxX, sMinY, sMaxY, sMinZ, sMaxZ;

				status = this->getMeshCellSizeX(&cellX);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshCellSizeY(&cellY);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshCellSizeX(&cellZ);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshSpatialXMin(&sMinX);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshSpatialXMax(&sMaxX);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshSpatialYMin(&sMinY);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshSpatialYMax(&sMaxY);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshSpatialZMin(&sMinZ);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getMeshSpatialZMax(&sMaxZ);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				*meshSourceConfig = new MeshSourceStructGenConfig<I,T>(cellX, cellY, cellZ, sMinX, sMaxX, sMinY, sMaxY, sMinZ, sMaxZ);

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSourceStructGenConfigJSON<int, float>;
template class cupcfd::geometry::mesh::MeshSourceStructGenConfigJSON<int, double>;
