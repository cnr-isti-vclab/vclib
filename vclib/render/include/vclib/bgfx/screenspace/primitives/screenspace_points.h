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

/**
 * @brief Renders a set of 2D points as screen-space splats (squares or
 * circles).
 *
 * Each point is expanded into a quad procedurally in the vertex shader
 * using programmable vertex pulling.
 */
class ScreenSpacePoints
{
    inline static const VertexBuffer NULL_VERTEX_BUFFER;

public:
    /**
     * @brief Specifies how point colors are determined during rendering.
     */
    enum class ColorSetting {
        PER_VERTEX, ///< Each point uses color from per-vertex color buffer.
        GENERAL     ///< All points use a single general color.
    };

    /**
     * @brief Specifies the visual shape of each point splat.
     */
    enum class Shape {
        SQUARE, ///< Square splats (axis-aligned quads).
        CIRCLE  ///< Circular splats (disk-shaped).
    };

private:
    uint mVertexCount = 0;

    float        mWidth        = 1.0f;
    ColorSetting mColorToUse   = ColorSetting::GENERAL;
    Shape        mShape        = Shape::SQUARE;
    Color        mGeneralColor = Color::Black;

    OwnedOrRefBuffer<VertexBuffer> mVertexPositions;
    OwnedOrRefBuffer<VertexBuffer> mVertexColors;

public:
    /**
     * @brief Default constructor — creates an empty point set with no points.
     */
    ScreenSpacePoints() = default;

    /**
     * @brief Constructs a point set from ranges of 2D coordinates and per-point
     * colors.
     *
     * @param[in] verts: Range of elements convertible to Point2 (must
     * provide x() and y()). Each element contributes one screen-space point at
     * (x, y).
     * @param[in] vertColors: Optional range of Color elements. If non-empty,
     * per-point colors are enabled; the size must match vertCoords. Default is
     * empty (falls back to general color).
     *
     * @note This constructor creates an owned buffer copy of the input data.
     * The original ranges are not referenced after construction.
     */
    template<Range RV, Range RC>
    requires Point2Concept<std::ranges::range_value_t<RV>> &&
             ColorConcept<std::ranges::range_value_t<RC>>
    ScreenSpacePoints(RV&& verts, RC&& vertColors = std::vector<Color>())
    {
        setVertices(verts);
        if (!vertColors.empty()) {
            setVertexColors(vertColors);
        }
    }

    ScreenSpacePoints(
        const uint          vertexCount,
        const VertexBuffer& verts,
        const VertexBuffer& vertColors = NULL_VERTEX_BUFFER);

    /**
     * @brief Sets point positions from a range of 2D points.
     *
     * @tparam R: Range whose value type satisfies Point2Concept (must provide
     * x() and y()).
     * @param[in] verts: Range of 2D points. Each element is read as a
     * screen-space coordinate (x, y). The size determines the number of
     * rendered points.
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires Point2Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& verts)
    {
        mVertexCount = std::ranges::size(verts);

        // move to the nearest multiple of 2 to ensure padding of 4 floats
        uint nv = mVertexCount + (mVertexCount % 2);

        VertexBuffer vertBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 2);

        for (size_t i = 0; const auto& v : verts) {
            buffer[i * 2 + 0] = v.x();
            buffer[i * 2 + 1] = v.y();
            ++i;
        }

        vertBuff.create(
            buffer,
            nv,
            bgfx::Attrib::Position,
            2,
            PrimitiveType::FLOAT,
            releaseFn);
        mVertexPositions.setOwned(std::move(vertBuff));
    }

    /**
     * @brief Sets per-point colors from a range of Color elements.
     *
     * @tparam R: Range whose value type satisfies ColorConcept.
     * @param[in] vertColors Range of Color objects. Must have exactly
     * the pointsCount() size. Each color is converted to ABGR format (uint).
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        assert(std::ranges::size(vertColors) == mVertexCount);

        VertexBuffer vColsBuff;

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(mVertexCount);

        for (uint i = 0; const auto& c : vertColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        vColsBuff.create(
            buffer,
            mVertexCount,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            releaseFn);
        mVertexColors.setOwned(std::move(vColsBuff));
    }

    void setVertices(const uint vertexCount, const VertexBuffer& verts);

    void setVertexColors(const VertexBuffer& vertColors);

    /**
     * @brief Sets the width of point splats (in screen-space pixels or
     * normalized units).
     * @param[in] width: The splat width value.
     */
    void setWidth(float width) { mWidth = width; }

    /**
     * @brief Sets the color mode for point rendering.
     * @param[in] colorToUse: Whether to use per-point colors or a general
     * uniform color.
     */
    void setColorSetting(ColorSetting colorToUse) { mColorToUse = colorToUse; }

    /**
     * @brief Sets the visual shape of each point splat.
     * @param[in] shape: The splat shape (SQUARE or CIRCLE).
     */
    void setShapeSetting(Shape shape) { mShape = shape; }

    /**
     * @brief Sets the general (uniform) color used when color mode is GENERAL.
     * @param[in] generalColor: The fallback color for all points.
     */
    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    void draw(bgfx::ViewId viewId) const;

private:
    static constexpr uint POINTS_POSITIONS_STAGE = 0;
    static constexpr uint POINTS_COLORS_STAGE    = 1;
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
