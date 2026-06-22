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

#include <vclib/bgfx/screenspace/primitives/screenspace_points.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/programs/vert_frag_loader.h>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.h>

namespace vcl {

/**
 * @brief Constructs a point set by referencing existing VertexBuffers.
 *
 * @param[in] vertexCount: Number of points (length of verts).
 * @param[in] verts: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 2 components per point (x, y),
 * stored as consecutive floats: [x0, y0, x1, y1, ..., xn-1, yn-1]. The
 * buffer must be created for compute access and must remain valid for the
 * lifetime of this object.
 * @param[in] vertColors: Optional VertexBuffer containing per-point
 * colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must be created for compute access and must remain valid for the lifetime
 * of this object.
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
 * buffer must be created for compute access and must remain valid for the
 * lifetime of this object.
 */
void ScreenSpacePoints::setVertices(
    const uint          vertexCount,
    const VertexBuffer& verts)
{
    mVertexCount = vertexCount;

    mVertexPositions.setReferenced(&verts);
}

/**
 * @brief Sets per-point colors by referencing an existing VertexBuffer.
 *
 * @param[in] vertColors: VertexBuffer containing per-point colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must be created for compute access and must remain valid for the lifetime
 * of this object.
 */
void ScreenSpacePoints::setVertexColors(const VertexBuffer& vertColors)
{
    mVertexColors.setReferenced(&vertColors);
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
    if (mVertexCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    Context& ctx = Context::instance();

    ProgramManager& pm = ctx.programManager();

    ColorSetting colorToUse = mColorToUse;
    if (colorToUse == ColorSetting::PER_VERTEX && !mVertexColors.isValid()) {
        colorToUse = ColorSetting::GENERAL;
    }

    ScreenSpacePointsUniforms::setColorSetting(static_cast<uint>(colorToUse));
    ScreenSpacePointsUniforms::setShape(static_cast<uint>(mShape));
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
    bgfx::setVertexCount(mVertexCount * 6);

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

    ScreenSpacePointsUniforms::bind();
    bgfx::submit(viewId, pm.getProgram<VertFragProgram::SCREENSPACE_POINTS>());
}

} // namespace vcl
