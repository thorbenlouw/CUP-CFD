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
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Polygon shapes.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads.
			 * 'auto' return types force compile-time checking that derived 
			 * classes have implemented functions.
			 *
			 * @tparam S The polygon specialisation
			 * @tparam T Numerical type
			 * @tparam N Number of spatial dimensions
			 * @tparam V Number of vertices
			 */
			template <class S, class T, uint N, uint V>
			class Polygon
			{
				public:
					// === Members ===
					const int numVertices = V;
					euc::EuclideanPoint<T,N> vertices[V];

					// === Constructors/Deconstructors ===

					Polygon();

					template<class...Args>
					Polygon(Args...v);

					virtual ~Polygon();

					// === Concrete Methods ===

					/**
					 * Copys the data from the source to this object
					 *
					 * @param source The Polygon to copy from
					 */
					inline void operator=(const Polygon<S,T,N,V>& source);

					/**
					 * Reverse vertex ordering, and flip normal
					 */
					void reverseVertexOrdering();

					/**
					 * Determine whether the provided point is inside the Polygon.
					 * Edges/Vertices are treated as inside the Polygon for this purpose.
					 *
					 * @return Return true if the point exists inside this Polygon
					 */
					__attribute__((warn_unused_result))
					auto isPointInside(const euc::EuclideanPoint<T,N>& point);

					__attribute__((warn_unused_result))
					auto getArea();

					__attribute__((warn_unused_result))
					auto getCentroid();

				protected:
					// This class contains all data needed/used by derived classes.
					// Derived classes just implement new methods.
					T area;
					bool areaComputed = false;

					euc::EuclideanPoint<T,N> centroid;
					bool centroidComputed = false;

					euc::EuclideanVector<T,N> normal;
					bool normalComputed = false;

			};
		}
	}
}

// Include Header Level Definitions
#include "Polygon.ipp"

#endif
