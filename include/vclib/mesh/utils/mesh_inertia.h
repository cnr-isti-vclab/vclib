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

#ifndef VCL_MESH_UTILS_MESH_INERTIA_H
#define VCL_MESH_UTILS_MESH_INERTIA_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/point.h>

namespace vcl {

/**
 * @brief The MeshInertia class
 *
 * Methods for computing Polyhedral Mass properties (like inertia tensor, volume, etc)


 * The algorithm is based on a three step reduction of the volume integrals
 * to successively simpler integrals. The algorithm is designed to minimize
 * the numerical errors that can result from poorly conditioned alignment of
 * polyhedral faces. It is also designed for efficiency. All required volume
 * integrals of a polyhedron are computed together during a single walk over
 * the boundary of the polyhedron; exploiting common subexpressions reduces
 * floating point operations.
 *
 * For more information, check out:
 *
 * <b>Brian Mirtich,</b>
 * ``Fast and Accurate Computation of Polyhedral Mass Properties,''
 * journal of graphics tools, volume 1, number 2, 1996
 */
template<FaceMeshConcept MeshType>
class MeshInertia
{
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using ScalarType = typename VertexType::ScalarType;

public:
	MeshInertia(const MeshType& m);

	ScalarType         volume() const;
	Point3<ScalarType> centerOfMass() const;

	template<typename Matrix33>
	Matrix33 inertiaTensor() const;

	template<typename Matrix33>
	void inertiaTensorEigen(Matrix33& eigenValues, Point3<ScalarType>& eigenVector) const;

private:
	enum { X = 0, Y = 1, Z = 2 };
	static inline ScalarType sqr(const ScalarType& x) { return x * x; }
	static inline ScalarType cube(const ScalarType& x) { return x * x * x; }

	void faceIntegrals(const FaceType& f, const vcl::Point3<ScalarType>& normal);
	void projectionIntegrals(const FaceType& f);

	int A; // alpha
	int B; // beta
	int C; // gamma

	/* projection integrals */
	double P1, Pa, Pb, Paa, Pab, Pbb, Paaa, Paab, Pabb, Pbbb;

	/* face integrals */
	double Fa, Fb, Fc, Faa, Fbb, Fcc, Faaa, Fbbb, Fccc, Faab, Fbbc, Fcca;

	/* volume integrals */
	double T0, T1[3], T2[3], TP[3];
};

} // namespace vcl

#include "mesh_inertia.cpp"

#endif // VCL_MESH_UTILS_MESH_INERTIA_H
