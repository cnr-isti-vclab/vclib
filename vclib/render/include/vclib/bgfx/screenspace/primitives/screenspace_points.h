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

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>

namespace vcl {

class ScreenSpacePoints
{
public:
    enum class PointsColor {
        PER_POINT, // Select color from point buffer color
        GENERAL,   // Select color from general color
    };

    enum class PointsShape {
        SQUARE, // square splats
        CIRCLE  // circular splats
    };

private:
    uint mPointsSize = 0;

    float       mWidth        = 1.0f;
    PointsColor mColorToUse   = PointsColor::GENERAL;
    PointsShape mShape        = PointsShape::SQUARE;
    Color       mGeneralColor = Color::Black;

    OwnedOrRefBuffer<VertexBuffer> mPoints;
    OwnedOrRefBuffer<VertexBuffer> mPointColors;

    VertexBuffer mPointSplats;
    IndexBuffer  mPointSplatIndices;

public:
    ScreenSpacePoints() = default;

    ScreenSpacePoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors = std::vector<uint>());

    ScreenSpacePoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexColors);

    void setPoints(
        const uint pointsSize,
        const float* vertCoords,
        const uint*  vertColors = nullptr);

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors = std::vector<uint>());

    void setPoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexColors);

    void setWidth(float width) { mWidth = width; }

    void setPointsColor(PointsColor colorToUse) { mColorToUse = colorToUse; }

    void setPointsShape(PointsShape shape) { mShape = shape; }

    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    void draw(bgfx::ViewId viewId) const;
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
