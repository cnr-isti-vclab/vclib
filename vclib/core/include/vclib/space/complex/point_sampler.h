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

#ifndef VCL_SPACE_COMPLEX_POINT_SAMPLER_H
#define VCL_SPACE_COMPLEX_POINT_SAMPLER_H

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>

namespace vcl {

template<PointConcept PointT = Point3d>
class PointSampler
{
    std::vector<PointT> mSamples;

public:
    using PointType     = PointT;
    using ScalarType    = PointT::ScalarType;
    using ConstIterator = std::vector<PointT>::const_iterator;

    PointSampler() = default;

    const std::vector<PointT>& samples() const { return mSamples; }

    const PointT& sample(uint i) const { return mSamples[i]; }

    std::size_t size() const { return mSamples.size(); }

    void clear() { mSamples.clear(); }

    void reserve(uint n) { mSamples.reserve(n); }

    void resize(uint n) { mSamples.resize(n); }

    void add(const PointT& p) { mSamples.push_back(p); }

    void set(uint i, const PointT& p) { mSamples[i] = p; }

    template<VertexConcept VertexType>
    void add(const VertexType& v)
    {
        mSamples.push_back(v.position());
    }

    template<VertexConcept VertexType>
    void set(uint i, const VertexType& v)
    {
        mSamples[i] = v.position();
    }

    template<EdgeConcept EdgeType>
    void add(const EdgeType& e, double u = 0.5)
    {
        mSamples.push_back(
            (e.vertex(0).position() * (1 - u)) + (e.vertex(1).position() * u));
    }

    template<EdgeConcept EdgeType>
    void set(uint i, const EdgeType& e, double u = 0.5)
    {
        mSamples[i] =
            (e.vertex(0).position() * (1 - u)) + (e.vertex(1).position() * u);
    }

    template<FaceConcept FaceType>
    void add(const FaceType& f)
    {
        mSamples.push_back(faceBarycenter(f));
    }

    template<FaceConcept FaceType>
    void set(uint i, const FaceType& f)
    {
        mSamples[i] = faceBarycenter(f);
    }

    template<FaceConcept FaceType>
    void add(const FaceType& f, const std::vector<ScalarType>& barCoords)
    {
        assert(f.vertexNumber() <= barCoords.size());

        PointT p;
        for (uint i = 0; i < f.vertexNumber(); i++)
            p += f.vertex(i)->position() * barCoords[i];

        mSamples.push_back(p);
    }

    template<FaceConcept FaceType>
    void set(
        uint                           i,
        const FaceType&                f,
        const std::vector<ScalarType>& barCoords)
    {
        assert(f.vertexNumber() <= barCoords.size());

        PointT p;
        for (uint i = 0; i < f.vertexNumber(); i++)
            p += f.vertex(i)->position() * barCoords[i];

        mSamples[i] = p;
    }

    template<FaceConcept FaceType>
    void add(const FaceType& f, const PointT& barCoords)
    {
        static_assert(FaceType::NV == 3 || FaceType::NV == -1);
        if constexpr (FaceType::NV == -1) {
            assert(f.vertexNumber() == 3);
        }

        PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

        mSamples.push_back(p);
    }

    template<FaceConcept FaceType>
    void set(uint i, const FaceType& f, const PointT& barCoords)
    {
        static_assert(FaceType::NV == 3 || FaceType::NV == -1);
        if constexpr (FaceType::NV == -1) {
            assert(f.vertexNumber() == 3);
        }

        PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

        mSamples[i] = p;
    }

    template<MeshConcept MeshType>
    MeshType toMesh() const
    {
        MeshType mesh;
        mesh.reserveVertices(this->size());
        for (const auto& p : mSamples) {
            mesh.addVertex(p);
        }
        return mesh;
    }

    ConstIterator begin() const { return mSamples.begin(); }

    ConstIterator end() const { return mSamples.end(); }
};

/**
 * @brief A concept representing a generic PointSampler.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a PointSampler class having any Point type.
 *
 * @tparam T: The type to be tested for conformity to the PointSamplerConcept.
 *
 * @ingroup space_complex
 */
template<typename T>
concept PointSamplerConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    PointSampler<typename RemoveRef<T>::PointType>>;

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_POINT_SAMPLER_H
