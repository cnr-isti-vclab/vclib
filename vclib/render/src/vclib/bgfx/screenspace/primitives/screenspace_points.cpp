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
 * @param[in] pointsSize: Number of points (length of vertexCoords).
 * @param[in] vertexCoords: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 2 components per point (x, y),
 * stored as consecutive floats: [x0, y0, x1, y1, ..., xn-1, yn-1]. The
 * buffer must be created for compute access and must remain valid for the
 * lifetime of this object.
 * @param[in] vertexColors: Optional VertexBuffer containing per-point
 * colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must be created for compute access and must remain valid for the lifetime
 * of this object.
 */
ScreenSpacePoints::ScreenSpacePoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexColors)
{
    setPoints(pointsSize, vertexCoords);
    if (vertexColors.isValid()) {
        setPointColors(vertexColors);
    }
}

/**
 * @brief Sets point positions by referencing an existing VertexBuffer.
 *
 * @param[in] pointsSize: Number of points (length of vertexCoords).
 * @param[in] vertexCoords: VertexBuffer containing point positions.
 * Expected layout: an array of `float` with 2 components per point (x, y),
 * stored as consecutive floats: [x0, y0, x1, y1, ..., xn-1, yn-1]. The
 * buffer must be created for compute access and must remain valid for the
 * lifetime of this object.
 */
void ScreenSpacePoints::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords)
{
    mPointsCount = pointsSize;

    mPoints.setReferenced(&vertexCoords);
}

/**
 * @brief Sets per-point colors by referencing an existing VertexBuffer.
 *
 * @param[in] vertexColors: VertexBuffer containing per-point colors.
 * Expected layout: an array of `uint` with 4 channels per color in
 * ABGR order (A, B, G, R packed as a single 32-bit integer). The buffer
 * must be created for compute access and must remain valid for the lifetime
 * of this object.
 */
void ScreenSpacePoints::setPointColors(const VertexBuffer& vertexColors)
{
    mPointColors.setReferenced(&vertexColors);
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
    if (mPointsCount == 0 || !mPoints.isValid()) {
        return;
    }

    Context& ctx = Context::instance();

    ProgramManager& pm = ctx.programManager();

    PointsColor colorToUse = mColorToUse;
    if (colorToUse == PointsColor::PER_POINT && !mPointColors.isValid()) {
        colorToUse = PointsColor::GENERAL;
    }

    ScreenSpacePointsUniforms::setPointsColor(static_cast<uint>(colorToUse));
    ScreenSpacePointsUniforms::setPointsShape(static_cast<uint>(mShape));
    ScreenSpacePointsUniforms::setPointsWidth(mWidth);
    ScreenSpacePointsUniforms::setPointsGeneralColor(mGeneralColor);

    // Bind the points buffer as a compute buffer (SSBO) for vertex shader
    // access
    mPoints.get().bindCompute(POINTS_POSITIONS_STAGE, bgfx::Access::Read);

    if (mPointColors.isValid()) {
        mPointColors.get().bindCompute(POINTS_COLORS_STAGE, bgfx::Access::Read);
    }

    // Set the number of vertices to generate procedurally
    // Each point generates 6 vertices (2 triangles)
    bgfx::setVertexCount(mPointsCount * 6);

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

    ScreenSpacePointsUniforms::bind();
    bgfx::submit(viewId, pm.getProgram<VertFragProgram::SCREENSPACE_POINTS>());
}

} // namespace vcl
