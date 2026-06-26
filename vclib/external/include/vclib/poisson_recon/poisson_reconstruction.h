// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

namespace vcl::poiss {

namespace detail {

template<typename Real>
struct RGBAColor
{
    RGBAColor(Real r = 0, Real g = 0, Real b = 0, Real a = 1) :
            r(r), g(g), b(b), a(a)
    {
    }

    RGBAColor& operator+=(const RGBAColor& c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        a += c.a;
        return *this;
    }

    RGBAColor& operator*=(Real s)
    {
        r *= s;
        g *= s;
        b *= s;
        a *= s;
        return *this;
    }

    RGBAColor& operator/=(Real s) { return operator*=(1 / s); }

    RGBAColor operator+(const RGBAColor& c) const
    {
        return RGBAColor(r + c.r, g + c.g, b + c.b, a + c.a);
    }

    RGBAColor operator*(Real s) const
    {
        return RGBAColor(r * s, g * s, b * s, a * s);
    }

    RGBAColor operator/(Real s) const { return operator*(1 / s); }

    Real r, g, b, a;
};

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

template<typename Real, typename MeshType>
struct MeshSampleWithColorStream :
        public PoissonRecon::Reconstructor::
            InputSampleStream<Real, 3, RGBAColor<Real>>
{
    MeshSampleWithColorStream(const MeshType& mesh) :
            mMesh(mesh), mIt(mesh.vertices().begin())
    {
    }

    void reset() { mIt = mMesh.vertices().begin(); }

    bool read(PoissonRecon::Point<Real, 3>& p, RGBAColor<Real>& c)
    {
        if (mIt == mMesh.vertices().end()) {
            return false;
        }

        const auto&  v     = *mIt;
        const auto&  vp    = v.position();
        const Color& color = v.color();

        p[0] = static_cast<Real>(vp.x());
        p[1] = static_cast<Real>(vp.y());
        p[2] = static_cast<Real>(vp.z());

        c.r = static_cast<Real>(color.redF());
        c.g = static_cast<Real>(color.greenF());
        c.b = static_cast<Real>(color.blueF());
        c.a = static_cast<Real>(color.alphaF());

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

template<typename Real, typename MeshType>
std::vector<RGBAColor<Real>> extrapolateColors(
    const MeshType&                                  mesh,
    const std::vector<PoissonRecon::Point<Real, 3>>& vertices,
    uint                                             depth,
    uint                                             width,
    uint                                             scale)
{
    using Extrapolator =
        PoissonRecon::Extrapolator::Implicit<Real, 3, RGBAColor<Real>>;

    MeshSampleWithColorStream<Real, MeshType> sampleStream(mesh);

    typename Extrapolator::Parameters params;
    params.depth = depth + 1;
    params.width = static_cast<Real>(width);
    params.scale = static_cast<Real>(scale);

    Extrapolator extrapolator(sampleStream, params, RGBAColor<Real>());

    std::vector<RGBAColor<Real>> colors(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        colors[i] = extrapolator(vertices[i]);
    }
    return colors;
}

template<typename Real>
Color toVclColor(const RGBAColor<Real>& c)
{
    auto channel = [](Real v) {
        v = std::clamp(v, static_cast<Real>(0), static_cast<Real>(1));
        return static_cast<uint8_t>(
            std::floor(v * 255 + static_cast<Real>(0.5)));
    };

    return Color(channel(c.r), channel(c.g), channel(c.b), channel(c.a));
}

} // namespace detail

} // namespace vcl::poiss

namespace PoissonRecon {

template<typename Real>
struct Atomic<vcl::poiss::detail::RGBAColor<Real>>
{
    using Color = vcl::poiss::detail::RGBAColor<Real>;

    static void Add(volatile Color& a, const Color& b)
    {
        Atomic<Real>::Add(a.r, b.r);
        Atomic<Real>::Add(a.g, b.g);
        Atomic<Real>::Add(a.b, b.b);
        Atomic<Real>::Add(a.a, b.a);
    }
};

} // namespace PoissonRecon

namespace vcl::poiss {

template<FaceMeshConcept OutMesh, MeshConcept MeshType>
OutMesh poissonReconstruction(
    const MeshType& mesh,
    uint            depth     = 8,
    uint            width     = 0,
    uint            scale     = 1,
    bool            linearFit = false)
{
    requirePerVertexNormal(mesh);

    OutMesh outMesh;

    if (mesh.vertexCount() == 0) {
        return outMesh;
    }

    using ScalarType = MeshType::VertexType::PositionType::ScalarType;
    using ReconType  = PoissonRecon::Reconstructor::Poisson;

    static constexpr unsigned int DIM     = 3;
    static constexpr unsigned int FEM_SIG = PoissonRecon::FEMDegreeAndBType<
        ReconType::DefaultFEMDegree,
        ReconType::DefaultFEMBoundary>::Signature;
    using FEMSigs = PoissonRecon::IsotropicUIntPack<DIM, FEM_SIG>;
    using Solver =
        typename ReconType::template Solver<ScalarType, DIM, FEMSigs>;
    using Implicit = typename PoissonRecon::Reconstructor::
        template Implicit<ScalarType, DIM, FEMSigs>;

    ReconType::template SolutionParameters<ScalarType> solverParams;
    solverParams.depth = depth;
    solverParams.width = static_cast<ScalarType>(width);
    solverParams.scale = static_cast<ScalarType>(scale);

    PoissonRecon::Reconstructor::LevelSetExtractionParameters extractionParams;
    extractionParams.linearFit = linearFit;

    detail::MeshOrientedSampleStream<ScalarType, MeshType> sampleStream(mesh);
    Implicit* implicit = Solver::Solve(sampleStream, solverParams);

    std::vector<PoissonRecon::Point<ScalarType, DIM>> vertices;
    std::vector<std::vector<unsigned int>>            faces;

    detail::OutputVertexStream<ScalarType> vStream(vertices);
    detail::OutputPolygonStream            fStream(faces);
    implicit->extractLevelSet(vStream, fStream, extractionParams);

    std::vector<detail::RGBAColor<ScalarType>> colors;
    bool                                       useVertexColor = false;
    if constexpr (HasPerVertexColor<MeshType> && HasPerVertexColor<OutMesh>) {
        if (isPerVertexColorAvailable(mesh)) {
            enableIfPerVertexColorOptional(outMesh);
            if (isPerVertexColorAvailable(outMesh)) {
                useVertexColor = true;
                colors         = detail::extrapolateColors(
                    mesh, vertices, depth, width, scale);
            }
        }
    }

    using OutPoint  = OutMesh::VertexType::PositionType;
    using OutScalar = OutPoint::ScalarType;

    outMesh.reserveVertices(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        const auto& p   = vertices[i];
        uint        vid = outMesh.addVertex(OutPoint(
            static_cast<OutScalar>(p[0]),
            static_cast<OutScalar>(p[1]),
            static_cast<OutScalar>(p[2])));
        if constexpr (HasPerVertexColor<OutMesh>) {
            if (useVertexColor) {
                outMesh.vertex(vid).color() = detail::toVclColor(colors[i]);
            }
        }
    }

    outMesh.reserveFaces(faces.size());
    for (const auto& f : faces) {
        outMesh.addFace(f);
    }

    delete implicit;

    return outMesh;
}

} // namespace vcl::poiss

#endif // VCL_POISSON_RECON_POISSON_RECONSTRUCTION_H
