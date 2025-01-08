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

#include <vclib/render/drawable/trackball/trackball_render_data.h>

#include <vclib/algorithms/core/polygon/create.h>
#include <vclib/math/transform.h>

namespace vcl {

TrackballRenderData::TrackballRenderData(uint pointsPerCircle)
{
    vcl::Polygon2f circle =
        vcl::createCircle<vcl::Polygon2f>(pointsPerCircle, 1.0f);

    mVertices.reserve(pointsPerCircle * 3);

    // x
    uint first = 0;
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        mVertices.push_back(vcl::Point3f(0, p.x(), p.y()));
        mEdges.push_back(i + first);
        mEdges.push_back((i + 1) % circle.size() + first);
    }

    // y
    first = circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        mVertices.push_back(vcl::Point3f(p.x(), 0, p.y()));
        mEdges.push_back(i + first);
        mEdges.push_back((i + 1) % circle.size() + first);
    }

    // z
    first = 2 * circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        mVertices.push_back(vcl::Point3f(p.x(), p.y(), 0));
        mEdges.push_back(i + first);
        mEdges.push_back((i + 1) % circle.size() + first);
    }
}

uint TrackballRenderData::vertexNumber() const
{
    return mVertices.size();
}

uint TrackballRenderData::edgeNumber() const
{
    return mEdges.size();
}

const float* TrackballRenderData::vertexBufferData() const
{
    return mVertices.front().data();
}

const uint16_t* TrackballRenderData::edgeBufferData() const
{
    return mEdges.data();
}

const float* TrackballRenderData::transformData() const
{
    return mTransform.data();
}

void TrackballRenderData::setTransform(const Matrix44f& mtx)
{
    mTransform = mtx;
}

} // namespace vcl
