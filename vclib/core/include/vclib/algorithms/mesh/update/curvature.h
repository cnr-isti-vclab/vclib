// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_CURVATURE_H
#define VCL_ALGORITHMS_MESH_UPDATE_CURVATURE_H

#include <vclib/algorithms/mesh/intersection.h>
#include <vclib/algorithms/mesh/point_sampling.h>
#include <vclib/algorithms/mesh/stat.h>
#include <vclib/algorithms/mesh/update/normal.h>

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>
#include <vclib/space/complex.h>
#include <vclib/space/core.h>

#include <mutex>

namespace vcl {

enum class PrincipalCurvatureAlgorithm { TAUBIN95, PCA };

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvatureTaubin95(MeshType& m, LogType& log = nullLogger)
{
    requireVertexPrincipalCurvature(m);
    requireVertexAdjacentFaces(m);
    requireFaceAdjacentFaces(m);

    using ConstVertexType = ConstVertexType<MeshType>;
    using PositionType    = PositionType<MeshType>;
    using ScalarType      = PositionType::ScalarType;

    // Auxiliary data structure
    struct AdjVertex
    {
        ConstVertexType vert;
        double          doubleArea;
        bool            isBorder;
    };

    log.log(0, "Updating per vertex normals...");

    updateVertexNormalsAngleWeighted(m);
    normalizeVertexNormals(m);

    log.log(5, "Computing per vertex curvature...");
    // log every 5%, starting from 5% to 100%
    log.startProgress(
        "", m.vertexCount(), LogType::ProgressMode::TIME, 1.0, 5, 100);

    for (auto v : m.vertices()) {
        std::vector<ScalarType> weights;
        std::vector<AdjVertex>  vertices;

        auto  pos                 = MeshPos(v.adjFace(0), v);
        auto  firstVertex         = pos.adjVertex();
        auto  tmpVertex           = ConstVertexType();
        float totalDoubleAreaSize = 0;

        // compute the area of each triangle around the central vertex as well
        // as their total area
        do {
            pos.nextEdgeAdjacentToV();
            tmpVertex = pos.adjVertex();
            AdjVertex adjV;
            adjV.isBorder   = pos.isEdgeOnBorder();
            adjV.vert       = tmpVertex;
            adjV.doubleArea = faceArea(pos.face()) * 2;
            totalDoubleAreaSize += adjV.doubleArea;
            vertices.push_back(adjV);
        } while (tmpVertex != firstVertex);

        weights.reserve(vertices.size());

        for (size_t i = 0; i < vertices.size(); ++i) {
            if (vertices[i].isBorder) {
                weights.push_back(vertices[i].doubleArea / totalDoubleAreaSize);
            }
            else {
                weights.push_back(
                    0.5f *
                    (vertices[i].doubleArea +
                     vertices[(i + vertices.size() - 1) % vertices.size()]
                         .doubleArea) /
                    totalDoubleAreaSize);
            }
            assert(weights.back() < 1.0f);
        }

        // compute I-NN^t to be used for computing the T_i's
        Matrix33<ScalarType> Tp;

        auto n = v.normal();
        for (int i = 0; i < 3; ++i)
            Tp(i, i) = 1.0f - std::pow(n[i], 2);
        Tp(0, 1) = Tp(1, 0) = -1.0f * (n[0] * n[1]);
        Tp(1, 2) = Tp(2, 1) = -1.0f * (n[1] * n[2]);
        Tp(0, 2) = Tp(2, 0) = -1.0f * (n[0] * n[2]);

        // for all neighbors vi compute the directional curvatures k_i and the
        // T_i compute M by summing all w_i k_i T_i T_i^t
        Matrix33<ScalarType> tempMatrix;
        Matrix33<ScalarType> M = Matrix33<ScalarType>::Zero();
        for (size_t i = 0; i < vertices.size(); ++i) {
            PositionType edge = (v.position() - vertices[i].vert.position());
            float        curvature =
                (2.0f * (v.normal().dot(edge))) / edge.squaredNorm();
            PositionType t = Tp * edge;
            t.normalize();
            tempMatrix = t.outerProduct(t);
            M += tempMatrix * weights[i] * curvature;
        }

        // compute vector W for the Householder matrix
        PositionType w;
        PositionType e1(1.0f, 0.0f, 0.0f);
        if ((e1 - v.normal()).squaredNorm() > (e1 + v.normal()).squaredNorm())
            w = e1 - v.normal();
        else
            w = e1 + v.normal();
        w.normalize();

        // compute the Householder matrix I - 2WW^t
        Matrix33<ScalarType> Q = Matrix33<ScalarType>::Identity();
        tempMatrix             = w.outerProduct(w);
        Q -= tempMatrix * 2.0f;

        // compute matrix Q^t M Q
        Matrix33<ScalarType> QtMQ = (Q.transpose() * M * Q);

        Eigen::Matrix<ScalarType, 1, 3> T1 = Q.col(1);
        Eigen::Matrix<ScalarType, 1, 3> T2 = Q.col(2);

        // diagonalize the 2x2 restriction of M in the tangent plane basis (T1,
        // T2)
        Eigen::Matrix<ScalarType, 2, 2> minor2x2;
        minor2x2(0, 0) = QtMQ(1, 1);
        minor2x2(0, 1) = QtMQ(1, 2);
        minor2x2(1, 0) = QtMQ(2, 1);
        minor2x2(1, 1) = QtMQ(2, 2);

        Eigen::SelfAdjointEigenSolver<Eigen::Matrix<ScalarType, 2, 2>> es(
            minor2x2);
        // es.eigenvalues() are sorted in ascending order: lambda0 <= lambda1
        ScalarType l0 = es.eigenvalues()[0];
        ScalarType l1 = es.eigenvalues()[1];

        // Taubin95 formula:
        // k0 = 3*l0 - l1
        // k1 = 3*l1 - l0
        // Since l0 <= l1, k0 <= k1 holds: (3*l1 - l0) - (3*l0 - l1) = 4*(l1 -
        // l0) >= 0
        ScalarType k0 = 3.0f * l0 - l1;
        ScalarType k1 = 3.0f * l1 - l0;

        auto ev0 = es.eigenvectors().col(0);
        auto ev1 = es.eigenvectors().col(1);

        Eigen::Matrix<ScalarType, 1, 3> dir0 = T1 * ev0[0] + T2 * ev0[1];
        Eigen::Matrix<ScalarType, 1, 3> dir1 = T1 * ev1[0] + T2 * ev1[1];

        v.principalCurvature().maxValue() = k1;
        v.principalCurvature().minValue() = k0;
        v.principalCurvature().maxDir()   = dir1.normalized();
        v.principalCurvature().minDir()   = dir0.normalized();

        log.progress(v.index());
    }

    log.endProgress();
    log.log(100, "Per vertex curvature computed.");
}

/**
 * @brief Computes the Principal Curvature meseaure as described in the paper:
 * Robust principal curvatures on Multiple Scales, Yong-Liang Yang, Yu-Kun Lai,
 * Shi-Min Hu Helmut Pottmann SGP 2004. If montecarloSampling==true the
 * covariance is computed by montecarlo sampling on the mesh (faster); If
 * montecarloSampling==false the covariance is computed by (analytic)integration
 * over the surface (slower).
 * @param m
 * @param radius
 * @param montecarloSampling
 * @param log
 */
template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvaturePCA(
    MeshType& m,
    auto      radius,
    bool      montecarloSampling = true,
    LogType&  log                = nullLogger)
{
    using VertexType   = VertexType<MeshType>;
    using PositionType = VertexType::PositionType;
    using ScalarType   = PositionType::ScalarType;

    using VGrid = StaticGrid3<VertexType, ScalarType>;

    VGrid      pGrid;
    ScalarType area;

    log.log(0, "Updating per vertex normals...");

    updateVertexNormalsAngleWeighted(m);
    normalizeVertexNormals(m);

    log.log(0, "Computing per vertex curvature...");
    log.startProgress("", m.vertexCount());

    if (montecarloSampling) {
        area  = surfaceArea(m);
        pGrid = VGrid(m.vertices());
    }

    parallelFor(m.vertices(), [&](VertexType v) {
        // for (VertexType& v : m.vertices()) {
        Matrix33<ScalarType> A, eigenvectors;
        PositionType         bp, eigenvalues;
        if (montecarloSampling) {
            Sphere s(v.position(), radius);
            auto   vec = pGrid.valuesInSphere(s); // vector of iterators
            std::vector<PositionType> points;
            points.reserve(vec.size());
            for (const auto& it : vec) {
                points.push_back(it->second.position());
            }
            A = covarianceMatrixOfPointCloud(points);
            A *= area * area / 1000;
        }
        else {
            Sphere<ScalarType> sph(v.position(), radius);
            MeshType           tmpMesh = intersection(m, sph);

            A = covarianceMatrixOfMesh(tmpMesh);
        }

        Eigen::SelfAdjointEigenSolver<Eigen::Matrix<ScalarType, 3, 3>> eig(A);
        eigenvalues  = PositionType(eig.eigenvalues());
        eigenvectors = eig.eigenvectors(); // eigenvector are stored as columns.
        // get the estimate of curvatures from eigenvalues and eigenvectors
        // find the 2 most tangent eigenvectors (by finding the one closest to
        // the normal)
        uint       best  = 0;
        ScalarType bestv = std::abs(
            v.normal().dot(PositionType(eigenvectors.col(0).normalized())));
        for (uint i = 1; i < 3; ++i) {
            ScalarType prod = std::abs(
                v.normal().dot(PositionType(eigenvectors.col(i).normalized())));
            if (prod > bestv) {
                bestv = prod;
                best  = i;
            }
        }
        v.principalCurvature().maxDir() =
            (eigenvectors.col((best + 1) % 3).normalized());
        v.principalCurvature().minDir() =
            (eigenvectors.col((best + 2) % 3).normalized());

        Matrix33<ScalarType> rot;
        ScalarType           angle;
        angle = acos(v.principalCurvature().maxDir().dot(v.normal()));

        rot = rotationMatrix<Matrix33<ScalarType>>(
            PositionType(v.principalCurvature().maxDir().cross(v.normal())),
            -(M_PI * 0.5 - angle));

        v.principalCurvature().maxDir() = rot * v.principalCurvature().maxDir();

        angle = acos(v.principalCurvature().minDir().dot(v.normal()));

        rot = rotationMatrix<Matrix33<ScalarType>>(
            PositionType(v.principalCurvature().minDir().cross(v.normal())),
            -(M_PI * 0.5 - angle));

        v.principalCurvature().minDir() = rot * v.principalCurvature().minDir();

        // computes the curvature values
        const ScalarType r5 = std::pow(radius, 5);
        const ScalarType r6 = r5 * radius;
        v.principalCurvature().maxValue() =
            (2.0 / 5.0) *
            (4.0 * M_PI * r5 + 15 * eigenvalues[(best + 2) % 3] -
             45.0 * eigenvalues[(best + 1) % 3]) /
            (M_PI * r6);
        v.principalCurvature().minValue() =
            (2.0 / 5.0) *
            (4.0 * M_PI * r5 + 15 * eigenvalues[(best + 1) % 3] -
             45.0 * eigenvalues[(best + 2) % 3]) /
            (M_PI * r6);
        if (v.principalCurvature().maxValue() <
            v.principalCurvature().minValue()) {
            std::swap(
                v.principalCurvature().minValue(),
                v.principalCurvature().maxValue());
            std::swap(
                v.principalCurvature().minDir(),
                v.principalCurvature().maxDir());
        }

        log.progress(v.index());
        //}
    });

    log.endProgress();
    log.log(100, "Per vertex curvature computed.");
}

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvature(MeshType& m, LogType& log = nullLogger)
{
    requireVertexPrincipalCurvature(m);

    updatePrincipalCurvatureTaubin95(m, log);
}

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvature(
    MeshType&                   m,
    PrincipalCurvatureAlgorithm alg = PrincipalCurvatureAlgorithm::TAUBIN95,
    LogType&                    log = nullLogger)
{
    using enum PrincipalCurvatureAlgorithm;
    requireVertexPrincipalCurvature(m);

    double radius;
    switch (alg) {
    case TAUBIN95: updatePrincipalCurvatureTaubin95(m, log); break;
    case PCA:
        radius = boundingBox(m).diagonal() * 0.1;
        updatePrincipalCurvaturePCA(m, radius, true, log);
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_CURVATURE_H
