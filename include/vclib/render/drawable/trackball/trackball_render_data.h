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

#ifndef VCL_RENDER_DRAWABLE_TRACKBALL_TRACKBALL_RENDER_DATA_H
#define VCL_RENDER_DRAWABLE_TRACKBALL_TRACKBALL_RENDER_DATA_H

#include <vclib/space/core/matrix.h>
#include <vclib/space/core/point.h>

namespace vcl {

class TrackballRenderData
{
    std::vector<vcl::Point3f> mVertices;
    std::vector<uint16_t>     mEdges;

    vcl::Matrix44f mTransform = vcl::Matrix44f::Identity();

public:
    TrackballRenderData(uint pointsPerCircle = 64);

    uint vertexNumber() const;

    uint edgeNumber() const;

    const float* vertexBufferData() const;

    const uint16_t* edgeBufferData() const;

    const float* transformData() const;

    void updateRotation(const vcl::Matrix44f& rot);
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_TRACKBALL_TRACKBALL_RENDER_DATA_H
