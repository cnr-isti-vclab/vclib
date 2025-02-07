/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/bgfx/context/load_program.h>
#include <vclib/bgfx/drawable/lines/polylines/gpu_generated_polylines.h>

namespace vcl::lines {

GPUGeneratedPolylines::GPUGeneratedPolylines()
{
    checkCaps();
}

GPUGeneratedPolylines::GPUGeneratedPolylines(
    const std::vector<LinesVertex>& points) : Lines()
{
    update(points);
}

void GPUGeneratedPolylines::swap(GPUGeneratedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mPoints, other.mPoints);
    swap(mVertices, other.mVertices);

    swap(mSegmentIndices, other.mSegmentIndices);
    swap(mJoinIndices, other.mJoinIndices);

    swap(mComputeData, other.mComputeData);
}

void GPUGeneratedPolylines::draw(uint viewId) const
{
    bindSettingsUniformPolylines();

    mVertices.bind(0);
    mSegmentIndices.bind();
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);

    // mJoinIndices is valid only if there are more than 2 points
    if (mJoinIndices.isValid() && settings().getJoin() != 0) {
        mVertices.bind(0);
        mJoinIndices.bind();
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }
}

void GPUGeneratedPolylines::update(const std::vector<LinesVertex>& points)
{
    if (points.size() > 1) {
        allocateAndSetPointsBuffer(points);
        allocateVertexBuffer(points.size());
        allocateIndexBuffer(points.size());
        // buffers are set for the compute stage
        generateVerticesAndIndicesBuffers(points.size());
        // here buffers are computed and ready for rendering
    }
    else {
        mPoints.destroy();
        mVertices.destroy();
        mSegmentIndices.destroy();
        mJoinIndices.destroy();
    }
}

void GPUGeneratedPolylines::allocateAndSetPointsBuffer(
    const std::vector<LinesVertex>& points)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .end();

    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<LinesVertex>(points.size());

    std::copy(points.begin(), points.end(), buffer);

    mPoints.create(
        bgfx::makeRef(buffer, sizeof(LinesVertex) * points.size(), releaseFn),
        layout,
        BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE,
        true);
}

void GPUGeneratedPolylines::allocateVertexBuffer(uint pointSize)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
        .end();

    const uint sz = (pointSize - 1) * 4 * 15;

    mVertices.create(
        bgfx::makeRef(nullptr, sizeof(float) * sz),
        layout,
        BGFX_BUFFER_COMPUTE_WRITE,
        true);
}

void GPUGeneratedPolylines::allocateIndexBuffer(uint pointSize)
{
    const uint szSeg = (pointSize - 1) * 6;
    const uint szJoin = (pointSize - 2) * 6;

    mSegmentIndices.create(
        bgfx::makeRef(nullptr, sizeof(uint) * szSeg),
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32,
        true);

    if (pointSize > 2) {
        mJoinIndices.create(
            bgfx::makeRef(nullptr, sizeof(uint) * szJoin),
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32,
            true);
    }
    else {
        mJoinIndices.destroy();
    }
}

void GPUGeneratedPolylines::generateVerticesAndIndicesBuffers(uint pointSize)
{
    float data[] = {static_cast<float>(pointSize - 1), 0, 0, 0};
    mComputeData.bind(data);

    mPoints.bind(0);
    mVertices.bind(1, bgfx::Access::Write);
    mSegmentIndices.bind(2, bgfx::Access::Write);
    mJoinIndices.bind(3, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeVertexPH, pointSize - 1, 1, 1);
    // after the dispatch, the vert and indices buffers are ready to be used in
    // the rendering pipeline
    mVertices.setCompute(false);
    mSegmentIndices.setCompute(false);
    mJoinIndices.setCompute(false);
}

} // namespace vcl::lines
