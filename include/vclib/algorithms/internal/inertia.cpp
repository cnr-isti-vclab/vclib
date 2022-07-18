/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "inertia.h"

#include "../polygon.h"

#include <Eigen/Eigenvalues>

namespace vcl::internal {

template <FaceMeshConcept MeshType>
Inertia<MeshType>::Inertia(const MeshType& m)
{
	double nx, ny, nz;

	T0 = T1[X] = T1[Y] = T1[Z]
		= T2[X] = T2[Y] = T2[Z]
		= TP[X] = TP[Y] = TP[Z] = 0;

	for (const FaceType& f : m.faces()) {
		if (vcl::polygonArea(f) > std::numeric_limits<float>::min()){
			Point3<ScalarType> fn = vcl::polygonNormal(f);
			fn.normalize();

			nx = std::abs(fn[0]);
			ny = std::abs(fn[1]);
			nz = std::abs(fn[2]);
			if (nx > ny && nx > nz) C = X;
			else C = (ny > nz) ? Y : Z;
			A = (C + 1) % 3;
			B = (A + 1) % 3;

			faceIntegrals(f, fn);

			T0 += fn[X] * ((A == X) ? Fa : ((B == X) ? Fb : Fc));

			T1[A] += fn[A] * Faa;
			T1[B] += fn[B] * Fbb;
			T1[C] += fn[C] * Fcc;
			T2[A] += fn[A] * Faaa;
			T2[B] += fn[B] * Fbbb;
			T2[C] += fn[C] * Fccc;
			TP[A] += fn[A] * Faab;
			TP[B] += fn[B] * Fbbc;
			TP[C] += fn[C] * Fcca;
		}
	}

	T1[X] /= 2; T1[Y] /= 2; T1[Z] /= 2;
	T2[X] /= 3; T2[Y] /= 3; T2[Z] /= 3;
	TP[X] /= 2; TP[Y] /= 2; TP[Z] /= 2;
}

/**
 * @brief Returns the volume (or mass) of the mesh.
 * Meaningful only if the mesh is watertight.
 * @return
 */
template<FaceMeshConcept MeshType>
typename Inertia<MeshType>::ScalarType Inertia<MeshType>::volume() const
{
	return T0;
}

template<FaceMeshConcept MeshType>
Point3<typename Inertia<MeshType>::ScalarType> Inertia<MeshType>::centerOfMass() const
{
	Point3<ScalarType>  r;
	r[X] = T1[X] / T0;
	r[Y] = T1[Y] / T0;
	r[Z] = T1[Z] / T0;
	return r;
}

template<FaceMeshConcept MeshType>
template<typename Matrix33>
Matrix33 Inertia<MeshType>::inertiaTensor() const
{
	Matrix33 J;
	Point3d r;
	r[X] = T1[X] / T0;
	r[Y] = T1[Y] / T0;
	r[Z] = T1[Z] / T0;
	/* compute inertia tensor */
	J(X,X) = (T2[Y] + T2[Z]);
	J(Y,Y) = (T2[Z] + T2[X]);
	J(Z,Z) = (T2[X] + T2[Y]);
	J(X,Y) = J(Y,X) = - TP[X];
	J(Y,Z) = J(Z,Y) = - TP[Y];
	J(Z,X) = J(X,Z) = - TP[Z];

	J(X,X) -= T0 * (r[Y]*r[Y] + r[Z]*r[Z]);
	J(Y,Y) -= T0 * (r[Z]*r[Z] + r[X]*r[X]);
	J(Z,Z) -= T0 * (r[X]*r[X] + r[Y]*r[Y]);
	J(X,Y) = J(Y,X) += T0 * r[X] * r[Y];
	J(Y,Z) = J(Z,Y) += T0 * r[Y] * r[Z];
	J(Z,X) = J(X,Z) += T0 * r[Z] * r[X];
	return J;
}

/**
 * @brief Computes the inertia tensor mesh.
 *
 * The result is factored as eigenvalues and eigenvectors (as ROWS).
 *
 * @param[out] eigenValues
 * @param[out] eigenVector
 */
template<FaceMeshConcept MeshType>
template<typename Matrix33>
void Inertia<MeshType>::inertiaTensorEigen(Matrix33& eigenValues, Point3<ScalarType>& eigenVector) const
{
	Eigen::Matrix3d it = inertiaTensor<Eigen::Matrix3d>();
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eig(it);
	Eigen::Vector3d c_val = eig.eigenvalues();
	Eigen::Matrix3d c_vec = eig.eigenvectors(); // eigenvector are stored as columns.
	c_vec.transposeInPlace();
	for (uint i = 0; i < 3; i++)
		for (uint j = 0; j < 3; j++)
			eigenValues(i,j) = c_vec(i,j);
	eigenVector = Point3<ScalarType>(c_val);
}

template<FaceMeshConcept MeshType>
void Inertia<MeshType>::faceIntegrals(const FaceType& f, const vcl::Point3<ScalarType>& n)
{
	ScalarType w;
	double k1, k2, k3, k4;

	projectionIntegrals(f);

	w = -f.vertex(0)->coord()*n;
	k1 = 1 / n[C]; k2 = k1 * k1; k3 = k2 * k1; k4 = k3 * k1;

	Fa = k1 * Pa;
	Fb = k1 * Pb;
	Fc = -k2 * (n[A]*Pa + n[B]*Pb + w*P1);

	Faa = k1 * Paa;
	Fbb = k1 * Pbb;
	Fcc = k3 * (sqr(n[A])*Paa + 2*n[A]*n[B]*Pab + sqr(n[B])*Pbb
				+ w*(2*(n[A]*Pa + n[B]*Pb) + w*P1));

	Faaa = k1 * Paaa;
	Fbbb = k1 * Pbbb;
	Fccc = -k4 * (cube(n[A])*Paaa + 3*sqr(n[A])*n[B]*Paab
				  + 3*n[A]*sqr(n[B])*Pabb + cube(n[B])*Pbbb
				  + 3*w*(sqr(n[A])*Paa + 2*n[A]*n[B]*Pab + sqr(n[B])*Pbb)
				  + w*w*(3*(n[A]*Pa + n[B]*Pb) + w*P1));

	Faab = k1 * Paab;
	Fbbc = -k2 * (n[A]*Pabb + n[B]*Pbbb + w*Pbb);
	Fcca = k3 * (sqr(n[A])*Paaa + 2*n[A]*n[B]*Paab + sqr(n[B])*Pabb
				 + w*(2*(n[A]*Paa + n[B]*Pab) + w*Pa));
}

/**
 * @brief Computes various integrations over projection of the given face
 * @param f
 */
template<FaceMeshConcept MeshType>
void Inertia<MeshType>::projectionIntegrals(const FaceType& f)
{
	double a0, a1, da;
	double b0, b1, db;
	double a0_2, a0_3, a0_4, b0_2, b0_3, b0_4;
	double a1_2, a1_3, b1_2, b1_3;
	double C1, Ca, Caa, Caaa, Cb, Cbb, Cbbb;
	double Cab, Kab, Caab, Kaab, Cabb, Kabb;

	P1 = Pa = Pb = Paa = Pab = Pbb = Paaa = Paab = Pabb = Pbbb = 0.0;

	for (uint i = 0; i < f.vertexNumber(); i++) {
		a0 = f.vertex(i)->coord()[A];
		b0 = f.vertex(i)->coord()[B];
		a1 = f.vertexMod(i+1)->coord()[A];
		b1 = f.vertexMod(i+1)->coord()[B];
		da = a1 - a0;
		db = b1 - b0;
		a0_2 = a0 * a0; a0_3 = a0_2 * a0; a0_4 = a0_3 * a0;
		b0_2 = b0 * b0; b0_3 = b0_2 * b0; b0_4 = b0_3 * b0;
		a1_2 = a1 * a1; a1_3 = a1_2 * a1;
		b1_2 = b1 * b1; b1_3 = b1_2 * b1;

		C1 = a1 + a0;
		Ca = a1*C1 + a0_2; Caa = a1*Ca + a0_3; Caaa = a1*Caa + a0_4;
		Cb = b1*(b1 + b0) + b0_2; Cbb = b1*Cb + b0_3; Cbbb = b1*Cbb + b0_4;
		Cab = 3*a1_2 + 2*a1*a0 + a0_2; Kab = a1_2 + 2*a1*a0 + 3*a0_2;
		Caab = a0*Cab + 4*a1_3; Kaab = a1*Kab + 4*a0_3;
		Cabb = 4*b1_3 + 3*b1_2*b0 + 2*b1*b0_2 + b0_3;
		Kabb = b1_3 + 2*b1_2*b0 + 3*b1*b0_2 + 4*b0_3;

		P1 += db*C1;
		Pa += db*Ca;
		Paa += db*Caa;
		Paaa += db*Caaa;
		Pb += da*Cb;
		Pbb += da*Cbb;
		Pbbb += da*Cbbb;
		Pab += db*(b1*Cab + b0*Kab);
		Paab += db*(b1*Caab + b0*Kaab);
		Pabb += da*(a1*Cabb + a0*Kabb);
	}

	P1 /= 2.0;
	Pa /= 6.0;
	Paa /= 12.0;
	Paaa /= 20.0;
	Pb /= -6.0;
	Pbb /= -12.0;
	Pbbb /= -20.0;
	Pab /= 24.0;
	Paab /= 60.0;
	Pabb /= -60.0;
}

} // namespace vcl
