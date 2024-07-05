/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_AUX_MESH_INERTIA_H
#define VCL_MESH_AUX_MESH_INERTIA_H

#include <Eigen/Eigenvalues>

#include <vclib/algorithms/core/polygon.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/core/point.h>

namespace vcl {

/**
 * @brief The MeshInertia class provides several for computing Polyhedral Mass
 * properties (like inertia tensor, volume, etc).
 *
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
 *
 * @ingroup mesh_aux
 */
template<FaceMeshConcept MeshType>
class MeshInertia
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;
    using ScalarType = VertexType::CoordType::ScalarType;

    enum { X = 0, Y = 1, Z = 2 };

    int mA; // alpha
    int mB; // beta
    int mC; // gamma

    /* projection integrals */
    double mP1, mPa, mPb, mPaa, mPab, mPbb, mPaaa, mPaab, mPabb, mPbbb;

    /* face integrals */
    double mFa, mFb, mFc, mFaa, mFbb, mFcc;
    double mFaaa, mFbbb, mFccc, mFaab, mFbbc, mFcca;

    /* volume integrals */
    double mT0 = 0, mT1[3] = {0, 0, 0}, mT2[3] = {0, 0, 0}, mTP[3] = {0, 0, 0};

public:
    MeshInertia(const MeshType& m)
    {
        double nx, ny, nz;

        for (const FaceType& f : m.faces()) {
            if (vcl::faceArea(f) > std::numeric_limits<float>::min()) {
                Point3<ScalarType> fn = vcl::faceNormal(f);
                fn.normalize();

                nx = std::abs(fn[0]);
                ny = std::abs(fn[1]);
                nz = std::abs(fn[2]);
                if (nx > ny && nx > nz)
                    mC = X;
                else
                    mC = (ny > nz) ? Y : Z;
                mA = (mC + 1) % 3;
                mB = (mA + 1) % 3;

                faceIntegrals(f, fn);

                mT0 += fn[X] * ((mA == X) ? mFa : ((mB == X) ? mFb : mFc));

                mT1[mA] += fn[mA] * mFaa;
                mT1[mB] += fn[mB] * mFbb;
                mT1[mC] += fn[mC] * mFcc;
                mT2[mA] += fn[mA] * mFaaa;
                mT2[mB] += fn[mB] * mFbbb;
                mT2[mC] += fn[mC] * mFccc;
                mTP[mA] += fn[mA] * mFaab;
                mTP[mB] += fn[mB] * mFbbc;
                mTP[mC] += fn[mC] * mFcca;
            }
        }

        mT1[X] /= 2;
        mT1[Y] /= 2;
        mT1[Z] /= 2;
        mT2[X] /= 3;
        mT2[Y] /= 3;
        mT2[Z] /= 3;
        mTP[X] /= 2;
        mTP[Y] /= 2;
        mTP[Z] /= 2;
    }

    /**
     * @brief Returns the volume (or mass) of the mesh.
     * Meaningful only if the mesh is watertight.
     * @return
     */
    ScalarType volume() const { return mT0; }

    Point3<ScalarType> centerOfMass() const
    {
        Point3<ScalarType> r;
        r[X] = mT1[X] / mT0;
        r[Y] = mT1[Y] / mT0;
        r[Z] = mT1[Z] / mT0;
        return r;
    }

    template<typename Matrix33>
    Matrix33 inertiaTensor() const
    {
        Matrix33 J;
        Point3d  r;
        r[X] = mT1[X] / mT0;
        r[Y] = mT1[Y] / mT0;
        r[Z] = mT1[Z] / mT0;
        /* compute inertia tensor */
        J(X, X) = (mT2[Y] + mT2[Z]);
        J(Y, Y) = (mT2[Z] + mT2[X]);
        J(Z, Z) = (mT2[X] + mT2[Y]);
        J(X, Y) = J(Y, X) = -mTP[X];
        J(Y, Z) = J(Z, Y) = -mTP[Y];
        J(Z, X) = J(X, Z) = -mTP[Z];

        J(X, X) -= mT0 * (r[Y] * r[Y] + r[Z] * r[Z]);
        J(Y, Y) -= mT0 * (r[Z] * r[Z] + r[X] * r[X]);
        J(Z, Z) -= mT0 * (r[X] * r[X] + r[Y] * r[Y]);
        J(X, Y) = J(Y, X) += mT0 * r[X] * r[Y];
        J(Y, Z) = J(Z, Y) += mT0 * r[Y] * r[Z];
        J(Z, X) = J(X, Z) += mT0 * r[Z] * r[X];
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
    template<typename Matrix33>
    void inertiaTensorEigen(
        Matrix33&           eigenValues,
        Point3<ScalarType>& eigenVector) const
    {
        Eigen::Matrix3d it = inertiaTensor<Eigen::Matrix3d>();
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eig(it);
        Eigen::Vector3d c_val = eig.eigenvalues();
        // eigenvector are stored as columns.
        Eigen::Matrix3d c_vec = eig.eigenvectors();
        c_vec.transposeInPlace();
        for (uint i = 0; i < 3; i++)
            for (uint j = 0; j < 3; j++)
                eigenValues(i, j) = c_vec(i, j);
        eigenVector = Point3<ScalarType>(c_val);
    }

private:
    static ScalarType sqr(const ScalarType& x) { return x * x; }

    static ScalarType cube(const ScalarType& x) { return x * x * x; }

    void faceIntegrals(const FaceType& f, const vcl::Point3<ScalarType>& n)
    {
        ScalarType w;
        double     k1, k2, k3, k4;

        projectionIntegrals(f);

        w  = -f.vertex(0)->coord().dot(n);
        k1 = 1 / n[mC];
        k2 = k1 * k1;
        k3 = k2 * k1;
        k4 = k3 * k1;

        mFa = k1 * mPa;
        mFb = k1 * mPb;
        mFc = -k2 * (n[mA] * mPa + n[mB] * mPb + w * mP1);

        mFaa = k1 * mPaa;
        mFbb = k1 * mPbb;
        mFcc = k3 * (sqr(n[mA]) * mPaa + 2 * n[mA] * n[mB] * mPab +
                     sqr(n[mB]) * mPbb +
                     w * (2 * (n[mA] * mPa + n[mB] * mPb) + w * mP1));

        mFaaa = k1 * mPaaa;
        mFbbb = k1 * mPbbb;
        mFccc = -k4 * (cube(n[mA]) * mPaaa + 3 * sqr(n[mA]) * n[mB] * mPaab +
                       3 * n[mA] * sqr(n[mB]) * mPabb + cube(n[mB]) * mPbbb +
                       3 * w *
                           (sqr(n[mA]) * mPaa + 2 * n[mA] * n[mB] * mPab +
                            sqr(n[mB]) * mPbb) +
                       w * w * (3 * (n[mA] * mPa + n[mB] * mPb) + w * mP1));

        mFaab = k1 * mPaab;
        mFbbc = -k2 * (n[mA] * mPabb + n[mB] * mPbbb + w * mPbb);
        mFcca = k3 * (sqr(n[mA]) * mPaaa + 2 * n[mA] * n[mB] * mPaab +
                      sqr(n[mB]) * mPabb +
                      w * (2 * (n[mA] * mPaa + n[mB] * mPab) + w * mPa));
    }

    /**
     * @brief Computes various integrations over projection of the given face
     * @param f
     */
    void projectionIntegrals(const FaceType& f)
    {
        double a0, a1, da;
        double b0, b1, db;
        double a0_2, a0_3, a0_4, b0_2, b0_3, b0_4;
        double a1_2, a1_3, b1_2, b1_3;
        double C1, Ca, Caa, Caaa, Cb, Cbb, Cbbb;
        double Cab, Kab, Caab, Kaab, Cabb, Kabb;

        mP1 = mPa = mPb = mPaa = mPab = mPbb = mPaaa = mPaab = mPabb = mPbbb =
            0.0;

        for (uint i = 0; i < f.vertexNumber(); i++) {
            a0   = f.vertex(i)->coord()[mA];
            b0   = f.vertex(i)->coord()[mB];
            a1   = f.vertexMod(i + 1)->coord()[mA];
            b1   = f.vertexMod(i + 1)->coord()[mB];
            da   = a1 - a0;
            db   = b1 - b0;
            a0_2 = a0 * a0;
            a0_3 = a0_2 * a0;
            a0_4 = a0_3 * a0;
            b0_2 = b0 * b0;
            b0_3 = b0_2 * b0;
            b0_4 = b0_3 * b0;
            a1_2 = a1 * a1;
            a1_3 = a1_2 * a1;
            b1_2 = b1 * b1;
            b1_3 = b1_2 * b1;

            C1   = a1 + a0;
            Ca   = a1 * C1 + a0_2;
            Caa  = a1 * Ca + a0_3;
            Caaa = a1 * Caa + a0_4;
            Cb   = b1 * (b1 + b0) + b0_2;
            Cbb  = b1 * Cb + b0_3;
            Cbbb = b1 * Cbb + b0_4;
            Cab  = 3 * a1_2 + 2 * a1 * a0 + a0_2;
            Kab  = a1_2 + 2 * a1 * a0 + 3 * a0_2;
            Caab = a0 * Cab + 4 * a1_3;
            Kaab = a1 * Kab + 4 * a0_3;
            Cabb = 4 * b1_3 + 3 * b1_2 * b0 + 2 * b1 * b0_2 + b0_3;
            Kabb = b1_3 + 2 * b1_2 * b0 + 3 * b1 * b0_2 + 4 * b0_3;

            mP1 += db * C1;
            mPa += db * Ca;
            mPaa += db * Caa;
            mPaaa += db * Caaa;
            mPb += da * Cb;
            mPbb += da * Cbb;
            mPbbb += da * Cbbb;
            mPab += db * (b1 * Cab + b0 * Kab);
            mPaab += db * (b1 * Caab + b0 * Kaab);
            mPabb += da * (a1 * Cabb + a0 * Kabb);
        }

        mP1 /= 2.0;
        mPa /= 6.0;
        mPaa /= 12.0;
        mPaaa /= 20.0;
        mPb /= -6.0;
        mPbb /= -12.0;
        mPbbb /= -20.0;
        mPab /= 24.0;
        mPaab /= 60.0;
        mPabb /= -60.0;
    }
};

} // namespace vcl

#endif // VCL_MESH_AUX_MESH_INERTIA_H
