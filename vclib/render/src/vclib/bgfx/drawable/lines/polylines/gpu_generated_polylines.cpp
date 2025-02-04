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

GPUGeneratedPolylines::GPUGeneratedPolylines(
    const std::vector<LinesVertex>& points) :
        mPointsSize(points.size()),
        mComputeDataUH(
            bgfx::createUniform("u_numWorksGroups", bgfx::UniformType::Vec4))
{
    checkCaps();
    allocatePointsBuffer();
    allocateVertexBuffer();
    allocateIndexBuffer();

    setPointsBuffer(points);
    generateBuffers();
}

GPUGeneratedPolylines::GPUGeneratedPolylines(GPUGeneratedPolylines&& other)
{
    swap(other);
}

GPUGeneratedPolylines::~GPUGeneratedPolylines()
{
    if (bgfx::isValid(mPointsBH))
        bgfx::destroy(mPointsBH);

    if (bgfx::isValid(mVertexBH))
        bgfx::destroy(mVertexBH);

    if (bgfx::isValid(mSegmentsIndicesBH))
        bgfx::destroy(mSegmentsIndicesBH);

    if (bgfx::isValid(mJoinesIndicesBH))
        bgfx::destroy(mJoinesIndicesBH);

    if (bgfx::isValid(mComputeDataUH))
        bgfx::destroy(mComputeDataUH);
}

GPUGeneratedPolylines& GPUGeneratedPolylines::operator=(
    GPUGeneratedPolylines&& other)
{
    swap(other);
    return *this;
}

void GPUGeneratedPolylines::swap(GPUGeneratedPolylines& other)
{
    Lines::swap(other);

    std::swap(mPointsSize, other.mPointsSize);

    std::swap(mPointsBH, other.mPointsBH);
    std::swap(mVertexBH, other.mVertexBH);

    std::swap(mSegmentsIndicesBH, other.mSegmentsIndicesBH);
    std::swap(mJoinesIndicesBH, other.mJoinesIndicesBH);

    std::swap(mComputeDataUH, other.mComputeDataUH);
}

void GPUGeneratedPolylines::draw(uint viewId) const
{
    bindSettingsUniformPolylines();

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                     BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                     UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, mVertexBH);
    bgfx::setIndexBuffer(mSegmentsIndicesBH);
    bgfx::setState(state);
    bgfx::submit(viewId, mLinesPH);

    if (settings().getJoin() != 0) {
        bgfx::setVertexBuffer(0, mVertexBH);
        bgfx::setIndexBuffer(mJoinesIndicesBH);
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH);
    }
}

void GPUGeneratedPolylines::update(const std::vector<LinesVertex>& points)
{
    int oldSize = mPointsSize;
    mPointsSize = points.size();

    if (oldSize != mPointsSize) {
        bgfx::destroy(mSegmentsIndicesBH);
        bgfx::destroy(mJoinesIndicesBH);
        allocateIndexBuffer();
    }

    if (oldSize < mPointsSize) {
        bgfx::destroy(mVertexBH);
        allocateVertexBuffer();
    }

    if (oldSize > mPointsSize) {
        bgfx::destroy(mPointsBH);
        allocatePointsBuffer();
    }

    setPointsBuffer(points);
    generateBuffers();
}

void GPUGeneratedPolylines::generateBuffers()
{
    float data[] = {static_cast<float>(mPointsSize - 1), 0, 0, 0};
    bgfx::setUniform(mComputeDataUH, data);

    bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
    bgfx::setBuffer(1, mVertexBH, bgfx::Access::Write);
    bgfx::setBuffer(2, mSegmentsIndicesBH, bgfx::Access::Write);
    bgfx::setBuffer(3, mJoinesIndicesBH, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeVertexPH, mPointsSize - 1, 1, 1);
}

void GPUGeneratedPolylines::allocateVertexBuffer()
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

    mVertexBH = bgfx::createDynamicVertexBuffer(
        (mPointsSize - 1) * 4,
        layout,
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE);
}

void GPUGeneratedPolylines::allocateIndexBuffer()
{
    mSegmentsIndicesBH = bgfx::createDynamicIndexBuffer(
        ((mPointsSize - 1) * 6),
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE |
            BGFX_BUFFER_INDEX32);

    mJoinesIndicesBH = bgfx::createDynamicIndexBuffer(
        ((mPointsSize - 2) * 6),
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE |
            BGFX_BUFFER_INDEX32);
}

void GPUGeneratedPolylines::allocatePointsBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .end();

    mPointsBH = bgfx::createDynamicVertexBuffer(
        mPointsSize,
        layout,
        BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
}

void GPUGeneratedPolylines::setPointsBuffer(
    const std::vector<LinesVertex>& points)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<LinesVertex>(points.size());

    std::copy(points.begin(), points.end(), buffer);

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(buffer, sizeof(LinesVertex) * points.size(), releaseFn));
}

} // namespace vcl::lines
