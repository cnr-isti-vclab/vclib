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

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_LINES_H
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_LINES_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_lines_uniforms.h>

namespace vcl {

/**
 * @brief Renders a set of 2D lines as screen-space line segments (1px width).
 *
 * Each line segment is rendered directly using bgfx's line primitive.
 * Lines are always indexed, and support both general color and per-vertex colors.
 */
class ScreenSpaceLines
{
    inline static const VertexBuffer NULL_VERTEX_BUFFER;

public:
    /**
     * @brief Specifies how line colors are determined during rendering.
     */
    enum class LinesColor {
        PER_VERTEX, ///< Each vertex uses color from per-vertex color buffer.
        GENERAL     ///< All vertices use a single general color.
    };

    /** @brief Specifies the topology used for rendering lines.
     *
     * - LINES: Lines are defined by consecutive pairs of vertices in the vertex
     *     buffer. (vertices 0-1 form line 1, vertices 2-3 form line 2, etc.)
     * - LINE_STRIP: Lines are defined by a strip of vertices, where each vertex
     *     after the first forms a line segment with the previous vertex.
     *     (vertices 0-1 form line 1, vertices 1-2 form line 2, etc.)
     *
     * The topology determines how the vertex data is interpreted to form lines.
     * The default is LINES.
     */
    enum class Topology {
        LINES,
        LINE_STRIP
    };

private:
    uint mLinesCount = 0;

    float       mWidth        = 1.0f;
    Topology    mTopology     = Topology::LINES;
    LinesColor  mColorToUse   = LinesColor::GENERAL;
    Color       mGeneralColor = Color::Black;

    OwnedOrRefBuffer<VertexBuffer> mLineCoords;
    OwnedOrRefBuffer<VertexBuffer> mVertexColors;

    OwnedOrRefBuffer<IndexBuffer> mIndices;

public:
    /**
     * @brief Default constructor — creates an empty line set with no lines.
     */
    ScreenSpaceLines() = default;

    /**
     * @brief Constructs a line set from ranges of 2D coordinates and per-vertex
     * colors.
     *
     * @param[in] vertCoords: Range of elements convertible to Point2 (must
     * provide x() and y()). Each pair of consecutive points defines one line
     * segment.
     * @param[in] vertColors: Optional range of Color elements. If non-empty,
     * per-vertex colors are enabled; the size must match vertCoords. Default is
     * empty (falls back to general color).
     *
     * @note This constructor creates an owned buffer copy of the input data.
     * The original ranges are not referenced after construction.
     */
    template<Range RV, Range RC>
    requires Point2Concept<std::ranges::range_value_t<RV>> &&
             ColorConcept<std::ranges::range_value_t<RC>>
    ScreenSpaceLines(RV&& vertCoords, RC&& vertColors = std::vector<Color>())
    {
        setVertices(vertCoords);
        if (!vertColors.empty()) {
            setVertexColors(vertColors);
        }
    }

    ScreenSpaceLines(
        const uint          lineCoordsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexColors = NULL_VERTEX_BUFFER);

    /**
     * @brief Sets line coordinates from a range of 2D points.
     *
     * @tparam R: Range whose value type satisfies Point2Concept (must provide
     * x() and y()).
     * @param[in] vertCoords: Range of 2D points. Each pair of consecutive
     * elements defines one line segment: (coords[0], coords[1]) is the first
     * line, (coords[2], coords[3]) is the second line, etc.
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires Point2Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& vertCoords)
    {
        mLinesCount = std::ranges::size(vertCoords) / 2;

        VertexBuffer lineCoords;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(std::ranges::size(vertCoords) * 2);

        size_t i = 0;
        for (const auto& v : vertCoords) {
            buffer[i * 2 + 0] = v.x();
            buffer[i * 2 + 1] = v.y();
            ++i;
        }

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .end();

        lineCoords.create(
            bgfx::makeRef(buffer, std::ranges::size(vertCoords) * 2 * sizeof(float), releaseFn),
            layout);
        mLineCoords.setOwned(std::move(lineCoords));
    }

    template<Range R>
    requires std::integral<std::ranges::range_value_t<R>>
    void setIndices(R&& indices)
    {
        assert(std::ranges::size(indices) == mLinesCount * 2);

        IndexBuffer indexBuffer;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(std::ranges::size(indices));

        size_t i = 0;
        for (const auto& idx : indices) {
            buffer[i] = idx;
            ++i;
        }

        indexBuffer.create(
            bgfx::makeRef(buffer, std::ranges::size(indices) * sizeof(uint), releaseFn),
            BGFX_BUFFER_INDEX32);

        mIndices.setOwned(std::move(indexBuffer));
    }

    /**
     * @brief Sets per-vertex colors from a range of Color elements.
     *
     * @tparam R: Range whose value type satisfies ColorConcept.
     * @param[in] vertColors Range of Color objects. Must have exactly
     * the lineCoordsSize() size (one color per vertex). Each color is converted to ABGR
     * format (uint).
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        assert(std::ranges::size(vertColors) == mLinesCount * 2);

        VertexBuffer vertexColors;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(std::ranges::size(vertColors));

        uint i = 0;
        for (const auto& c : vertColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        vertexColors.create(
            bgfx::makeRef(buffer, std::ranges::size(vertColors) * sizeof(uint), releaseFn),
            layout);
        mVertexColors.setOwned(std::move(vertexColors));
    }

    void setVertices(const uint vertexCount, const VertexBuffer& vertexCoords);

    void setVertexColors(const VertexBuffer& vertexColors);

    /**
     * @brief Sets the width of line segments (in screen-space pixels).
     * @param[in] width: The line width value.
     */
    void setWidthSetting(float width) { mWidth = width; }

    /**
     * @brief Sets the topology used for rendering lines.
     * @param[in] topo: The desired line topology (LINES or LINE_STRIP).
     */
    void setTopology(Topology topo) { mTopology = topo; }

    /**
     * @brief Sets the color mode for line rendering.
     * @param[in] colorToUse: Whether to use per-vertex colors or a general
     * uniform color.
     */
    void setColorSetting(LinesColor colorToUse) { mColorToUse = colorToUse; }

    /**
     * @brief Sets the general (uniform) color used when color mode is GENERAL.
     * @param[in] generalColor: The fallback color for all lines.
     */
    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    void draw(bgfx::ViewId viewId) const;

private:
    static constexpr uint LINES_COORDS_STAGE = 0;
    static constexpr uint LINES_COLORS_STAGE = 1;
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_LINES_H
