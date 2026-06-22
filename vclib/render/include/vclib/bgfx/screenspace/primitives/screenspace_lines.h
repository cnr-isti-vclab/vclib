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

namespace vcl {

/**
 * @brief Renders a set of 2D lines as screen-space line segments (1px width).
 *
 * Each line segment is rendered directly using bgfx's line primitive.
 *
 * Lines are rendered using either consecutive vertex pairs or a strip topology.
 * Optional indices can be provided to define the vertex order; if omitted,
 * vertices are used in declaration order.
 */
class ScreenSpaceLines
{
public:
    /**
     * @brief Specifies how line colors are determined during rendering.
     */
    enum class ColorSetting {
        PER_VERTEX = 0, ///< Each vertex uses color from per-vertex color buffer
        PER_LINE   = 1, ///< Each line uses color from per-line color buffer
        GENERAL    = 2  ///< All vertices use a single general color
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
    enum class Topology { LINES = 0, LINE_STRIP = 1 };

private:
    uint mVerPosCount    = 0;
    uint mIndexCount     = 0;
    uint mVerColCount    = 0;
    uint mLineColorCount = 0;

    float        mWidth        = 1.0f;
    Topology     mTopology     = Topology::LINES;
    ColorSetting mColorSetting = ColorSetting::GENERAL;
    Color        mGeneralColor = Color::Black;

    OwnedOrRefBuffer<VertexBuffer> mVertexPositions;
    OwnedOrRefBuffer<VertexBuffer> mVertexColors;

    OwnedOrRefBuffer<IndexBuffer> mIndices;
    OwnedOrRefBuffer<IndexBuffer> mLineColors;

public:
    /**
     * @brief Default constructor — creates an empty line set with no lines.
     */
    ScreenSpaceLines() = default;

    /**
     * @brief Constructs a line set from ranges of 2D coordinates and per-vertex
     * colors.
     *
     * @param[in] verts: Range of elements convertible to Point2 (must
     * provide x() and y()).
     *
     * @note This constructor creates an owned buffer copy of the input data.
     * The original ranges are not referenced after construction.
     */
    template<Range RV>
    requires Point2Concept<std::ranges::range_value_t<RV>>
    ScreenSpaceLines(RV&& verts)
    {
        setVertices(verts);
    }

    ScreenSpaceLines(const uint vertexCount, const VertexBuffer& verts);

    /**
     * @brief Sets line coordinates from a range of 2D points.
     *
     * If no indices are set, the interpretation depends on the topology:
     * - For LINES topology, each pair of vertices defines one line segment.
     * - For LINE_STRIP topology, each vertex after the first forms a line
     *   segment with the previous vertex.
     *
     * @tparam R: Range whose value type satisfies Point2Concept (must provide
     * x() and y()).
     * @param[in] verts: Range of 2D points.
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires Point2Concept<std::ranges::range_value_t<R>>
    void setVertices(R&& verts)
    {
        mVerPosCount = std::ranges::size(verts);

        // Move to the nearest multiple of 2 to ensure we have complete line
        // pairs
        uint nv = mVerPosCount + (mVerPosCount % 2);

        VertexBuffer vertBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 2);

        size_t i = 0;
        for (const auto& v : verts) {
            buffer[i * 2 + 0] = v.x();
            buffer[i * 2 + 1] = v.y();
            ++i;
        }

        // Use createForCompute to enable SSBO binding for vertex pulling
        vertBuff.createForCompute(
            buffer,
            nv,
            bgfx::Attrib::Position,
            2,
            PrimitiveType::FLOAT,
            false,
            bgfx::Access::Read,
            releaseFn);
        mVertexPositions.setOwned(std::move(vertBuff));
    }

    /**
     * @brief Sets line indices from a range of unsigned integers.
     *
     * @tparam R: Range whose value type is an integral type (e.g., uint).
     * @param[in] indices: Range of indices defining the vertex order for lines.
     * The interpretation of indices depends on the topology:
     * - For LINES topology, each pair of indices defines one line segment.
     * - For LINE_STRIP topology, each index after the first forms a line
     *   segment with the previous index.
     *
     * @note This creates an owned buffer copy. The original range is not
     * referenced.
     */
    template<Range R>
    requires std::integral<std::ranges::range_value_t<R>>
    void setIndices(R&& indices)
    {
        mIndexCount = std::ranges::size(indices);

        IndexBuffer indexBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(mIndexCount);

        size_t i = 0;
        for (const auto& idx : indices) {
            buffer[i] = idx;
            ++i;
        }

        indexBuff.create(buffer, mIndexCount, releaseFn);

        mIndices.setOwned(std::move(indexBuff));
    }

    /**
     * @brief Sets per-vertex colors from a range of Color elements.
     *
     * @tparam R: Range whose value type satisfies ColorConcept.
     * @param[in] vertColors Range of Color objects. Must have exactly
     * the lineCoordsSize() size (one color per vertex). Each color is converted
     * to ABGR format (uint).
     */
    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setVertexColors(R&& vertColors)
    {
        mVerColCount = std::ranges::size(vertColors);

        VertexBuffer vColsBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(
                std::ranges::size(vertColors));

        uint i = 0;
        for (const auto& c : vertColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        // Use createForCompute to enable SSBO binding for vertex pulling
        vColsBuff.createForCompute(
            buffer,
            mVerPosCount,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            bgfx::Access::Read,
            releaseFn);
        mVertexColors.setOwned(std::move(vColsBuff));
    }

    template<Range R>
    requires ColorConcept<std::ranges::range_value_t<R>>
    void setLineColors(R&& linColors)
    {
        mLineColorCount = std::ranges::size(linColors);

        IndexBuffer lColsBuff;
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(
                std::ranges::size(linColors));

        uint i = 0;
        for (const auto& c : linColors) {
            buffer[i] = c.abgr();
            ++i;
        }

        lColsBuff.create(buffer, mLineColorCount, releaseFn);
        mLineColors.setOwned(std::move(lColsBuff));
    }

    void setVertices(uint vertexCount, const VertexBuffer& verts);

    void setIndices(uint indexCount, const IndexBuffer& indices);

    void setVertexColors(uint vColsCount, const VertexBuffer& vertexColors);

    void setLineColors(uint lColorCount, const IndexBuffer& lineColors);

    /**
     * @brief Sets the width of line segments (in screen-space pixels).
     * @param[in] width: The line width value.
     */
    void setWidth(float width) { mWidth = width; }

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
    void setColorSetting(ColorSetting colorToUse)
    {
        mColorSetting = colorToUse;
    }

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
    static constexpr uint V_POS_STAGE = 0;
    static constexpr uint V_COL_STAGE = 1;

    static constexpr uint L_IND_STAGE = 2;
    static constexpr uint L_COL_STAGE = 3;

    void validityCheck() const;

    uint vertexPullingInstances() const;

    bgfx::ProgramHandle screenspaceLinesProgramSelector() const;
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_LINES_H
