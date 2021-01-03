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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON3D_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON3D_3D_IPP_H

#include "EuclideanPlane3D.h"
#include "EuclideanVector2D.h"
#include "Triangle3D.h"
#include "Matrix.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class P, class T, uint V>
			Polygon3D<P,T,V>::Polygon3D() {
			}

			template <class P, class T, uint V>
			template<class...Args>
			Polygon3D<P,T,V>::Polygon3D(Args...v)
			: Polygon<Polygon3D<P,T,V>,T,3,V>::Polygon( (v)... )
			{
				static_assert(sizeof...(Args) == V, "Polygon constructor dimension does not match number of parameters");
				if (!this->coplanar()) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
				}

				if (!this->verifyNoEdgesIntersect()) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
				}
			}
			
			template <class P, class T, uint V>
			Polygon3D<P,T,V>::~Polygon3D() {
			}
			
			template <class P, class T, uint V>
			T Polygon3D<P,T,V>::computeArea() {
				T area = T(0);
				euc::EuclideanPoint<T,3> v0 = this->vertices[0];
				for (int i2=2; i2<V; i2++) {
					const uint i1 = i2-1;
					shapes::Triangle3D<T> t(v0, this->vertices[i1], this->vertices[i2]);
					area += t.computeArea();
				}
				return area;
			}
			
			template <class P, class T, uint V>
			T Polygon3D<P,T,V>::getArea() {
				if (!this->areaComputed) {
					this->area = this->computeArea();
					this->areaComputed = true;
				}
				return this->area;
			}

			template <class P, class T, uint V>
			euc::EuclideanPoint<T,3> Polygon3D<P,T,V>::computeCentroid() {
				// https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
				// Necessary assumption: the vertices forming this 3D polygon are co-planar.
				// Entering this process without knowing what plane the polygon lies on, so 
				// a little more work required. 
				// Process: calculate centroids of three projections (using Wiki method):
				//  1. XY plane
				//  2. YZ plane
				//  3. YZ plane
				// 'unwind' the projection to get the polygon centroid

				T area = this->computeArea();

				// 1. calculate centroid of XY projection
				euc::EuclideanPoint<T,3> cXY(T(0), T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cXY.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
					cXY.cmp[1] += (v1.cmp[1] + v2.cmp[1]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
				}
				cXY *= T(1) / (T(6) * area);

				// 2. calculate centroid of XZ projection
				euc::EuclideanPoint<T,3> cXZ(T(0), T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cXZ.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[2] - v2.cmp[0] * v1.cmp[2] );
					cXZ.cmp[2] += (v1.cmp[2] + v2.cmp[2]) * ( v1.cmp[0]*v2.cmp[2] - v2.cmp[0] * v1.cmp[2] );
				}
				cXZ *= T(1) / (T(6) * area);

				// 3. calculate centroid of YZ projection
				euc::EuclideanPoint<T,3> cYZ(T(0), T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cYZ.cmp[1] += (v1.cmp[1] + v2.cmp[1]) * ( v1.cmp[1]*v2.cmp[2] - v2.cmp[1] * v1.cmp[2] );
					cYZ.cmp[2] += (v1.cmp[2] + v2.cmp[2]) * ( v1.cmp[1]*v2.cmp[2] - v2.cmp[1] * v1.cmp[2] );
				}
				cYZ *= T(1) / (T(6) * area);

				// 4. 'unwind' projections:
				euc::EuclideanPoint<T,3> centroid;
				for (uint i=0; i<3; i++) {
					if (cXY.cmp[i] != T(0)) {
						centroid.cmp[i] = cXY.cmp[i];
					}
					else if (cXZ.cmp[i] != T(0)) {
						centroid.cmp[i] = cXZ.cmp[i];
					}
					else {
						centroid.cmp[i] = cYZ.cmp[i];
					}
				}

				return centroid;
			}

			template <class P, class T, uint V>
			euc::EuclideanPoint<T,3> Polygon3D<P,T,V>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}

			template <class P, class T, uint V>
			cupcfd::geometry::euclidean::EuclideanVector3D<T> Polygon3D<P,T,V>::computeNormal() {
				return euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
			}

			template <class P, class T, uint V>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> Polygon3D<P,T,V>::getNormal() {
				if (!this->normalComputed) {
					this->normal = this->computeNormal();
					this->normalComputed = true;
				}
				return this->normal;
			}

			template <class P, class T, uint V>
			bool Polygon3D<P,T,V>::isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p) {
				// Does the point lie on the same plane as the Quadrilateral Points?
				euc::EuclideanPlane3D<T> plane(this->vertices[0], this->vertices[1], this->vertices[2]);
				
				if(!(plane.isPointOnPlane(p))) {
					return false;
				}
				
				// Does the point equal one of the points - if so it counts as inside
				for (int i=0; i<V; i++) {
					if (p == this->vertices[i]) {
						return true;
					}
				}
				
				// If the point lies on one of the edges it counts as inside
				for (int i2=1; i2<V; i2++) {
					const int i1 = i2-1;
					if (isPointOnLine(this->vertices[i1], this->vertices[i2], p)) {
						return true;
					}
				}
				
				// Compute the centroid
				euc::EuclideanPoint<T,3> centroid = this->computeCentroid();
				
				// Test the intersection of the ray ranging from the point to the centroid. 
				// If it intersects any of the edges, then the point p lies on the opposite side
				// of an edge to the centroid, and so must be outside
				for (int i2=1; i2<V; i2++) {
					const int i1 = i2-1;
					if (euc::isVectorRangeIntersection(p, centroid, this->vertices[i1], this->vertices[i2])) {
						return false;
					}
				}
				return true;
			}

			template <class P, class T, uint V>
			bool Polygon3D<P,T,V>::coplanar() {
				// Calculate plane normal from first three vertices:
				euc::EuclideanVector3D<T> normal = this->computeNormal();
				// Check if any other vertex is NOT coplanar:
				for (int i=3; i<V; i++){
					euc::EuclideanVector3D<T> v = this->vertices[i] - this->vertices[0];
					T dp = v.dotProduct(normal);
					if (dp != T(0)) {
						printf("NOT COPLANAR! (dp = %.2e)\n", dp);
						return false;
					}
				}
				return true;
			}
			
			// Probably an actual geometric term for 'polygon edges do not intersect':
			// template <class S, class T, uint N, uint V>
			template <class P, class T, uint V>
			bool Polygon3D<P,T,V>::verifyNoEdgesIntersect() {
				// floating-point round errors could prevent detection of any true
				// 3D intersection point. Best bet is to rotate the polygon so
				// normal becomes Z-axis, then only working in 2D and will 
				// definitely find intersection point (unless parallel)

				// Operate on copy of vertices:
				euc::EuclideanPoint<T,3> vertices[V];
				for (int i=0; i<V; i++) {
					vertices[i] = this->vertices[i];
				}

				// Calculate rotation matrix:
				euc::EuclideanVector3D<T> normal = euc::EuclideanPlane3D<T>::calculateNormal(vertices[0], vertices[1], vertices[2]);
				normal.normalise();
				euc::EuclideanVector3D<T> zAxis(T(0), T(0), T(1));
				euc::Matrix<T,3,3> rotMatrix2;
				rotMatrix2 = euc::calculateRotationMatrix(normal, zAxis);

				// Apply rotation:
				for (int i=0; i<V; i++) {
					vertices[i] = rotMatrix2 * this->vertices[i];
				}
				euc::EuclideanVector3D<T> normalRotated = rotMatrix2 * normal;

				// Verify that rotation aligned polygon normal with Z-axis:
				euc::EuclideanVector3D<T> normalActual = euc::EuclideanPlane3D<T>::calculateNormal(vertices[0], vertices[1], vertices[2]);
				normalActual.normalise();
				T cosine = zAxis.dotProduct(normalActual);
				if (cosine < T(0.999)) {
					// Ideally 'cosine' should be 1.0 exactly, but allow 
					// a tiny tolerance for floating-point.
					printf("Polygon rotation failed to align its normal with Z-axis\n");

					printf("Polygon vertices before:\n");
					for (int i=0; i<V; i++) {
						printf("V %d: [ %.2f %.2f %.2f ]\n", i, this->vertices[i].cmp[0], this->vertices[i].cmp[1], this->vertices[i].cmp[2]);
					}
					
					printf("Polygon vertices after:\n");
					for (int i=0; i<V; i++) {
						printf("V %d: [ %.2f %.2f %.2f ]\n", i, vertices[i].cmp[0], vertices[i].cmp[1], vertices[i].cmp[2]);
					}
					printf("Normal after: ");
					normalActual.print(); std::cout << std::endl;
					printf("Normal rotated (should match above): ");
					normalRotated.print(); std::cout << std::endl;

					printf("Rotation matrix: \n");
					rotMatrix2.print(); std::cout << std::endl;

					printf("\n");

					HARD_CHECK_ECODE(cupcfd::error::E_ERROR);
				}

				// Now have polygon rotated into 2D X-Y plane, scan for any 
				// crossing edges. 
				// Permute through all possible pairs of edges - maybe there is a better 
				// way to scan, but this check is only performed once.
				for (uint e1=1; e1<V; e1++) {
					euc::EuclideanPoint<T,2> e1a(vertices[e1-1].cmp[0], vertices[e1-1].cmp[1]);
					euc::EuclideanPoint<T,2> e1b(vertices[e1  ].cmp[0], vertices[e1  ].cmp[1]);

					// Don't test connected edges:
					for (uint e2=e1+2; e2<V; e2++) {
						euc::EuclideanPoint<T,2> e2a(vertices[e2-1].cmp[0], vertices[e2-1].cmp[1]);
						euc::EuclideanPoint<T,2> e2b(vertices[e2  ].cmp[0], vertices[e2  ].cmp[1]);

						euc::EuclideanPoint<T,2> intersectPoint(T(0.0), T(0.0));
						cupcfd::error::eCodes status = euc::computeVectorRangeIntersection(e1a, e1b, e2a, e2b, intersectPoint);
						if (status == cupcfd::error::E_SUCCESS) {
							printf("Intersection detected!\n");

							printf("Polygon vertices:\n");
							for (int i=0; i<V; i++) {
								printf("V %d: [ %.2f %.2f %.2f ]\n", i, vertices[i].cmp[0], vertices[i].cmp[1], vertices[i].cmp[2]);
							}

							printf("Edges:\n");
							e1a.print(); printf(" --> "); e1b.print(); printf("\n");
							e2a.print(); printf(" --> "); e2b.print(); printf("\n");

							printf("Intersection point:\n");
							intersectPoint.print();

							return false;
						}
					}
				}

				return true;
			}
			
			template <class T>
			inline bool isVertexOrderClockwise(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& observation, cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int nPoints) {
				if (nPoints < 3) {
					throw(std::runtime_error("Polygon3D::isVertexOrderClockwise() called with < 3 points"));
				}
			
				// ToDo: Error Check - observation must not be coplanar with points
			
				// Draw a vector from the observation point to a point on the polygon
				cupcfd::geometry::euclidean::EuclideanVector3D<T> obsVec = observation - points[0];
				
				// Compute the normal of the polygon
				cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(points[0], points[1], points[2]);
				cupcfd::geometry::euclidean::EuclideanVector3D<T> normal = plane.getNormal();
				
				// If the points are traversed in clockwise order from the observation point, the vector will
				// point towards the observation point.
				// Compute the dot-product - if the vectors both point in the same direction (>0) then it is anti-clockwise from the observation point,
				// else it is clockwise from the observation point (since they will be in opposite directions)
				
				T dot = normal.dotProduct(obsVec);
				
				if(dot < 0) {
					// Anti-Clockwise
					return true;
				}
				
				return false;
			}
		}
	}
}

#endif
