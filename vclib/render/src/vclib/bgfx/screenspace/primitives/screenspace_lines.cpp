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
void ScreenSpaceLines::setVertices(uint vertexCount, const VertexBuffer& verts)
{
    mVerPosCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
}

/**
 * @brief Sets line indices from a range of unsigned integers.
 *
 * @param[in] indexCount: Number of indices in the IndexBuffer.
 * @param[in] indices: IndexBuffer to use for lines.
 * The interpretation of indices depends on the topology:
 * - For LINES topology, each pair of indices defines one line segment.
 * - For LINE_STRIP topology, each index after the first forms a line
 *   segment with the previous index.
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void ScreenSpaceLines::setIndices(uint indexCount, const IndexBuffer& indices)
{
    mIndexCount = indexCount;
    mIndices.setReferenced(&indices);
}

/**
 * @brief Sets per-vertex colors by referencing an existing VertexBuffer.
 *
 * @param[in] vColsCount: Number of vertex colors in the VertexBuffer.
 * @param[in] vertexColors: VertexBuffer containing per-vertex colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer).
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void ScreenSpaceLines::setVertexColors(
    uint                vColsCount,
    const VertexBuffer& vertexColors)
{
    mVerColCount = vColsCount;
    mVertexColors.setReferenced(&vertexColors);
}

/**
 * @brief Sets per-line colors by referencing an existing IndexBuffer.
 *
 * @param[in] lColorCount: Number of line colors in the IndexBuffer.
 * @param[in] lineColors: IndexBuffer containing per-line colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer).
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void ScreenSpaceLines::setLineColors(
    uint               lColorCount,
    const IndexBuffer& lineColors)
{
    mLineColorCount = lColorCount;
    mLineColors.setReferenced(&lineColors);
}

/**
 * @brief Draws the line segments on the specified view.
 *
 * Renders all lines as screen-space line segments with variable width using
 * vertex pulling. Each line (pair of endpoints) is expanded into 2 triangles
 * (6 vertices) procedurally in the vertex shader.
 *
 * If the line set is empty/invalid, this method does nothing.
 *
 * @param[in] viewId: The bgfx view ID to submit the rendering commands to.
 */
void ScreenSpaceLines::draw(bgfx::ViewId viewId) const
{
    if (mVerPosCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    validityCheck();

    Context& ctx = Context::instance();

    ProgramManager& pm = ctx.programManager();

    ScreenSpaceLinesUniforms::setWidth(mWidth);
    ScreenSpaceLinesUniforms::setTopology(vcl::toUnderlying(mTopology));
    ScreenSpaceLinesUniforms::setIndexed(mIndices.isValid());
    ScreenSpaceLinesUniforms::setColorSetting(vcl::toUnderlying(mColorSetting));
    ScreenSpaceLinesUniforms::setGeneralColor(mGeneralColor);

    // Bind the vertex buffer as a compute buffer (SSBO) for vertex shader
    // access (vertex pulling)
    mVertexPositions.get().bindCompute(V_POS_STAGE, bgfx::Access::Read);

    if (mIndices.isValid()) {
        mIndices.get().bind(L_IND_STAGE, bgfx::Access::Read);
    }

    if (mVertexColors.isValid()) {
        mVertexColors.get().bindCompute(V_COL_STAGE, bgfx::Access::Read);
    }

    if (mLineColors.isValid()) {
        mLineColors.get().bind(L_COL_STAGE, bgfx::Access::Read);
    }

    // Vertex Pulling: the vertex shader will generate the line geometry
    // number of vertex shader invocations is returned by
    // vertexPullingInstances()
    bgfx::setVertexCount(vertexPullingInstances());

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

    ScreenSpaceLinesUniforms::bind();
    bgfx::submit(viewId, pm.getProgram<VertFragProgram::SCREENSPACE_LINES>());
}

void ScreenSpaceLines::validityCheck() const
{
    uint nv = mIndices.isValid() ? mIndexCount : mVerPosCount;
    std::string primstr = mIndices.isValid() ? "indices" : "vertices";

    if (mTopology == Topology::LINES && nv % 2 != 0) {
        throw std::runtime_error(
            "ScreenSpaceLines: For LINES topology, the number of " + primstr +
            "must be even (each line requires 2 endpoints).");
    }
    if (mTopology == Topology::LINE_STRIP && nv < 2) {
        throw std::runtime_error(
            "ScreenSpaceLines: For LINE_STRIP topology, at least 2 " + primstr +
            " are required to form a line.");
    }
    if (mColorSetting == ColorSetting::PER_VERTEX) {
        if (!mVertexColors.isValid()) {
            throw std::runtime_error(
                "ScreenSpaceLines: PER_VERTEX color setting requires a "
                "valid vertex color buffer.");
        }
        if (mVerColCount != mVerPosCount) {
            throw std::runtime_error(
                "ScreenSpaceLines: The number of vertex colors must match "
                "the number of vertices.");
        }
    }
    if (mColorSetting == ColorSetting::PER_LINE) {
        if (!mLineColors.isValid()) {
            throw std::runtime_error(
                "ScreenSpaceLines: PER_LINE color setting requires a valid "
                "line color buffer.");
        }
        if (mTopology == Topology::LINES &&
            mLineColorCount != nv / 2) {
            throw std::runtime_error(
                "ScreenSpaceLines: The number of line colors must match "
                "the number of lines (" + primstr+ " / 2) for LINES topology.");
        }
        if (mTopology == Topology::LINE_STRIP &&
            mLineColorCount != nv - 1) {
            throw std::runtime_error(
                "ScreenSpaceLines: The number of line colors must match "
                "the number of lines (" + primstr+ " - 1) for LINE_STRIP "
                "topology.");
        }
    }
}

uint ScreenSpaceLines::vertexPullingInstances() const
{
    uint nVPI = 0;

    uint nv = mIndices.isValid() ? mIndexCount : mVerPosCount;

    if (mTopology == Topology::LINES) {
        // the buffer contains 2 vertices per line
        // each line generates 6 vertices (2 triangles) in the shader
        // n = (nv / 2) * 6 = nv * 3
        nVPI = nv * 3;
    }
    else if (mTopology == Topology::LINE_STRIP) {
        // the buffer contains nv vertices forming a line strip
        // each vertex after the first forms a line with the previous vertex
        // so we have (nv - 1) lines, each generating 6 verts (2 triangles)
        // n = (nv - 1) * 6
        nVPI = (nv - 1) * 6;
    }

    return nVPI;
}

} // namespace vcl
