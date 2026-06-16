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

#include <vclib/bgfx/screenspace/primitives/screenspace_lines.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/programs/vert_frag_loader.h>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_lines_uniforms.h>

namespace vcl {

/**
 * @brief Constructs a line set by referencing existing VertexBuffers.
 *
 * @param[in] vertexCount: Number of coordinate pairs (each line uses 2
 * points).
 * @param[in] verts: VertexBuffer containing 2D vertex positions.
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
ScreenSpaceLines::ScreenSpaceLines(
    const uint          vertexCount,
    const VertexBuffer& verts)
{
    setVertices(vertexCount, verts);
}

/**
 * @brief Sets vertex positions by referencing an existing VertexBuffer.
 *
 * If no indices are set, the interpretation depends on the topology:
 * - For LINES topology, each pair of vertices defines one line segment.
 * - For LINE_STRIP topology, each vertex after the first forms a line
 *   segment with the previous vertex.
 *
 * @param[in] vertexCount: Number of coordinate pairs (each line uses 2
 * points).
 * @param[in] verts: VertexBuffer containing vertex positions.
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void ScreenSpaceLines::setVertices(
    const uint          vertexCount,
    const VertexBuffer& verts)
{
    mVertexCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
}

/**
 * @brief Sets line indices from a range of unsigned integers.
 *
 * @param[in] indices: IndexBuffer to use for lines.
 * The interpretation of indices depends on the topology:
 * - For LINES topology, each pair of indices defines one line segment.
 * - For LINE_STRIP topology, each index after the first forms a line
 *   segment with the previous index.
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void ScreenSpaceLines::setIndices(const IndexBuffer& indices)
{
    mIndices.setReferenced(&indices);
}

/**
 * @brief Sets per-vertex colors by referencing an existing VertexBuffer.
 *
 * @param[in] vertexColors: VertexBuffer containing per-vertex colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer).
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void ScreenSpaceLines::setVertexColors(const VertexBuffer& vertexColors)
{
    mVertexColors.setReferenced(&vertexColors);
}

/**
 * @brief Draws the line segments on the specified view.
 *
 * Renders all lines as screen-space 1px-width line segments using bgfx's
 * native line primitive rendering with alpha blending.
 *
 * If the line set is empty/invalid, this method does nothing.
 *
 * @param[in] viewId: The bgfx view ID to submit the rendering commands to.
 */
void ScreenSpaceLines::draw(bgfx::ViewId viewId) const
{
    if (mVertexCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    Context& ctx = Context::instance();

    ColorSetting colorToUse = mColorToUse;
    if (colorToUse == ColorSetting::PER_VERTEX && !mVertexColors.isValid()) {
        colorToUse = ColorSetting::GENERAL;
    }

    ScreenSpaceLinesUniforms::setColorSetting(
        colorToUse == ColorSetting::PER_VERTEX ? 1 : 0);
    ScreenSpaceLinesUniforms::setGeneralColor(mGeneralColor);
    ScreenSpaceLinesUniforms::bind();

    // Bind vertex coordinates
    mVertexPositions.get().bind(0);

    // Bind colors if using per-vertex colors
    if (mVertexColors.isValid()) {
        mVertexColors.get().bind(1);
    }

    // Bind indices for line primitives
    if (mIndices.isValid())
        mIndices.get().bind();

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_ALWAYS |
                     BGFX_STATE_BLEND_ALPHA;

    ProgramManager& pm = ctx.programManager();
    state |= mTopology == Topology::LINES ? BGFX_STATE_PT_LINES :
                                            BGFX_STATE_PT_LINESTRIP;
    bgfx::setState(state);
    bgfx::submit(viewId, pm.getProgram<VertFragProgram::SCREENSPACE_LINES>());
}

} // namespace vcl
