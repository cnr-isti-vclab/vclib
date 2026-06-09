/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_POISSON_RECON_POISSON_RECONSTRUCTION_H
#define VCL_POISSON_RECON_POISSON_RECONSTRUCTION_H

#include <vclib/mesh.h>
#include <vclib/mesh/requirements/vertex_requirements.h>

// clang-format off
// inclusion order matters for PoissonRecon
#include <poisson_recon/PreProcessor.h>
#include <poisson_recon/Reconstructors.h>
#include <poisson_recon/Extrapolator.h>
// clang-format on

#include <utility>
#include <vector>

namespace vcl::poiss {

namespace detail {

template<typename Real, typename MeshType>
struct MeshOrientedSampleStream :
        public PoissonRecon::Reconstructor::InputOrientedSampleStream<Real, 3>
{
    MeshOrientedSampleStream(const MeshType& mesh) :
            mMesh(mesh), mIt(mesh.vertices().begin())
    {
    }

    void reset() { mIt = mMesh.vertices().begin(); }

    bool read(PoissonRecon::Point<Real, 3>& p, PoissonRecon::Point<Real, 3>& n)
    {
        if (mIt == mMesh.vertices().end()) {
            return false;
        }

        const auto& v  = *mIt;
        const auto& vp = v.position();
        const auto& vn = v.normal();

        p[0] = static_cast<Real>(vp.x());
        p[1] = static_cast<Real>(vp.y());
        p[2] = static_cast<Real>(vp.z());

        n[0] = static_cast<Real>(vn.x());
        n[1] = static_cast<Real>(vn.y());
        n[2] = static_cast<Real>(vn.z());

        ++mIt;
        return true;
    }

private:
    const MeshType&                                              mMesh;
    decltype(std::declval<const MeshType&>().vertices().begin()) mIt;
};

template<typename Real>
struct OutputVertexStream :
        public PoissonRecon::Reconstructor::OutputLevelSetVertexStream<Real, 3>
{
    using PointType = PoissonRecon::Point<Real, 3>;

    explicit OutputVertexStream(std::vector<PointType>& vertices) :
            mVertices(vertices)
    {
    }

    size_t size() const { return mVertices.size(); }

    size_t write(const PointType& p, const PointType&, const Real&)
    {
        mVertices.push_back(p);
        return mVertices.size() - 1;
    }

private:
    std::vector<PointType>& mVertices;
};

struct OutputPolygonStream :
        public PoissonRecon::Reconstructor::OutputFaceStream<2>
{
    explicit OutputPolygonStream(
        std::vector<std::vector<unsigned int>>& faces) : mFaces(faces)
    {
    }

    size_t size() const { return mFaces.size(); }

    size_t write(const std::vector<PoissonRecon::node_index_type>& polygon)
    {
        std::vector<unsigned int> f;
        f.reserve(polygon.size());
        for (PoissonRecon::node_index_type vi : polygon) {
            f.push_back(static_cast<unsigned int>(vi));
        }
        mFaces.push_back(std::move(f));
        return mFaces.size() - 1;
    }

private:
    std::vector<std::vector<unsigned int>>& mFaces;
};

} // namespace detail

template<FaceMeshConcept OutMesh, MeshConcept MeshType>
OutMesh poissonReconstruction(
    const MeshType& mesh,
    unsigned        depth     = 8,
    unsigned        width     = 0,
    unsigned        scale     = 1,
    bool            linearFit = false)
{
    requirePerVertexNormal(mesh);

    OutMesh outMesh;

    if (mesh.vertexCount() == 0) {
        return outMesh;
    }

    using Real      = typename MeshType::VertexType::PositionType::ScalarType;
    using ReconType = PoissonRecon::Reconstructor::Poisson;

    static constexpr unsigned int Dim    = 3;
    static constexpr unsigned int FEMSig = PoissonRecon::FEMDegreeAndBType<
        ReconType::DefaultFEMDegree,
        ReconType::DefaultFEMBoundary>::Signature;
    using FEMSigs  = PoissonRecon::IsotropicUIntPack<Dim, FEMSig>;
    using Solver   = typename ReconType::template Solver<Real, Dim, FEMSigs>;
    using Implicit = typename PoissonRecon::Reconstructor::
        template Implicit<Real, Dim, FEMSigs>;

    typename ReconType::template SolutionParameters<Real> solverParams;
    solverParams.depth = depth;
    solverParams.width = static_cast<Real>(width);
    solverParams.scale = static_cast<Real>(scale);

    PoissonRecon::Reconstructor::LevelSetExtractionParameters extractionParams;
    extractionParams.linearFit = linearFit;

    detail::MeshOrientedSampleStream<Real, MeshType> sampleStream(mesh);
    Implicit* implicit = Solver::Solve(sampleStream, solverParams);

    std::vector<PoissonRecon::Point<Real, Dim>> vertices;
    std::vector<std::vector<unsigned int>>      faces;

    detail::OutputVertexStream<Real> vStream(vertices);
    detail::OutputPolygonStream      fStream(faces);
    implicit->extractLevelSet(vStream, fStream, extractionParams);

    using OutScalar = typename OutMesh::VertexType::PositionType::ScalarType;
    using OutPoint  = typename OutMesh::VertexType::PositionType;

    for (const auto& p : vertices) {
        outMesh.addVertex(OutPoint(
            static_cast<OutScalar>(p[0]),
            static_cast<OutScalar>(p[1]),
            static_cast<OutScalar>(p[2])));
    }

    for (const auto& f : faces) {
        outMesh.addFace(f);
    }

    delete implicit;

    return outMesh;
}

} // namespace vcl::poiss

#endif // VCL_POISSON_RECON_POISSON_RECONSTRUCTION_H
