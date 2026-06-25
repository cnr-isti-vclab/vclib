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

/**
 * @brief Constructs a line set by referencing existing VertexBuffers.
 *
 * @param[in] vertexCount: Number of vertices.
 * @param[in] verts: VertexBuffer containing vertex positions.
 * Expected layout: an array of `float` with 3 components per vertex (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1].
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
Lines::Lines(const uint vertexCount, const VertexBuffer& verts)
{
    setVertices(vertexCount, verts);
}

/**
 * @brief Sets vertex positions by referencing an existing VertexBuffer.
 *
 * @param[in] vertexCount: Number of vertices in the VertexBuffer.
 * @param[in] verts: VertexBuffer containing vertex positions.
 * Expected layout: an array of `float` with 3 components per vertex (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1].
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void Lines::setVertices(uint vertexCount, const VertexBuffer& verts)
{
    if (vertexCount != mVerPosCount)
        mIsUpdateProgramNeeded = true;
    mVerPosCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
}

/**
 * @brief Sets line indices by referencing an existing IndexBuffer.
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
void Lines::setIndices(uint indexCount, const IndexBuffer& indices)
{
    if (indexCount != mIndexCount)
        mIsUpdateProgramNeeded = true;
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
void Lines::setVertexColors(uint vColsCount, const VertexBuffer& vertexColors)
{
    if (vColsCount != mVerColCount)
        mIsUpdateProgramNeeded = true;
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
void Lines::setLineColors(uint lColorCount, const IndexBuffer& lineColors)
{
    if (lColorCount != mLineColorCount)
        mIsUpdateProgramNeeded = true;
    mLineColorCount = lColorCount;
    mLineColors.setReferenced(&lineColors);
}

/**
 * @brief Sets vertex normals by referencing an existing VertexBuffer.
 *
 * @param[in] vertexCount: Number of vertex normals in the VertexBuffer.
 * @param[in] verts: VertexBuffer containing vertex normals.
 * Expected layout: an array of `float` with 3 components per vertex (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1].
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
void Lines::setVertexNormals(uint vNorCount, const VertexBuffer& vertexNormals)
{
    if (vNorCount != mVerNorCount)
        mIsUpdateProgramNeeded = true;
    mVerNorCount = vNorCount;
    mVertexNormals.setReferenced(&vertexNormals);
}

/**
 * @brief Sets per-line normals by referencing an existing VertexBuffer.
 *
 * @param[in] lNorCount: Number of line normals in the VertexBuffer.
 * @param[in] verts: VertexBuffer containing per-line normals.
 * Expected layout: an array of `float` with 3 components per line (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1].
 *
 * @note The buffer must remain valid for the lifetime of this object.
 */
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
                "lines (" + primstr + " / 2) for LINES topology.");
        }
        if (mTopology == Topology::LINE_STRIP && mLineColorCount != nv - 1) {
            throw std::runtime_error(
                "Lines: The number of line colors must match the number of "
                "lines (" + primstr + " - 1) for LINE_STRIP topology.");
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

    auto& pm = Context::instance().programManager();

    if (mIndices.isValid()) {
        if (mTopology == Topology::LINES) {
            if (mColorSetting == ColorSetting::PER_VERTEX) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_PVC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_PVC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_PVC_PLS>();
            }
            else if (mColorSetting == ColorSetting::PER_LINE) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_PLC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_PLC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_PLC_PLS>();
            }
            else { // GC
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_GC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_GC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_LINES_GC_PLS>();
            }
        }
        else { // LINE_STRIP
            if (mColorSetting == ColorSetting::PER_VERTEX) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_PVC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_PVC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_PVC_PLS>();
            }
            else if (mColorSetting == ColorSetting::PER_LINE) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_PLC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_PLC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_PLC_PLS>();
            }
            else { // GC
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_GC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_GC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_IDX_STRIP_GC_PLS>();
            }
        }
    }
    else { // NOIDX
        if (mTopology == Topology::LINES) {
            if (mColorSetting == ColorSetting::PER_VERTEX) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_PVC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_PVC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_PVC_PLS>();
            }
            else if (mColorSetting == ColorSetting::PER_LINE) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_PLC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_PLC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_PLC_PLS>();
            }
            else { // GC
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_GC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_GC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_LINES_GC_PLS>();
            }
        }
        else { // LINE_STRIP
            if (mColorSetting == ColorSetting::PER_VERTEX) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_PVC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_PVC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_PVC_PLS>();
            }
            else if (mColorSetting == ColorSetting::PER_LINE) {
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_PLC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_PLC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_PLC_PLS>();
            }
            else { // GC
                if (mShading == Shading::NONE)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_GC_NS>();
                else if (mShading == Shading::PER_VERTEX)
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_GC_PVS>();
                else
                    mProgram = pm.getProgram<
                        VertFragProgram::PRIMITIVE_LINES_NOIDX_STRIP_GC_PLS>();
            }
        }
    }
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

} // namespace vcl
