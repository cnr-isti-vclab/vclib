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
#include <vclib/bgfx/programs/compute_loader.h>
#include <vclib/bgfx/programs/vert_frag_loader.h>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.h>

namespace vcl {

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

void ScreenSpacePoints::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords)
{
    mPointsCount = pointsSize;

    mPoints.setReferenced(&vertexCoords);

    setSplatsBuffers();
}

void ScreenSpacePoints::setPointColors(const VertexBuffer& vertexColors)
{
    mPointColors.setReferenced(&vertexColors);
}

void ScreenSpacePoints::draw(bgfx::ViewId viewId) const
{
    if (mPointsCount == 0 || !mPoints.isValid() || !mPointSplats.isValid() ||
        !mPointSplatIndices.isValid()) {
        return;
    }

    Context& ctx = Context::instance();
    if (!ctx.supportsCompute()) {
        return;
    }

    ProgramManager& pm = ctx.programManager();

    PointsColor colorToUse = mColorToUse;
    if (colorToUse == PointsColor::PER_POINT && !mPointColors.isValid()) {
        colorToUse = PointsColor::GENERAL;
    }

    ScreenSpacePointsUniforms::setPointsColor(static_cast<uint>(colorToUse));
    ScreenSpacePointsUniforms::setPointsShape(static_cast<uint>(mShape));
    ScreenSpacePointsUniforms::setPointsWidth(mWidth);
    ScreenSpacePointsUniforms::setPointsGeneralColor(mGeneralColor);

    mPoints.get().bindCompute(POINTS_POSITIONS_STAGE, bgfx::Access::Read);
    mPointSplats.bindCompute(POINTS_OUTPUT_STAGE, bgfx::Access::Write);

    ScreenSpacePointsUniforms::bind();
    bgfx::dispatch(
        viewId,
        pm.getComputeProgram<ComputeProgram::SCREENSPACE_POINTS>(),
        mPointsCount,
        1,
        1);

    mPointSplats.bindVertex(0);
    mPointSplatIndices.bind();

    if (mPointColors.isValid()) {
        mPointColors.get().bindCompute(POINTS_COLORS_STAGE, bgfx::Access::Read);
    }

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

    ScreenSpacePointsUniforms::bind();
    bgfx::submit(viewId, pm.getProgram<VertFragProgram::SCREENSPACE_POINTS>());
}

void ScreenSpacePoints::setPointSplatsBuffer(
    VertexBuffer& splats,
    uint          pointsSize)
{
    if (pointsSize == 0) {
        splats.destroy();
        return;
    }

    const uint splatVertCount =
        pointsSize * POINTS_SPLAT_VERTEX_COUNT_PER_POINT;

    auto [buffer, releaseFn] =
        vcl::Context::getAllocatedBufferAndReleaseFn<float>(splatVertCount * 4);

    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
        .end();

    splats.create(
        bgfx::makeRef(buffer, splatVertCount * 4 * sizeof(float), releaseFn),
        layout,
        BGFX_BUFFER_COMPUTE_WRITE);
}

void ScreenSpacePoints::setPointSplatIndicesBuffer(
    IndexBuffer& indices,
    uint         pointsSize)
{
    const uint indexCount = pointsSize * POINTS_SPLAT_INDEX_COUNT_PER_POINT;

    auto [buffer, releaseFn] =
        vcl::Context::getAllocatedBufferAndReleaseFn<uint>(indexCount);

    for (uint i = 0; i < pointsSize; ++i) {
        const uint v = i * POINTS_SPLAT_VERTEX_COUNT_PER_POINT;
        const uint k = i * POINTS_SPLAT_INDEX_COUNT_PER_POINT;

        buffer[k + 0] = v + 0;
        buffer[k + 1] = v + 1;
        buffer[k + 2] = v + 2;
        buffer[k + 3] = v + 2;
        buffer[k + 4] = v + 1;
        buffer[k + 5] = v + 3;
    }

    indices.create(buffer, indexCount, true, releaseFn);
}

} // namespace vcl
