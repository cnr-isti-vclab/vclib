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
 * @param[in] lineCoordsSize: Number of coordinate pairs (each line uses 2
 * points).
 * @param[in] vertexCoords: VertexBuffer containing line coordinates.
 * Expected layout: an array of `float` with 2 components per point (x, y),
 * stored as consecutive floats: [x0, y0, x1, y1, ..., xn-1, yn-1]. The
 * buffer must remain valid for the lifetime of this object.
 * @param[in] vertexColors: Optional VertexBuffer containing per-vertex colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must remain valid for the lifetime of this object.
 */
ScreenSpaceLines::ScreenSpaceLines(
    const uint          lineCoordsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexColors)
{
    setVertices(lineCoordsSize, vertexCoords);
    if (vertexColors.isValid()) {
        setVertexColors(vertexColors);
    }
}

/**
 * @brief Sets vertex positions by referencing an existing VertexBuffer.
 *
 * @param[in] vertexCount: Number of coordinate pairs (each line uses 2
 * points).
 * @param[in] vertexCoords: VertexBuffer containing vertex positions.
 * Expected layout: an array of `float` with 2 components per point (x, y),
 * stored as consecutive floats: [x0, y0, x1, y1, ..., xn-1, yn-1]. The
 * buffer must remain valid for the lifetime of this object.
 */
void ScreenSpaceLines::setVertices(
    const uint          vertexCount,
    const VertexBuffer& vertexCoords)
{
    mLinesCount = vertexCount;
    mLineCoords.setReferenced(&vertexCoords);
}

/**
 * @brief Sets per-vertex colors by referencing an existing VertexBuffer.
 *
 * @param[in] vertexColors: VertexBuffer containing per-vertex colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must remain valid for the lifetime of this object.
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
    if (mLinesCount == 0 || !mLineCoords.isValid()) {
        return;
    }

    Context& ctx = Context::instance();

    LinesColor colorToUse = mColorToUse;
    if (colorToUse == LinesColor::PER_VERTEX && !mVertexColors.isValid()) {
        colorToUse = LinesColor::GENERAL;
    }

    // Bind indices for line primitives
    if (mIndices.isValid())
        mIndices.get().bind();

    // Set uniforms BEFORE binding vertex attributes (shader uses uniform to decide color)
    ScreenSpaceLinesUniforms::setLinesUsePerVertexColor(
        colorToUse == LinesColor::PER_VERTEX ? 1 : 0);
    ScreenSpaceLinesUniforms::setLinesGeneralColor(mGeneralColor);
    ScreenSpaceLinesUniforms::bind();

    // Bind vertex coordinates
    mLineCoords.get().bind(0);

    // Bind colors if using per-vertex colors
    if (mVertexColors.isValid()) {
        mVertexColors.get().bind(1);
    }

    // Set up render state for lines with alpha blending (DEPTH_TEST_ALWAYS like ScreenSpaceBox)
    uint64_t state = 0 | BGFX_STATE_WRITE_RGB |
                     BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA;

    // Submit to bgfx with line primitive type
    ProgramManager& pm = ctx.programManager();
    state |= mTopology == Topology::LINES ? BGFX_STATE_PT_LINES :
                                            BGFX_STATE_PT_LINESTRIP;
    bgfx::setState(state);
    bgfx::submit(
        viewId,
        pm.getProgram<VertFragProgram::SCREENSPACE_LINES>());
}

} // namespace vcl
