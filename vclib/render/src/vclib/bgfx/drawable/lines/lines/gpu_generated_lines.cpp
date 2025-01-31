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
#include <vclib/bgfx/drawable/lines/lines/gpu_generated_lines.h>

namespace vcl::lines {

GPUGeneratedLines::GPUGeneratedLines(const std::vector<LinesVertex>& points) :
        mPoints(points)
{
    assert(bgfx::isValid(mComputeVerticesPH));
    allocateVertexBuffer();
    allocateIndexBuffer();
    allocatePointsBuffer();

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    generateBuffers();
}

GPUGeneratedLines::GPUGeneratedLines(const GPUGeneratedLines& other) :
        DrawableLines(other)
{
    mPoints = other.mPoints;
    assert(bgfx::isValid(mComputeVerticesPH));

    allocateIndexBuffer();
    allocateVertexBuffer();
    allocatePointsBuffer();

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    generateBuffers();
}

GPUGeneratedLines::GPUGeneratedLines(GPUGeneratedLines&& other) :
        DrawableLines(other)
{
    swap(other);
}

GPUGeneratedLines::~GPUGeneratedLines()
{
    if (bgfx::isValid(mIndexesBH))
        bgfx::destroy(mIndexesBH);

    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mPointsBH))
        bgfx::destroy(mPointsBH);
}

GPUGeneratedLines& GPUGeneratedLines::operator=(GPUGeneratedLines other)
{
    swap(other);
    return *this;
}

void GPUGeneratedLines::swap(GPUGeneratedLines& other)
{
    std::swap(mSettings, other.mSettings);
    std::swap(mVisible, other.mVisible);

    std::swap(mIndexesBH, other.mIndexesBH);
    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mPoints, other.mPoints);
    std::swap(mPointsBH, other.mPointsBH);
}

std::shared_ptr<vcl::DrawableObject> GPUGeneratedLines::clone() const
{
    return std::make_shared<GPUGeneratedLines>(*this);
}

void GPUGeneratedLines::draw(uint viewId) const
{
    mSettings.bindUniformLines();

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                     BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                     UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndexesBH);
    bgfx::setState(state);
    bgfx::submit(viewId, mLinesPH);
}

void GPUGeneratedLines::update(const std::vector<LinesVertex>& points)
{
    int oldSize = mPoints.size();
    mPoints     = points;

    if (oldSize != mPoints.size()) {
        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);
        allocateIndexBuffer();
    }

    if (oldSize < mPoints.size()) {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);
        allocateVertexBuffer();
    }

    if (oldSize > mPoints.size()) {
        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);
        allocatePointsBuffer();
    }

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    generateBuffers();
}

void GPUGeneratedLines::generateBuffers()
{
    bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
    bgfx::setBuffer(1, mVerticesBH, bgfx::Access::Write);
    bgfx::setBuffer(2, mIndexesBH, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeVerticesPH, (mPoints.size() / 2), 1, 1);
}

void GPUGeneratedLines::allocateVertexBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
        .end();

    mVerticesBH = bgfx::createDynamicVertexBuffer(
        (mPoints.size() / 2) * 4,
        layout,
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE);
}

void GPUGeneratedLines::allocateIndexBuffer()
{
    mIndexesBH = bgfx::createDynamicIndexBuffer(
        (mPoints.size() / 2) * 6,
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE |
            BGFX_BUFFER_INDEX32);
}

void GPUGeneratedLines::allocatePointsBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .end();

    mPointsBH = bgfx::createDynamicVertexBuffer(
        mPoints.size(),
        layout,
        BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
}

} // namespace vcl::lines
