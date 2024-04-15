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

#ifndef VCL_SPACE_SAMPLER_POINT_SAMPLER_H
#define VCL_SPACE_SAMPLER_POINT_SAMPLER_H

#include <vclib/algorithms/core/polygon.h>
#include <vclib/concepts/space/sampler.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<PointConcept PointT = vcl::Point3d>
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
        mSamples.push_back(v.coord());
    }

    template<VertexConcept VertexType>
    void set(uint i, const VertexType& v)
    {
        mSamples[i] = v.coord();
    }

    template<EdgeConcept EdgeType>
    void add(const EdgeType& e, double u = 0.5)
    {
        mSamples.push_back(
            (e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u));
    }

    template<EdgeConcept EdgeType>
    void set(uint i, const EdgeType& e, double u = 0.5)
    {
        mSamples[i] =
            (e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u);
    }

    template<FaceConcept FaceType>
    void add(const FaceType& f)
    {
        mSamples.push_back(vcl::faceBarycenter(f));
    }

    template<FaceConcept FaceType>
    void set(uint i, const FaceType& f)
    {
        mSamples[i] = vcl::faceBarycenter(f);
    }

    template<FaceConcept FaceType>
    void add(const FaceType& f, const std::vector<ScalarType>& barCoords)
    {
        assert(f.vertexNumber() <= barCoords.size());

        PointT p;
        for (uint i = 0; i < f.vertexNumber(); i++)
            p += f.vertex(i)->coord() * barCoords[i];

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
            p += f.vertex(i)->coord() * barCoords[i];

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

    ConstIterator begin() const { return mSamples.begin(); }

    ConstIterator end() const { return mSamples.end(); }
};

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_POINT_SAMPLER_H
