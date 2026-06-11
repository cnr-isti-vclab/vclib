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

#include <vclib/bgfx/programs/embedded_c_programs/screenspace_points.h>
#include <vclib/bgfx/programs/embedded_vf_programs/screenspace_points.h>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.h>

#include <algorithm>
#include <cassert>

namespace {

constexpr uint POINTS_POSITIONS_STAGE = 0;
constexpr uint POINTS_COLORS_STAGE    = 1;
constexpr uint POINTS_OUTPUT_STAGE    = 2;

constexpr uint POINTS_SPLAT_INDEX_COUNT_PER_POINT = 6;
constexpr uint POINTS_SPLAT_VERTEX_COUNT_PER_POINT = 4;

void setPointSplatsBuffer(vcl::VertexBuffer& splats, uint pointsSize)
{
    if (pointsSize == 0) {
        splats.destroy();
        return;
    }

    const uint splatVertCount = pointsSize * POINTS_SPLAT_VERTEX_COUNT_PER_POINT;

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

void setPointSplatIndicesBuffer(vcl::IndexBuffer& indices, uint pointsSize)
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

} // namespace

namespace vcl {

ScreenSpacePoints::ScreenSpacePoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors)
{
    setPoints(vertCoords, vertColors);
}

ScreenSpacePoints::ScreenSpacePoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexColors)
{
    setPoints(pointsSize, vertexCoords, vertexColors);
}

void ScreenSpacePoints::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors)
{
    assert(vertCoords.size() % 2 == 0);

    mPointsSize = vertCoords.size() / 2;

    VertexBuffer points;
    {
        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(mPointsSize * 4);

        for (uint i = 0; i < mPointsSize; ++i) {
            buffer[i * 4 + 0] = vertCoords[i * 2 + 0];
            buffer[i * 4 + 1] = vertCoords[i * 2 + 1];
            buffer[i * 4 + 2] = 0.0f;
            buffer[i * 4 + 3] = 0.0f;
        }

        points.createForCompute(
            buffer,
            mPointsSize,
            bgfx::Attrib::Position,
            4,
            PrimitiveType::FLOAT,
            false,
            bgfx::Access::Read,
            releaseFn);
    }
    mPoints.setOwned(std::move(points));

    if (!vertColors.empty()) {
        assert(vertColors.size() == mPointsSize);

        VertexBuffer pointColors;
        {
            auto [buffer, releaseFn] =
                Context::getAllocatedBufferAndReleaseFn<uint>(mPointsSize);

            std::copy(vertColors.begin(), vertColors.end(), buffer);

            pointColors.createForCompute(
                buffer,
                mPointsSize,
                bgfx::Attrib::Color0,
                4,
                PrimitiveType::UCHAR,
                true,
                bgfx::Access::Read,
                releaseFn);
        }
        mPointColors.setOwned(std::move(pointColors));
    }
    else {
        mPointColors.setOwned();
    }

    setPointSplatsBuffer(mPointSplats, mPointsSize);
    setPointSplatIndicesBuffer(mPointSplatIndices, mPointsSize);
}

void ScreenSpacePoints::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexColors)
{
    mPointsSize = pointsSize;

    mPoints.setReferenced(&vertexCoords);
    mPointColors.setReferenced(&vertexColors);

    setPointSplatsBuffer(mPointSplats, mPointsSize);
    setPointSplatIndicesBuffer(mPointSplatIndices, mPointsSize);
}

void ScreenSpacePoints::draw(bgfx::ViewId viewId) const
{
    if (mPointsSize == 0 || !mPoints.isValid() || !mPointSplats.isValid() ||
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
    if (mPointColors.isValid()) {
        mPointColors.get().bindCompute(POINTS_COLORS_STAGE, bgfx::Access::Read);
    }
    mPointSplats.bindCompute(POINTS_OUTPUT_STAGE, bgfx::Access::Write);

    ScreenSpacePointsUniforms::bind();
    bgfx::dispatch(
        viewId,
        pm.getComputeProgram<ComputeProgram::SCREENSPACE_POINTS>(),
        mPointsSize,
        1,
        1);

    mPointSplats.bindVertex(0);
    mPointSplatIndices.bind();

    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

    ScreenSpacePointsUniforms::bind();
    bgfx::submit(
        viewId,
        pm.getProgram<VertFragProgram::SCREENSPACE_POINTS>());
}

} // namespace vcl
