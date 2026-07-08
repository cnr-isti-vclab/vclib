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

#include <vclib/bgfx/primitives/lines.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/primitives/uniforms/lines_uniforms.h>
#include <vclib/bgfx/programs/vert_frag_loader.h>

#include <stdexcept>

namespace vcl {

Lines::Lines(const uint vertexCount, const VertexBuffer& verts)
{
    setVertices(vertexCount, verts);
}

void Lines::setVertices(uint vertexCount, const VertexBuffer& verts)
{
    if (vertexCount != mVerPosCount)
        mIsUpdateProgramNeeded = true;
    mVerPosCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
}

void Lines::setIndices(uint indexCount, const IndexBuffer& indices)
{
    if (indexCount != mIndexCount)
        mIsUpdateProgramNeeded = true;
    mIndexCount = indexCount;
    mIndices.setReferenced(&indices);
}

void Lines::setVertexColors(uint vColsCount, const VertexBuffer& vertexColors)
{
    if (vColsCount != mVerColCount)
        mIsUpdateProgramNeeded = true;
    mVerColCount = vColsCount;
    mVertexColors.setReferenced(&vertexColors);
}

void Lines::setLineColors(uint lColorCount, const IndexBuffer& lineColors)
{
    if (lColorCount != mLineColorCount)
        mIsUpdateProgramNeeded = true;
    mLineColorCount = lColorCount;
    mLineColors.setReferenced(&lineColors);
}

void Lines::setVertexNormals(uint vNorCount, const VertexBuffer& vertexNormals)
{
    if (vNorCount != mVerNorCount)
        mIsUpdateProgramNeeded = true;
    mVerNorCount = vNorCount;
    mVertexNormals.setReferenced(&vertexNormals);
}

void Lines::setLineNormals(uint lNorCount, const VertexBuffer& lineNormals)
{
    if (lNorCount != mLineNorCount)
        mIsUpdateProgramNeeded = true;
    mLineNorCount = lNorCount;
    mLineNormals.setReferenced(&lineNormals);
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
void Lines::draw(bgfx::ViewId viewId) const
{
    if (mVerPosCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    checkAndUpdateProgram();

    LinesUniforms::setWidth(mWidth);
    LinesUniforms::setGeneralColor(mGeneralColor);
    LinesUniforms::setDepthOffset(mDepthOffset);

    // Bind buffers for compute / vertex pulling
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
    if (mVertexNormals.isValid() && mShading == Shading::PER_VERTEX) {
        mVertexNormals.get().bindCompute(V_NOR_STAGE, bgfx::Access::Read);
    }
    if (mLineNormals.isValid() && mShading == Shading::PER_LINE) {
        mLineNormals.get().bindCompute(L_NOR_STAGE, bgfx::Access::Read);
    }

    bgfx::setVertexCount(vertexPullingInstances());
    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_BLEND_ALPHA);
    LinesUniforms::bind();
    bgfx::submit(viewId, mProgram);
}

void Lines::checkAndUpdateProgram() const
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
            "Lines: For LINES topology, the number of " + primstr +
            " must be even (each line requires 2 endpoints).");
    }
    if (mTopology == Topology::LINE_STRIP && nv < 2) {
        throw std::runtime_error(
            "Lines: For LINE_STRIP topology, at least 2 " + primstr +
            " are required to form a line.");
    }

    if (mColorSetting == ColorSetting::PER_VERTEX) {
        if (!mVertexColors.isValid()) {
            throw std::runtime_error(
                "Lines: PER_VERTEX color setting requires a valid vertex color "
                "buffer.");
        }
        if (mVerColCount != mVerPosCount) {
            throw std::runtime_error(
                "Lines: The number of vertex colors must match the number of "
                "vertices.");
        }
    }
    else if (mColorSetting == ColorSetting::PER_LINE) {
        if (!mLineColors.isValid()) {
            throw std::runtime_error(
                "Lines: PER_LINE color setting requires a valid line color "
                "buffer.");
        }
        if (mTopology == Topology::LINES && mLineColorCount != nv / 2) {
            throw std::runtime_error(
                "Lines: The number of line colors must match the number of "
                "lines (" +
                primstr + " / 2) for LINES topology.");
        }
        if (mTopology == Topology::LINE_STRIP && mLineColorCount != nv - 1) {
            throw std::runtime_error(
                "Lines: The number of line colors must match the number of "
                "lines (" +
                primstr + " - 1) for LINE_STRIP topology.");
        }
    }

    if (mShading == Shading::PER_VERTEX) {
        if (!mVertexNormals.isValid()) {
            throw std::runtime_error(
                "Lines: PER_VERTEX shading setting requires a valid vertex "
                "normal buffer.");
        }
        if (mVerNorCount != mVerPosCount) {
            throw std::runtime_error(
                "Lines: The number of vertex normals must match the number of "
                "vertices.");
        }
    }
    else if (mShading == Shading::PER_LINE) {
        if (!mLineNormals.isValid()) {
            throw std::runtime_error(
                "Lines: PER_LINE shading setting requires a valid line normal "
                "buffer.");
        }
        if (mTopology == Topology::LINES && mLineNorCount != nv / 2) {
            throw std::runtime_error(
                "Lines: The number of line normals must match the number of "
                "lines (" +
                primstr + " / 2) for LINES topology.");
        }
        if (mTopology == Topology::LINE_STRIP && mLineNorCount != nv - 1) {
            throw std::runtime_error(
                "Lines: The number of line normals must match the number of "
                "lines (" +
                primstr + " - 1) for LINE_STRIP topology.");
        }
    }

    mProgram               = linesProgramSelector();
    mIsUpdateProgramNeeded = false;
}

uint Lines::vertexPullingInstances() const
{
    uint nVPI = 0;
    uint nv   = mIndices.isValid() ? mIndexCount : mVerPosCount;

    if (mTopology == Topology::LINES) {
        // each line generates 6 vertices (2 triangles) in the shader
        nVPI = (nv / 2) * 6; // Which is nv * 3
    }
    else if (mTopology == Topology::LINE_STRIP) {
        // each vertex after the first forms a line
        nVPI = (nv - 1) * 6;
    }

    return nVPI;
}

bgfx::ProgramHandle Lines::linesProgramSelector() const
{
    using enum VertFragProgram;

    constexpr uint N_INDEX_MODES   = 2;
    constexpr uint N_TOPO_MODES    = 2;
    constexpr uint N_COLOR_MODES   = 3;

    uint shading  = toUnderlying(mShading);
    uint indices  = mIndices.isValid() ? 0 : 1;
    uint topology = toUnderlying(mTopology);
    uint color    = toUnderlying(mColorSetting);

    // the first shader of all the combinations
    uint base = toUnderlying(
        PRIMITIVE_LINES_SHADING_NONE_INDICES_ON_TOPO_LINES_COLOR_PER_VERTEX);

    // matrix is generated from lines.config:
    // SHADING x INDICES x TOPOLOGY x COLOR

    uint program = base +
                   shading * N_INDEX_MODES * N_TOPO_MODES * N_COLOR_MODES +
                   indices * N_TOPO_MODES * N_COLOR_MODES +
                   topology * N_COLOR_MODES + color;

    ProgramManager& pm = Context::instance().programManager();
    return pm.getProgram(static_cast<VertFragProgram>(program));
}

} // namespace vcl
