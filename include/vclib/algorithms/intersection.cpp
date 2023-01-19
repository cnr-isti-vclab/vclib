/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "intersection.h"

namespace vcl {

namespace internal {

// triangle box intersect functions
template<typename ScalarType>
inline void findMinMax(
	ScalarType x0,
	ScalarType x1,
	ScalarType x2,
	ScalarType &min,
	ScalarType &max) {
	min = max = x0;
	if (x1 < min)
		min = x1;
	if (x1 > max)
		max = x1;
	if (x2 < min)
		min = x2;
	if (x2 > max)
		max = x2;
}

/*======================== X-tests ========================*/
template<typename ScalarType, PointConcept PointType>
inline bool axisTestX01(
	ScalarType       a,
	ScalarType       b,
	ScalarType       fa,
	ScalarType       fb,
	const PointType& v0,
	const PointType& v2,
	const PointType& bHalfSixe)
{
	ScalarType p0 = a * v0.y() - b * v0.z();
	ScalarType p2 = a * v2.y() - b * v2.z();
	ScalarType min, max;
	if (p0 < p2) {
		min = p0;
		max = p2;
	} else {
		min = p2;
		max = p0;
	}
	ScalarType rad = fa * bHalfSixe.y() + fb * bHalfSixe.z();
	if (min > rad || max < -rad)
		return false;
	return true;
}

template<typename ScalarType, PointConcept PointType>
inline bool axisTestX2(
	ScalarType       a,
	ScalarType       b,
	ScalarType       fa,
	ScalarType       fb,
	const PointType& v0,
	const PointType& v1,
	const PointType& bHalfSixe)
{
	ScalarType p0 = a * v0.y() - b * v0.z();
	ScalarType p1 = a * v1.y() - b * v1.z();
	ScalarType min, max;
	if (p0 < p1) {
		min = p0;
		max = p1;
	} else {
		min = p1;
		max = p0;
	}
	ScalarType  rad = fa * bHalfSixe.y() + fb * bHalfSixe.z();
	if (min > rad || max < -rad)
		return false;
	return true;
}

/*======================== Y-tests ========================*/
template<typename ScalarType, PointConcept PointType>
inline bool axisTestY02(
	ScalarType       a,
	ScalarType       b,
	ScalarType       fa,
	ScalarType       fb,
	const PointType& v0,
	const PointType& v2,
	const PointType& bHalfSixe)
{
	ScalarType p0 = -a * v0.x() + b * v0.z();
	ScalarType p2 = -a * v2.x() + b * v2.z();
	ScalarType min, max;
	if (p0 < p2) {
		min = p0;
		max = p2;
	} else {
		min = p2;
		max = p0;
	}
	ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.z();
	if (min > rad || max < -rad)
		return false;
	return true;
}

template<typename ScalarType, PointConcept PointType>
inline bool axisTestY1(
	ScalarType       a,
	ScalarType       b,
	ScalarType       fa,
	ScalarType       fb,
	const PointType& v0,
	const PointType& v1,
	const PointType& bHalfSixe)
{
	ScalarType p0 = -a * v0.x() + b * v0.z();
	ScalarType p1 = -a * v1.x() + b * v1.z();
	ScalarType min, max;
	if (p0 < p1) {
		min = p0;
		max = p1;
	} else {
		min = p1;
		max = p0;
	}
	ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.z();
	if (min > rad || max < -rad)
		return false;
	return true;
}

/*======================== Z-tests ========================*/
template<typename ScalarType, PointConcept PointType>
inline bool axisTestZ12(
	ScalarType       a,
	ScalarType       b,
	ScalarType       fa,
	ScalarType       fb,
	const PointType& v1,
	const PointType& v2,
	const PointType& bHalfSixe)
{
	ScalarType p1 = a * v1.x() - b * v1.y();
	ScalarType p2 = a * v2.x() - b * v2.y();
	ScalarType min, max;
	if (p1 < p2) {
		min = p1;
		max = p2;
	} else {
		min = p2;
		max = p1;
	}
	ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.y();
	if (min > rad || max < -rad)
		return false;
	return true;
}

template<typename ScalarType, PointConcept PointType>
inline bool axisTestZ0(
	ScalarType       a,
	ScalarType       b,
	ScalarType       fa,
	ScalarType       fb,
	const PointType& v0,
	const PointType& v1,
	const PointType& bHalfSixe)
{
	ScalarType p0 = a * v0.x() - b * v0.y();
	ScalarType p1 = a * v1.x() - b * v1.y();
	ScalarType min, max;
	if (p0 < p1) {
		min = p0;
		max = p1;
	} else {
		min = p1;
		max = p0;
	}
	ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.y();
	if (min > rad || max < -rad)
		return false;
	return true;
}

} // namespace vcl::internal

/**
 * @brief Checks if a plane intersects with a box.
 *
 * https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
 *
 * @param p
 * @param b
 * @return
 */
template<typename PlaneType, typename BoxType>
bool planeBoxIntersect(const PlaneType& p, const BoxType& b)
{
	using PointType = typename BoxType::PointType;
	using ScalarType = typename PointType::ScalarType;

	// Convert AABB to center-extents representation
	PointType c = (b.max + b.min) * 0.5f; // Compute AABB center
	PointType e = b.max - c; // Compute positive extents

	PointType n = p.direction();
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	ScalarType r = e[0]*std::abs(n[0]) + e[1]*std::abs(n[1]) + e[2]*std::abs(n[2]);

	// Compute distance of box center from plane
	ScalarType s = n.dot(c) - p.offset();

	// Intersection occurs when distance s falls within [-r,+r] interval
	return std::abs(s) <= r;
}

/**
 * @brief Checks if a triangle intersects with a box.
 * https://gist.github.com/jflipts/fc68d4eeacfcc04fbdb2bf38e0911850
 */
template<PointConcept PointType>
bool triangleBoxIntersect(
	const PointType&      tv0,
	const PointType&      tv1,
	const PointType&      tv2,
	const Box<PointType>& b) requires (PointType::DIM == 3)
{
	using ScalarType = typename PointType::ScalarType;

	PointType boxcenter = b.center();
	PointType bHalfSixe = b.size() / 2;

	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	ScalarType min, max;
	PointType normal;

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	PointType v0 = tv0 - boxcenter;
	PointType v1 = tv1 - boxcenter;
	PointType v2 = tv2 - boxcenter;

	/* compute triangle edges */
	PointType e0 = v1 - v0;
	PointType e1 = v2 - v1;
	PointType e2 = v0 - v2;

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	ScalarType fex = std::abs(e0.x());
	ScalarType fey = std::abs(e0.y());
	ScalarType fez = std::abs(e0.z());

	if (!internal::axisTestX01(e0.z(), e0.y(), fez, fey, v0, v2, bHalfSixe))
		return false;
	if (!internal::axisTestY02(e0.z(), e0.x(), fez, fex, v0, v2, bHalfSixe))
		return false;
	if (!internal::axisTestZ12(e0.y(), e0.x(), fey, fex, v1, v2, bHalfSixe))
		return false;

	fex = std::abs(e1.x());
	fey = std::abs(e1.y());
	fez = std::abs(e1.z());

	if (!internal::axisTestX01(e1.z(), e1.y(), fez, fey, v0, v2, bHalfSixe))
		return false;
	if (!internal::axisTestY02(e1.z(), e1.x(), fez, fex, v0, v2, bHalfSixe))
		return false;
	if (!internal::axisTestZ0(e1.y(), e1.x(), fey, fex, v0, v1, bHalfSixe))
		return false;

	fex = std::abs(e2.x());
	fey = std::abs(e2.y());
	fez = std::abs(e2.z());
	if (!internal::axisTestX2(e2.z(), e2.y(), fez, fey, v0, v1, bHalfSixe))
		return false;
	if (!internal::axisTestY1(e2.z(), e2.x(), fez, fex, v0, v1, bHalfSixe))
		return false;
	if (!internal::axisTestZ12(e2.y(), e2.x(), fey, fex, v1, v2, bHalfSixe))
		return false;

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	internal::findMinMax(v0.x(), v1.x(), v2.x(), min, max);
	if (min > bHalfSixe.x() || max < -bHalfSixe.x())
		return false;

	/* test in Y-direction */
	internal::findMinMax(v0.y(), v1.y(), v2.y(), min, max);
	if (min > bHalfSixe.y() || max < -bHalfSixe.y())
		return false;

	/* test in Z-direction */
	internal::findMinMax(v0.z(), v1.z(), v2.z(), min, max);
	if (min > bHalfSixe.z() || max < -bHalfSixe.z())
		return false;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	normal = e0.cross(e1);
	vcl::Plane<ScalarType> plane(tv0, tv1, tv2);
	if (!planeBoxIntersect(plane, b))
		return false;

	return true; /* box and triangle overlaps */
}

template<FaceConcept FaceType, PointConcept PointType>
bool faceBoxIntersect(const FaceType& f, const Box<PointType>& b)
{
	if constexpr(TriangleFaceConcept<FaceType>) {
		return triangleBoxIntersect(
			f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord(), b);
	}
	else {
		bool b = false;
		std::vector<uint> tris = vcl::earCut(f);
		for (uint i = 0; i < tris.size() && !b; i += 3) {
			b |= triangleBoxIntersect(
				f.vertex(tris[i])->coord(),
				f.vertex(tris[i + 1])->coord(),
				f.vertex(tris[i + 2])->coord(),
				b);
		}
		return b;
	}
}

template<EdgeMeshConcept EdgeMesh, FaceMeshConcept MeshType, typename PlaneType>
void meshPlaneIntersection(EdgeMesh& em, const MeshType& m, const PlaneType& pl)
{
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using CoordType  = typename VertexType::CoordType;

	std::vector<double> qH(m.vertexContainerSize());

	for (const VertexType& v : m.vertices())
		qH[m.index(v)] = pl.dist(v.coord());

	for (const FaceType& f : m.faces()) {
		std::vector<CoordType>  ptVec;
		std::vector<CoordType> nmVec;
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (qH[m.index(f.vertex(j))] == 0) {
				ptVec.push_back(f.vertex(j)->coord());
				if constexpr(HasPerVertexNormal<MeshType>) {
					if (isPerVertexNormalEnabled(m)) {
						nmVec.push_back(f.vertex(j)->normal());
					}
				}
			}
			else if ((qH[m.index(f.vertex(j))] * qH[m.index(f.vertexMod(j + 1))]) < 0) {
				const CoordType&  p0 = f.vertex(j)->coord();
				const CoordType&  p1 = f.vertexMod(j + 1)->coord();
				float             q0 = qH[m.index(f.vertex(j))];
				float             q1 = qH[m.index(f.vertexMod(j + 1))];

				std::pair<CoordType, CoordType> seg(p0, p1);
				CoordType                       pp = pl.segmentIntersection(seg);
				ptVec.push_back(pp);
				if constexpr(HasPerVertexNormal<MeshType>) {
					if (isPerVertexNormalEnabled(m)) {
						using NormalType = typename VertexType::NormalType;
						const NormalType& n0 = f.vertex(j)->normal();
						const NormalType& n1 = f.vertexMod(j + 1)->normal();
						CoordType nn = (n0 * fabs(q1) + n1 * fabs(q0)) / fabs(q0 - q1);
						nmVec.push_back(nn);
					}
				}
			}
		}
		if (ptVec.size() >= 2) {
			uint eid               = em.addEdge();
			uint v0                = em.addVertices(2);
			uint v1                = v0 + 1;
			em.vertex(v0).coord()  = ptVec[0];
			em.vertex(v1).coord()  = ptVec[1];
			em.edge(eid).vertex(0) = &em.vertex(v0);
			em.edge(eid).vertex(1) = &em.vertex(v1);
			if constexpr (HasPerVertexNormal<MeshType> && HasPerVertexNormal<EdgeMesh>) {
				if (isPerVertexNormalEnabled(m) && isPerVertexNormalEnabled(em)) {
					em.vertex(v0).normal() = nmVec[0];
					em.vertex(v1).normal() = nmVec[1];
				}
			}
		}
	}

//	// Clean-up: Remove duplicate vertex
//	tri::Clean<EdgeMeshType>::RemoveDuplicateVertex(em);

//	// Clean-up: Sort edges ensuring orientation coherence
//	for (size_t j = 1; j < em.edge.size(); j++) {
//		auto& n = em.edge[j - 1].V(1);
//		for (size_t i = j; i < em.edge.size(); i++) {
//			auto& ei = em.edge[i];
//			if (ei.V(1) == n)
//				std::swap(ei.V(0), ei.V(1));
//			if (ei.V(0) == n) {
//				std::swap(em.edge[j], em.edge[i]);
//				break;
//			}
//		}
	//	}
}

} // namespace vcl
