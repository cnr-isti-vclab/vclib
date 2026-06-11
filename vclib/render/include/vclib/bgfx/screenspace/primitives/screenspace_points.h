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

namespace vcl {

class ScreenSpacePoints
{
    inline static const VertexBuffer NULL_VERTEX_BUFFER;

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
    uint mPointsCount = 0;

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

    template<Range RV, Range RC>
    requires Point2Concept<std::ranges::range_value_t<RV>> &&
             ColorConcept<std::ranges::range_value_t<RC>>
    ScreenSpacePoints(
        RV&& vertCoords,
        RC&& vertColors = std::vector<Color>())
    {
        setPoints(vertCoords);
        if (!vertColors.empty()) {
            setPointColors(vertColors);
        }
    }

    ScreenSpacePoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexColors = NULL_VERTEX_BUFFER);

    template<Range R>
    requires Point2Concept<std::ranges::range_value_t<R>>
    void setPoints(R&& vertCoords)
    {
        mPointsCount = std::ranges::size(vertCoords);

        VertexBuffer points;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(mPointsCount * 2);

        for (size_t i = 0; const auto& v : vertCoords) {
            buffer[i * 2 + 0] = v.x();
            buffer[i * 2 + 1] = v.y();
            ++i;
        }

        points.createForCompute(
            buffer,
            mPointsCount,
            bgfx::Attrib::Position,
            2,
            PrimitiveType::FLOAT,
            false,
            bgfx::Access::Read,
            releaseFn);
        mPoints.setOwned(std::move(points));

        setSplatsBuffers();
    }

    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setPointColors(R&& vertColors)
    {
        assert(std::ranges::size(vertColors) == mPointsCount);

        VertexBuffer pointColors;

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(mPointsCount);

        for (uint i = 0; const auto& c : vertColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        pointColors.createForCompute(
            buffer,
            mPointsCount,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            bgfx::Access::Read,
            releaseFn);
        mPointColors.setOwned(std::move(pointColors));
    }

    void setPoints(const uint pointsSize, const VertexBuffer& vertexCoords);

    void setPointColors(const VertexBuffer& vertexColors);

    void setWidthSetting(float width) { mWidth = width; }

    void setColorSetting(PointsColor colorToUse) { mColorToUse = colorToUse; }

    void setShapeSetting(PointsShape shape) { mShape = shape; }

    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    void draw(bgfx::ViewId viewId) const;

private:
    static constexpr uint POINTS_POSITIONS_STAGE = 0;
    static constexpr uint POINTS_COLORS_STAGE    = 1;
    static constexpr uint POINTS_OUTPUT_STAGE    = 2;

    static constexpr uint POINTS_SPLAT_INDEX_COUNT_PER_POINT = 6;
    static constexpr uint POINTS_SPLAT_VERTEX_COUNT_PER_POINT = 4;

    void setSplatsBuffers()
    {
        setPointSplatsBuffer(mPointSplats, mPointsCount);
        setPointSplatIndicesBuffer(mPointSplatIndices, mPointsCount);
    }

    static void setPointSplatsBuffer(vcl::VertexBuffer& splats, uint pointsSize);

    static void setPointSplatIndicesBuffer(vcl::IndexBuffer& indices, uint pointsSize);
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
