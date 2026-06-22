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

namespace vcl {

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

void Points::setVertices(const uint vertexCount, const VertexBuffer& verts)
{
    mVertexCount = vertexCount;
    mVertexPositions.setReferenced(&verts);
}

void Points::setVertexColors(const VertexBuffer& vertColors)
{
    mVertexColors.setReferenced(&vertColors);
}

void Points::draw(bgfx::ViewId viewId) const
{
    // Skip rendering if there are no vertices or the position buffer is invalid
    if (mVertexCount == 0 || !mVertexPositions.isValid()) {
        return;
    }

    Context& ctx = Context::instance();
    ProgramManager& pm = ctx.programManager();

    // Upload rendering settings to the shader via uniform.
    PointsUniforms::setColorSetting(static_cast<uint>(mColorToUse));
    PointsUniforms::setShape(static_cast<uint>(mShape));
    PointsUniforms::setSize(mSize);
    PointsUniforms::setGeneralColor(mGeneralColor);

    // Bind the position buffer as a compute buffer (SSBO) for vertex shader
    // access. The point positions are read by the vertex pulling mechanism.
    mVertexPositions.get().bindCompute(
        POINTS_POSITIONS_STAGE, bgfx::Access::Read);

    if (mVertexColors.isValid()) {
        // TODO: bind the per-vertex color buffer as a compute buffer at the
        // colors stage so the shader can read it.
        mVertexColors.get().bindCompute(
            POINTS_COLORS_STAGE, bgfx::Access::Read);
    }

    // Each point is expanded into one quad (6 vertices / 2 triangles).
    // TODO: verify this triangle count matches the intended shader behavior;
    // adjust if a single-vertex point sprite (e.g., GL_POINTS) is preferred.
    bgfx::setVertexCount(mVertexCount * 6);

    // TODO: configure appropriate rasterization state for world-space points.
    // Suggested settings (adjust according to the final shader design):
    //   - Enable depth testing with BGFX_STATE_DEPTH_TEST_LESS so closer points
    //     occlude farther ones correctly.
    //   - Write RGB and alpha channels.
    //   - Optionally enable MSAA-compatible blending: BGFX_STATE_BLEND_ALPHA.
    bgfx::setState(0 |
                   BGFX_STATE_WRITE_RGB        |
                   BGFX_STATE_WRITE_A          |
                   BGFX_STATE_DEPTH_TEST_LESS  |
                   BGFX_STATE_BLEND_ALPHA);

    // Bind the updated uniforms to the shader stage.
    PointsUniforms::bind();

    // TODO: select and submit with the appropriate world-space points program.
    // Replace the placeholder below with the real program, e.g.:
    //   bgfx::submit(viewId, pm.getProgram<VertFragProgram::POINTS>());
    //bgfx::submit(viewId, nullptr);

    // TODO: unbind compute buffers after draw (if not handled automatically
    // by setComputeTexture or the framework).
}

} // namespace vcl
