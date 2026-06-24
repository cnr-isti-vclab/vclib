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

#include <vclib/bgfx/primitives/points.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/primitives/uniforms/points_uniforms.h>

#include <stdexcept>

namespace vcl {

/**
 * @brief Constructs a point set by referencing existing VertexBuffers.
 *
 * @param[in] vertexCount: Number of points (length of verts).
 * @param[in] verts: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 3 components per point (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1]. The buffer must be created for compute access and must
 * remain valid for the lifetime of this object.
 * @param[in] vertColors: Optional VertexBuffer containing per-point colors.
 * Expected layout: an array of `uint` with 4 channels per color in ABGR
 * order (A, B, G, R packed as a single 32-bit integer). The buffer must be
 * created for compute access and must remain valid for the lifetime of
 * this object.
 */
Points::Points(
    const uint          vertexCount,
    const VertexBuffer& verts,
    const VertexBuffer& vertColors)
{
    setVertices(vertexCount, verts);
    if (vertColors.isValid()) {
        setVertexColors(vertColors);
    }
}

/**
 * @brief Sets point positions by referencing an existing VertexBuffer.
 *
 * @param[in] vertexCount: Number of points (length of verts).
 * @param[in] verts: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 3 components per point (x, y,
 * z), stored as consecutive floats: [x0, y0, z0, x1, y1, z1, ..., xn-1,
 * yn-1, zn-1]. The buffer must be created for compute access and must
 * remain valid for the lifetime of this object.
 */
void Points::setVertices(const uint vertexCount, const VertexBuffer& verts)
{
    mVertexCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Sets per-point normals by referencing an existing VertexBuffer.
 *
 * @param[in] vertNormals: VertexBuffer containing per-point normals.
 * Expected layout: an array of `float` with 3 components per normal (x, y,
 * z), stored as consecutive floats: [nx0, ny0, nz0, nx1, ny1, nz1, ...,
 * nxn-1, nyn-1, nzn-1]. The buffer must be created for compute access and
 * must remain valid for the lifetime of this object.
 */
void Points::setVertexNormals(const VertexBuffer& vertNormals)
{
    mVertexNormals.setReferenced(&vertNormals);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Sets per-point colors by referencing an existing VertexBuffer.
 *
 * @param[in] vertColors: VertexBuffer containing per-point colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must be created for compute access and must remain valid for the
 * lifetime of this object.
 */
void Points::setVertexColors(const VertexBuffer& vertColors)
{
    mVertexColors.setReferenced(&vertColors);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Draws the point splats in world space on the specified view.
 *
 * Renders all 3D points as screen-space splats using programmable vertex
 * pulling. Points are positioned through the standard camera projection,
 * and each point is expanded into a quad procedurally depending on the
 * configured Shape setting.
 *
 * @param[in] viewId: The bgfx view ID to submit the rendering commands to.
 */
void Points::draw(bgfx::ViewId viewId) const
{
    // Skip rendering if there are no vertices or the position buffer is invalid
    if (mVertexCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    checkAndUpdateProgram();

    // Upload rendering settings to the shader via uniform.
    PointsUniforms::setWidth(mWidth);
    PointsUniforms::setGeneralColor(mGeneralColor);

    // Bind the position buffer as a compute buffer (SSBO) for vertex shader
    // access. The point positions are read by the vertex pulling mechanism.
    mVertexPositions.get().bindCompute(
        POINTS_POSITIONS_STAGE, bgfx::Access::Read);

    if (mVertexNormals.isValid()) {
        mVertexNormals.get().bindCompute(
            POINTS_NORMALS_STAGE, bgfx::Access::Read);
    }

    if (mVertexColors.isValid()) {
        mVertexColors.get().bindCompute(
            POINTS_COLORS_STAGE, bgfx::Access::Read);
    }

    bgfx::setVertexCount(mVertexCount * 6);

    bgfx::setState(0 |
                   BGFX_STATE_WRITE_RGB        |
                   BGFX_STATE_WRITE_A          |
                   BGFX_STATE_WRITE_Z          |
                   BGFX_STATE_DEPTH_TEST_LESS  |
                   BGFX_STATE_BLEND_ALPHA);

    // Bind the updated uniforms to the shader stage.
    PointsUniforms::bind();

    bgfx::submit(viewId, mProgram);
}

void Points::checkAndUpdateProgram() const
{
    if (!mIsUpdateProgramNeeded) {
        return;
    }

    if (mColorToUse == ColorSetting::PER_VERTEX) {
        if (!mVertexColors.isValid()) {
            throw std::runtime_error(
                "Points: PER_VERTEX color setting requires a "
                "valid vertex color buffer.");
        }
    }
    if (mShading == Shading::PER_VERTEX) {
        if (!mVertexNormals.isValid()) {
            throw std::runtime_error(
                "Points: PER_VERTEX shading setting requires a "
                "valid vertex normal buffer.");
        }
    }

    mProgram = pointsProgramSelector();
    mIsUpdateProgramNeeded = false;
}

bgfx::ProgramHandle Points::pointsProgramSelector() const
{
    using enum VertFragProgram;

    Context& ctx = Context::instance();
    ProgramManager& pm = ctx.programManager();

    if (mColorToUse == ColorSetting::PER_VERTEX) {
        if (mShading == Shading::NONE) {
            if (mShape == Shape::SQUARE) {
                return pm.getProgram<PRIMITIVE_POINTS_PVC_NS_SQ>();
            } else {
                return pm.getProgram<PRIMITIVE_POINTS_PVC_NS_CIR>();
            }
        } else {
            if (mShape == Shape::SQUARE) {
                return pm.getProgram<PRIMITIVE_POINTS_PVC_PVS_SQ>();
            } else {
                return pm.getProgram<PRIMITIVE_POINTS_PVC_PVS_CIR>();
            }
        }
    } else {
        if (mShading == Shading::NONE) {
            if (mShape == Shape::SQUARE) {
                return pm.getProgram<PRIMITIVE_POINTS_GC_NS_SQ>();
            } else {
                return pm.getProgram<PRIMITIVE_POINTS_GC_NS_CIR>();
            }
        } else {
            if (mShape == Shape::SQUARE) {
                return pm.getProgram<PRIMITIVE_POINTS_GC_PVS_SQ>();
            } else {
                return pm.getProgram<PRIMITIVE_POINTS_GC_PVS_CIR>();
            }
        }
    }
}

} // namespace vcl
