/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Triangle class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_IPP_H

#include "ArithmeticKernels.h"
#include "EuclideanPlane3D.h"
#include "EuclideanVector3D.h"
#include "Triangle.h"

#include <iostream>

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Triangle3D<T>::Triangle3D(const euc::EuclideanPoint<T,3>& a,
								  	  const euc::EuclideanPoint<T,3>& b,
								      const euc::EuclideanPoint<T,3>& c)
			: Triangle<Triangle3D<T>, T, 3>(a, b, c)
			{
			}
			
			template <class T>
			Triangle3D<T>::Triangle3D(const Triangle3D<T>& source)
			: Triangle<Triangle3D<T>, T, 3>(source)
			{
			}

			template <class T>
			Triangle3D<T>::~Triangle3D()
			{
				// Nothing to do currently
			}

			// === Static Methods ===

			// template <class T>
			// inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> Triangle3D<T>::computeCentroid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
			// 																						const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
			// 																						const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c) {
			// 	// ToDo: This could be moved up into a generic polygon method....
			// 	// https://en.wikipedia.org/wiki/Centroid
			// 	// https://en.wikipedia.org/wiki/Median_(triangle)
			
			// 	// Centroid located at arithmetic mean of three points
			// 	return ((a + b + c) / T(3.0));
			// }
			
			// template <class T>
			// inline bool Triangle3D<T>::isPointInsideCentroid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
			// 												 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
			// 												 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
			// 												 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p) {
			// 	// Does the point lie on the same plane as the Triangle Points?
			// 	cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(a,b,c);
				
			// 	if(!(plane.isPointOnPlane(p))) {
			// 		return false;
			// 	}
				
			// 	// Does the point equal one of the triangle points - if so it counts as inside
			// 	if(p == a || p == b || p == c) {
			// 		return true;
			// 	}
				
			// 	// If the point lies on one of the edges it counts as inside
			// 	if(isPointOnLine(a, b, p) || isPointOnLine(a, c, p) || isPointOnLine(b, c, p)) {
			// 		return true;
			// 	}
				
			// 	// Compute the centroid
			// 	// cupcfd::geometry::euclidean::EuclideanPoint<T,3> centroid = Triangle3D<T>::computeCentroid(a, b, c);

				
			// 	// Test the intersection of the ray ranging from the point to the centroid
			// 	// if it intersects any of the faces, then the point p lies on the opposite side
			// 	// of an edge to the centroid, and so must be outside
				
			// 	bool abIntersect, acIntersect, bcIntersect;
				
			// 	abIntersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(p, centroid, a, b);
			// 	acIntersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(p, centroid, a, c);
			// 	bcIntersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(p, centroid, b, c);
				
			// 	if(abIntersect || acIntersect || bcIntersect) {
			// 		// One of the edges was intersected, so must be outside
			// 		return false;
			// 	}
				
			// 	// No intersections detected
			// 	return true;
			// }

			// === Concrete Methods ===

			template <class T>
			T Triangle3D<T>::computeArea() {
				return Triangle<Triangle3D<T>,T,3>::heronsFormula(this->vertices[0], this->vertices[1], this->vertices[2]);
			}

			template <class T>
			bool Triangle3D<T>::calculateIntersection(const euc::EuclideanPoint<T,3> v0, const euc::EuclideanVector<T,3> velocity, 
														euc::EuclideanPoint<T,3>& intersection, 
														T& timeToIntersect, 
														bool* onEdge,
														bool verbose) const
			{
				// http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection
				euc::EuclideanVector3D<T> e1 = this->vertices[1] - this->vertices[0];
				euc::EuclideanVector3D<T> e2 = this->vertices[2] - this->vertices[0];
				
				euc::EuclideanVector3D<T> vel(velocity);
				euc::EuclideanVector3D<T> h = vel.crossProduct(e2);
			
				T a = e1.dotProduct(h);

				T f = T(1)/a;

				euc::EuclideanVector3D<T> s = v0 - this->vertices[0];

				T u = f * s.dotProduct(h);

				euc::EuclideanVector3D<T> q = s.crossProduct(e1);

				T v = f * q.dotProduct(velocity);

				T t = f * q.dotProduct(e2);
				if (t == -T(0)) {
					t = T(0);
				}

				intersection = (T(1)-u-v)*this->vertices[0] + u*this->vertices[1] + v*this->vertices[2];

				if (u < T(0.0) || u > T(1.0)) {
					return false;
				}
				if (v < T(0.0) || (u+v) > T(1.0)) {
					return false;
				}

				if (verbose) {
					std::cout << "          > u=" << u << ", v=" << v << ", t=" <<t << std::endl;
				}
				timeToIntersect = t;

				if (u == T(0.0) || u == T(1.0) || v == T(0.0) || v == T(1.0)) {
					*onEdge = true;
				} else {
					*onEdge = false;
				}

				return true;
			}
		}
	}
}

#endif
