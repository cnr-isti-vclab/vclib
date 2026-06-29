// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/screenspace/primitives/screenspace_points.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/programs/vert_frag_loader.h>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.h>

namespace vcl {

/**
 * @brief Constructs a point set by referencing existing VertexBuffers.
 *
 * @param[in] vertexCount: Number of points.
 * @param[in] verts: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 2 components per point (x, y),
 * stored as consecutive floats: [x0, y0, x1, y1, ..., xn-1, yn-1]. The
 * buffer must remain valid for the lifetime of this object.
 * @param[in] vertColors: Optional VertexBuffer containing per-point
 * colors. Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must remain valid for the lifetime of this object.
 */
ScreenSpacePoints::ScreenSpacePoints(
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
 * Expected layout: an array of `float` with 2 components per point (x, y),
 * stored as consecutive floats: [x0, y0, x1, y1, ..., xn-1, yn-1]. The
 * buffer must remain valid for the lifetime of this object.
 */
void ScreenSpacePoints::setVertices(
    const uint          vertexCount,
    const VertexBuffer& verts)
{
    mVerPosCount = vertexCount;

    mVertexPositions.setReferenced(&verts);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Sets per-point colors by referencing an existing VertexBuffer.
 *
 * @param[in] vertColors: VertexBuffer containing per-point colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must remain valid for the lifetime of this object.
 */
void ScreenSpacePoints::setVertexColors(const VertexBuffer& vertColors)
{
    mVertexColors.setReferenced(&vertColors);
    mIsUpdateProgramNeeded = true;
}

/**
 * @brief Draws the point splats on the specified view.
 *
 * Renders all points as screen-space splats using programmable vertex
 * pulling. Each point is expanded into a quad procedurally in the vertex
 * shader.
 *
 * If the point set is empty/invalid, this method does nothing.
 *
 * @param[in] viewId: The bgfx view ID to submit the rendering commands to.
 */
void ScreenSpacePoints::draw(bgfx::ViewId viewId) const
{
    if (mVerPosCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    checkAndUpdateProgram();

    ScreenSpacePointsUniforms::setWidth(mWidth);
    ScreenSpacePointsUniforms::setGeneralColor(mGeneralColor);

    // Bind the points buffer as a compute buffer (SSBO) for vertex shader
    // access
    mVertexPositions.get().bindCompute(
        POINTS_POSITIONS_STAGE, bgfx::Access::Read);

    if (mVertexColors.isValid()) {
        mVertexColors.get().bindCompute(
            POINTS_COLORS_STAGE, bgfx::Access::Read);
    }

    // Set the number of vertices to generate procedurally
    // Each point generates 6 vertices (2 triangles)
    bgfx::setVertexCount(mVerPosCount * 6);

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

    ScreenSpacePointsUniforms::bind();
    bgfx::submit(viewId, mProgram);
}

/**
 * @brief Checks if the shader program needs to be updated and updates it.
 *
 * Validates that required buffers (colors) are available based on
 * the current color settings, throwing an exception if invalid.
 * Then it selects the appropriate shader program.
 */
void ScreenSpacePoints::checkAndUpdateProgram() const
{
    if (!mIsUpdateProgramNeeded) {
        return;
    }

    if (mColorSetting == ColorSetting::PER_VERTEX) {
        if (!mVertexColors.isValid()) {
            throw std::runtime_error(
                "ScreenSpacePoints: PER_VERTEX color setting requires a valid "
                "vertex color buffer.");
        }
    }

    mProgram               = screenspacePointsProgramSelector();
    mIsUpdateProgramNeeded = false;
}

/**
 * @brief Selects the correct bgfx shader program based on current settings.
 *
 * @return The appropriate bgfx::ProgramHandle for the current configuration.
 */
bgfx::ProgramHandle ScreenSpacePoints::screenspacePointsProgramSelector() const
{
    using enum VertFragProgram;

    Context&        ctx = Context::instance();
    ProgramManager& pm  = ctx.programManager();

    if (mColorSetting == ColorSetting::PER_VERTEX) {
        if (mShape == Shape::SQUARE) {
            return pm.getProgram<SCREENSPACE_POINTS_PVC_SQ>();
        }
        else {
            return pm.getProgram<SCREENSPACE_POINTS_PVC_CIR>();
        }
    }
    else {
        if (mShape == Shape::SQUARE) {
            return pm.getProgram<SCREENSPACE_POINTS_GC_SQ>();
        }
        else {
            return pm.getProgram<SCREENSPACE_POINTS_GC_CIR>();
        }
    }
}

} // namespace vcl
