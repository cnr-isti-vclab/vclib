/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_SPACE_COMPLEX_SAMPLER_MESH_SAMPLER_H
#define VCL_SPACE_COMPLEX_SAMPLER_MESH_SAMPLER_H

#include <vclib/algorithms/core/polygon.h>
#include <vclib/concepts/mesh/elements/edge.h>
#include <vclib/concepts/mesh/mesh_concept.h>
#include <vclib/concepts/space/sampler.h>
#include <vclib/mesh/requirements/vertex_requirements.h>
#include <vclib/views/mesh.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshSampler
{
    using CoordView =
        decltype(View<typename MeshType::VertexIterator>() | views::coords);

    MeshType mMesh;

public:
    using PointType     = MeshType::VertexType::PositionType;
    using ScalarType    = PointType::ScalarType;
    using ConstIterator = std::ranges::iterator_t<CoordView>;

    MeshSampler()
    {
        enableIfPerVertexNormalOptional(mMesh);
        enableIfPerVertexQualityOptional(mMesh);
        if constexpr (HasName<MeshType>) {
            mMesh.name() = "Sampling";
        }
    }

    const MeshType& samples() const { return mMesh; }

    const PointType& sample(uint i) const { return mMesh.vertex(i).coord(); }

    std::size_t size() const { return mMesh.vertexNumber(); }

    void clear() { mMesh.clear(); }

    void resize(uint n)
    {
        if (n > mMesh.vertexNumber()) {
            uint k = n - mMesh.vertexNumber();
            mMesh.addVertices(k);
        }
    }

    void reserve(uint n) { mMesh.reserveVertices(n); }

    void add(const PointType& p) { mMesh.addVertex(p); }

    void set(uint i, const PointType& p) { mMesh.vertex(i).coord() = p; }

    template<VertexConcept VertexType>
    void add(const VertexType& v)
    {
        uint vi = mMesh.addVertex(v.coord());
        mMesh.vertex(vi).importFrom(v, false);

        setBirthElement(vi, "birthVertex", v.index());
    }

    template<VertexConcept VertexType>
    void set(uint i, const VertexType& v)
    {
        mMesh.vertex(i).coord() = v.coord();
        mMesh.vertex(i).importFrom(v, false);

        setBirthElement(i, "birthVertex", v.index());
    }

    template<EdgeConcept EdgeType>
    void add(const EdgeType& e, double u, bool copyQuality = true)
    {
        uint vi = mMesh.addVertex(
            (e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u));

        if constexpr (
            HasPerVertexQuality<MeshType> && edge::HasQuality<EdgeType>) {
            if (copyQuality) {
                if (isPerVertexQualityAvailable(mMesh) &&
                    comp::isQualityAvailableOn(e)) {
                    mMesh.vertex(vi).quality() = e.quality();
                }
            }
        }

        setBirthElement(vi, "birthEdge", e.index());
    }

    template<EdgeConcept EdgeType>
    void set(uint i, const EdgeType& e, double u, bool copyQuality = true)
    {
        mMesh.vertex(i).coord() =
            (e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u);

        if constexpr (
            HasPerVertexQuality<MeshType> && edge::HasQuality<EdgeType>) {
            if (copyQuality) {
                if (isPerVertexQualityAvailable(mMesh) &&
                    comp::isQualityAvailableOn(e)) {
                    mMesh.vertex(i).quality() = e.quality();
                }
            }
        }

        setBirthElement(i, "birthEdge", e.index());
    }

    template<FaceConcept FaceType>
    void add(
        const FaceType& f,
        bool            copyNormal  = false,
        bool            copyQuality = true)
    {
        uint vi = mMesh.addVertex(faceBarycenter(f));

        copyComponents(vi, f, copyNormal, copyQuality);
        setBirthElement(vi, "birthFace", f.index());
    }

    template<FaceConcept FaceType>
    void set(
        uint            i,
        const FaceType& f,
        bool            copyNormal  = false,
        bool            copyQuality = true)
    {
        mMesh.vertex(i).coord() = faceBarycenter(f);

        copyComponents(i, f, copyNormal, copyQuality);
        setBirthElement(i, "birthFace", f.index());
    }

    template<FaceConcept FaceType>
    void add(
        const FaceType&                f,
        const std::vector<ScalarType>& barCoords,
        bool                           copyNormal  = false,
        bool                           copyQuality = true)
    {
        assert(f.vertexNumber() <= barCoords.size());

        PointType p;
        for (uint i = 0; i < f.vertexNumber(); i++)
            p += f.vertex(i)->coord() * barCoords[i];

        uint vi = mMesh.addVertex(p);

        copyComponents(vi, f, copyNormal, copyQuality);
        setBirthElement(vi, "birthFace", f.index());
    }

    template<FaceConcept FaceType>
    void set(
        uint                           i,
        const FaceType&                f,
        const std::vector<ScalarType>& barCoords,
        bool                           copyNormal  = false,
        bool                           copyQuality = true)
    {
        assert(f.vertexNumber() <= barCoords.size());

        PointType p;
        for (uint i = 0; i < f.vertexNumber(); i++)
            p += f.vertex(i)->coord() * barCoords[i];

        mMesh.vertex(i).coord() = p;

        copyComponents(i, f, copyNormal, copyQuality);
        setBirthElement(i, "birthFace", f.index());
    }

    template<FaceConcept FaceType>
    void add(
        const FaceType&  f,
        const PointType& barCoords,
        bool             copyNormal  = false,
        bool             copyQuality = true)
    {
        static_assert(FaceType::NV == 3 || FaceType::NV == -1);
        if constexpr (FaceType::NV == -1) {
            assert(f.vertexNumber() == 3);
        }

        PointType p = triangleBarycentricCoordinatePoint(f, barCoords);

        uint vi = mMesh.addVertex(p);

        copyComponents(vi, f, copyNormal, copyQuality);
        setBirthElement(vi, "birthFace", f.index());
    }

    template<FaceConcept FaceType>
    void set(
        uint             i,
        const FaceType&  f,
        const PointType& barCoords,
        bool             copyNormal  = false,
        bool             copyQuality = true)
    {
        static_assert(FaceType::NV == 3 || FaceType::NV == -1);
        if constexpr (FaceType::NV == -1) {
            assert(f.vertexNumber() == 3);
        }

        PointType p = triangleBarycentricCoordinatePoint(f, barCoords);

        mMesh.vertex(i).coord() = p;

        copyComponents(i, f, copyNormal, copyQuality);
        setBirthElement(i, "birthFace", f.index());
    }

    ConstIterator begin() const
    {
        return std::ranges::begin(mMesh.vertices() | views::coords);
    }

    ConstIterator end() const
    {
        return std::ranges::end(mMesh.vertices() | views::coords);
    }

private:
    template<FaceConcept FaceType>
    void copyComponents(
        uint            vi,
        const FaceType& f,
        bool            copyNormal,
        bool            copyQuality)
    {
        if constexpr (
            HasPerVertexNormal<MeshType> && face::HasNormal<FaceType>) {
            if (copyNormal) {
                if (isPerVertexNormalAvailable(mMesh) &&
                    comp::isNormalAvailableOn(f)) {
                    mMesh.vertex(vi).normal() = f.normal();
                }
            }
        }

        if constexpr (
            HasPerVertexQuality<MeshType> && face::HasQuality<FaceType>) {
            if (copyQuality) {
                if (isPerVertexQualityAvailable(mMesh) &&
                    comp::isQualityAvailableOn(f)) {
                    mMesh.vertex(vi).quality() = f.quality();
                }
            }
        }
    }

    void setBirthElement(uint vi, const std::string& key, uint value)
    {
        if constexpr (HasPerVertexCustomComponents<MeshType>) {
            if (!mMesh.hasPerVertexCustomComponent(key)) {
                mMesh.template addPerVertexCustomComponent<uint>(key);
            }
            mMesh.vertex(vi).template customComponent<uint>(key) = value;
        }
    }
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_SAMPLER_MESH_SAMPLER_H
