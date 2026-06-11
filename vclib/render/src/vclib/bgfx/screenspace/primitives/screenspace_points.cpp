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

#include <vclib/bgfx/screenspace/primitives/screenspace_points.h>

#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.h>

namespace vcl {

ScreenSpacePoints::ScreenSpacePoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors)
{
    // TODO
}

ScreenSpacePoints::ScreenSpacePoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexColors)
{
    // TODO
}

void ScreenSpacePoints::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors)
{
    // TODO
}

void ScreenSpacePoints::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexColors)
{
    // TODO
}

void ScreenSpacePoints::draw(bgfx::ViewId viewId) const
{
    // TODO
}

} // namespace vcl
