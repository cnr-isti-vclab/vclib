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

#include <vclib/bgfx/drawable/lines/gpu_generated_lines.h>

namespace vcl {

GPUGeneratedLines::GPUGeneratedLines(const std::vector<LinesVertex>& points)
{
    checkCaps();
    assert(bgfx::isValid(mComputeVerticesPH));

    setPoints(points);
}

void GPUGeneratedLines::swap(GPUGeneratedLines& other)
{
    Lines::swap(other);

    std::swap(mPoints, other.mPoints);

    std::swap(mVertices, other.mVertices);
    std::swap(mIndices, other.mIndices);
}

void GPUGeneratedLines::draw(uint viewId) const
{
    bindSettingsUniform();

    mVertices.bind(0);
    mIndices.bind();
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);
}

void GPUGeneratedLines::setPoints(const std::vector<LinesVertex>& points)
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
        mIndices.destroy();
    }
}

void GPUGeneratedLines::allocateAndSetPointsBuffer(
    const std::vector<LinesVertex>& points)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
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

void GPUGeneratedLines::allocateVertexBuffer(uint pointsSize)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
        .end();


    const uint sz = pointsSize * 4 * 12;

    mVertices.create(
        bgfx::makeRef(nullptr, sizeof(float) * sz),
        layout,
        BGFX_BUFFER_COMPUTE_WRITE,
        true);
}

void GPUGeneratedLines::allocateIndexBuffer(uint pointsSize)
{
    const uint sz = (pointsSize / 2) * 6;

    mIndices.create(
        bgfx::makeRef(nullptr, sizeof(uint) * sz),
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32,
        true);
}

void GPUGeneratedLines::generateVerticesAndIndicesBuffers(uint pointsSize)
{
    mPoints.bind(0);
    mVertices.bind(1, bgfx::Access::Write);
    mIndices.bind(2, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeVerticesPH, (pointsSize / 2), 1, 1);
    // after the dispatch, the vert and indices buffers are ready to be used in
    // the rendering pipeline
    mVertices.setCompute(false);
    mIndices.setCompute(false);
}

} // namespace vcl
