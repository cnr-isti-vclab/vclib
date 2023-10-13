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

#ifndef VCL_SPACE_SAMPLER_MESH_SAMPLER_H
#define VCL_SPACE_SAMPLER_MESH_SAMPLER_H

#include <vclib/algorithms/polygon.h>
#include <vclib/concepts/mesh/elements/edge.h>
#include <vclib/concepts/space/sampler.h>
#include <vclib/mesh/requirements.h>
#include <vclib/views/mesh.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshSampler
{
    using CoordView =
        decltype(vcl::View<typename MeshType::VertexIterator>() | views::coords);

    MeshType m;

public:
    using PointType     = MeshType::VertexType::CoordType;
    using ScalarType    = PointType::ScalarType;
    using ConstIterator = std::ranges::iterator_t<CoordView>;

    MeshSampler()
    {
        vcl::enableIfPerVertexNormalOptional(m);
        vcl::enableIfPerVertexQualityOptional(m);
        if constexpr (vcl::HasName<MeshType>) {
            m.name() = "Sampling";
        }
    }

    const MeshType& samples() const { return m; }

    const PointType& sample(uint i) const { return m.vertex(i).coord(); }

    std::size_t size() const { return m.vertexNumber(); }

    void clear() { m.clear(); }

    void resize(uint n)
    {
        if (n > m.vertexNumber()) {
            uint k = n - m.vertexNumber();
            m.addVertices(k);
        }
    }

    void reserve(uint n) { m.reserveVertices(n); }

    void add(const PointType& p) { m.addVertex(p); }

    void set(uint i, const PointType& p) { m.vertex(i).coord() = p; }

    template<VertexConcept VertexType>
    void add(const VertexType& v)
    {
        uint vi = m.addVertex(v.coord());
        m.vertex(vi).importFrom(v);

        setBirthElement(vi, "birthVertex", v.index());
    }

    template<VertexConcept VertexType>
    void set(uint i, const VertexType& v)
    {
        m.vertex(i).coord() = v.coord();
        m.vertex(i).importFrom(v);

        setBirthElement(i, "birthVertex", v.index());
    }

    template<EdgeConcept EdgeType>
    void add(const EdgeType& e, double u, bool copyQuality = true)
    {
        uint vi = m.addVertex(
            (e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u));

        if constexpr (
            vcl::HasPerVertexQuality<MeshType> &&
            vcl::edge::HasQuality<EdgeType>)
        {
            if (copyQuality) {
                if (vcl::isPerVertexQualityAvailable(m) &&
                    comp::isQualityAvailableOn(e))
                {
                    m.vertex(vi).quality() = e.quality();
                }
            }
        }

        setBirthElement(vi, "birthEdge", e.index());
    }

    template<EdgeConcept EdgeType>
    void set(uint i, const EdgeType& e, double u, bool copyQuality = true)
    {
        m.vertex(i).coord() =
            (e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u);

        if constexpr (
            vcl::HasPerVertexQuality<MeshType> &&
            vcl::edge::HasQuality<EdgeType>)
        {
            if (copyQuality) {
                if (vcl::isPerVertexQualityAvailable(m) &&
                    comp::isQualityAvailableOn(e))
                {
                    m.vertex(i).quality() = e.quality();
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
        uint vi = m.addVertex(vcl::faceBarycenter(f));

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
        m.vertex(i).coord() = vcl::faceBarycenter(f);

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

        uint vi = m.addVertex(p);

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

        m.vertex(i).coord() = p;

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

        uint vi = m.addVertex(p);

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

        m.vertex(i).coord() = p;

        copyComponents(i, f, copyNormal, copyQuality);
        setBirthElement(i, "birthFace", f.index());
    }

    ConstIterator begin() const
    {
        return std::ranges::begin(m.vertices() | views::coords);
    }

    ConstIterator end() const
    {
        return std::ranges::end(m.vertices() | views::coords);
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
            vcl::HasPerVertexNormal<MeshType> && vcl::face::HasNormal<FaceType>)
        {
            if (copyNormal) {
                if (vcl::isPerVertexNormalAvailable(m) &&
                    comp::isNormalAvailableOn(f))
                {
                    m.vertex(vi).normal() = f.normal();
                }
            }
        }

        if constexpr (
            vcl::HasPerVertexQuality<MeshType> &&
            vcl::face::HasQuality<FaceType>)
        {
            if (copyQuality) {
                if (vcl::isPerVertexQualityAvailable(m) &&
                    comp::isQualityAvailableOn(f))
                {
                    m.vertex(vi).quality() = f.quality();
                }
            }
        }
    }

    void setBirthElement(uint vi, const std::string& key, uint value)
    {
        if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
            if (!m.hasPerVertexCustomComponent(key)) {
                m.template addPerVertexCustomComponent<uint>(key);
            }
            m.vertex(vi).template customComponent<uint>(key) = value;
        }
    }
};

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_MESH_SAMPLER_H
