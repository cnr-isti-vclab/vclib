// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/screenspace/primitives/screenspace_lines.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/programs/vert_frag_loader.h>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_lines_uniforms.h>

namespace vcl {

ScreenSpaceLines::ScreenSpaceLines(
    const uint          vertexCount,
    const VertexBuffer& verts)
{
    setVertices(vertexCount, verts);
}

void ScreenSpaceLines::setVertices(uint vertexCount, const VertexBuffer& verts)
{
    mVerPosCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
    mIsUpdateProgramNeeded = true;
}

void ScreenSpaceLines::setIndices(uint indexCount, const IndexBuffer& indices)
{
    mIndexCount = indexCount;
    mIndices.setReferenced(&indices);
    mIsUpdateProgramNeeded = true;
}

void ScreenSpaceLines::setVertexColors(
    uint                vColsCount,
    const VertexBuffer& vertexColors)
{
    mVerColCount = vColsCount;
    mVertexColors.setReferenced(&vertexColors);
    mIsUpdateProgramNeeded = true;
}

void ScreenSpaceLines::setLineColors(
    uint               lColorCount,
    const IndexBuffer& lineColors)
{
    mLineColorCount = lColorCount;
    mLineColors.setReferenced(&lineColors);
    mIsUpdateProgramNeeded = true;
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

    checkAndUpdateProgram();

    ScreenSpaceLinesUniforms::setWidth(mWidth);
    ScreenSpaceLinesUniforms::setGeneralColor(mGeneralColor);

    // Bind the vertex buffer as a compute buffer (SSBO) for vertex shader
    // access (vertex pulling) - always needed
    mVertexPositions.get().bindCompute(V_POS_STAGE, bgfx::Access::Read);

    if (mVertexColors.isValid() && mColorSetting == ColorSetting::PER_VERTEX) {
        mVertexColors.get().bindCompute(V_COL_STAGE, bgfx::Access::Read);
    }
    if (mLineColors.isValid() && mColorSetting == ColorSetting::PER_LINE) {
        mLineColors.get().bind(L_COL_STAGE, bgfx::Access::Read);
    }
    if (mIndices.isValid()) {
        mIndices.get().bind(L_IND_STAGE, bgfx::Access::Read);
    }

    // Vertex Pulling: the vertex shader will generate the line geometry
    // number of vertex shader invocations is returned by
    // vertexPullingInstances()
    bgfx::setVertexCount(vertexPullingInstances());

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

    ScreenSpaceLinesUniforms::bind();
    bgfx::submit(viewId, mProgram);
}

/**
 * @brief Checks if the shader program needs to be updated and updates it.
 *
 * Validates that required buffers (colors) are available based on
 * the current topology and color settings, throwing an exception if invalid.
 * Then it selects the appropriate shader program.
 */
void ScreenSpaceLines::checkAndUpdateProgram() const
{
    if (!mIsUpdateProgramNeeded) {
        return;
    }

    uint        nv      = mIndices.isValid() ? mIndexCount : mVerPosCount;
    std::string primstr = mIndices.isValid() ? "indices" : "vertices";

    // Validate that buffer capacities match the expected topology rules.
    // E.g., LINES requires pairs, and LINE_STRIP requires at least 2 points.

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
        if (mTopology == Topology::LINES && mLineColorCount != nv / 2) {
            throw std::runtime_error(
                "ScreenSpaceLines: The number of line colors must match "
                "the number of lines (" +
                primstr + " / 2) for LINES topology.");
        }
        if (mTopology == Topology::LINE_STRIP && mLineColorCount != nv - 1) {
            throw std::runtime_error(
                "ScreenSpaceLines: The number of line colors must match "
                "the number of lines (" +
                primstr +
                " - 1) for LINE_STRIP "
                "topology.");
        }
    }

    mProgram               = screenspaceLinesProgramSelector();
    mIsUpdateProgramNeeded = false;
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

/**
 * @brief Selects the correct program based on topology, indexing, and color
 * setting.
 *
 * @param[in] pm: The program manager to retrieve program handles.
 * @return bgfx::ProgramHandle for the selected program.
 */
bgfx::ProgramHandle ScreenSpaceLines::screenspaceLinesProgramSelector() const
{
    using enum VertFragProgram;

    constexpr uint N_INDEX_MODES = 2;
    constexpr uint N_TOPO_MODES  = 2;
    constexpr uint N_COLOR_MODES = 3;

    constexpr uint N_PROGRAMS = N_INDEX_MODES * N_TOPO_MODES * N_COLOR_MODES;

    static const std::array<VertFragProgram, N_PROGRAMS> programs = {
        SCREENSPACE_LINES_INDICES_ON_TOPO_LINES_COLOR_PER_VERTEX,
        SCREENSPACE_LINES_INDICES_ON_TOPO_LINES_COLOR_PER_LINE,
        SCREENSPACE_LINES_INDICES_ON_TOPO_LINES_COLOR_GENERAL,
        SCREENSPACE_LINES_INDICES_ON_TOPO_LINE_STRIP_COLOR_PER_VERTEX,
        SCREENSPACE_LINES_INDICES_ON_TOPO_LINE_STRIP_COLOR_PER_LINE,
        SCREENSPACE_LINES_INDICES_ON_TOPO_LINE_STRIP_COLOR_GENERAL,
        SCREENSPACE_LINES_INDICES_OFF_TOPO_LINES_COLOR_PER_VERTEX,
        SCREENSPACE_LINES_INDICES_OFF_TOPO_LINES_COLOR_PER_LINE,
        SCREENSPACE_LINES_INDICES_OFF_TOPO_LINES_COLOR_GENERAL,
        SCREENSPACE_LINES_INDICES_OFF_TOPO_LINE_STRIP_COLOR_PER_VERTEX,
        SCREENSPACE_LINES_INDICES_OFF_TOPO_LINE_STRIP_COLOR_PER_LINE,
        SCREENSPACE_LINES_INDICES_OFF_TOPO_LINE_STRIP_COLOR_GENERAL};

    uint indexMode = mIndices.isValid() ? 0 : 1;
    uint topoMode  = (mTopology == Topology::LINES) ? 0 : 1;
    uint colorMode = 0;
    if (mColorSetting == ColorSetting::PER_LINE) {
        colorMode = 1;
    }
    else if (mColorSetting == ColorSetting::GENERAL) {
        colorMode = 2;
    }

    uint program = indexMode * (N_TOPO_MODES * N_COLOR_MODES) +
                   topoMode * N_COLOR_MODES + colorMode;

    ProgramManager& pm = Context::instance().programManager();
    return pm.getProgram(programs[program]);
}

} // namespace vcl
