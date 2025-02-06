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

#include <vclib/bgfx/drawable/lines/lines/gpu_generated_lines.h>

#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {

GPUGeneratedLines::GPUGeneratedLines(const std::vector<LinesVertex>& points)
{
    checkCaps();
    assert(bgfx::isValid(mComputeVerticesPH));

    update(points);
}

GPUGeneratedLines::GPUGeneratedLines(GPUGeneratedLines&& other)
{
    swap(other);
}

GPUGeneratedLines::~GPUGeneratedLines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mIndicesBH))
        bgfx::destroy(mIndicesBH);
}

GPUGeneratedLines& GPUGeneratedLines::operator=(GPUGeneratedLines&& other)
{
    swap(other);
    return *this;
}

void GPUGeneratedLines::swap(GPUGeneratedLines& other)
{
    Lines::swap(other);

    std::swap(mPoints, other.mPoints);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mIndicesBH, other.mIndicesBH);
}

void GPUGeneratedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndicesBH);
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);
}

void GPUGeneratedLines::update(const std::vector<LinesVertex>& points)
{
    allocateAndSetPointsBuffer(points);

    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);
    allocateVertexBuffer(points.size());

    if (bgfx::isValid(mIndicesBH))
        bgfx::destroy(mIndicesBH);
    allocateIndexBuffer(points.size());

    generateBuffers(points.size());
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

    mVerticesBH = bgfx::createDynamicVertexBuffer(
        (pointsSize / 2) * 4,
        layout,
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE);
}

void GPUGeneratedLines::allocateIndexBuffer(uint pointsSize)
{
    mIndicesBH = bgfx::createDynamicIndexBuffer(
        (pointsSize / 2) * 6,
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE |
            BGFX_BUFFER_INDEX32);
}

void GPUGeneratedLines::generateBuffers(uint pointsSize)
{
    assert(mPoints.isCompute());
    mPoints.bind(0);
    //bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
    bgfx::setBuffer(1, mVerticesBH, bgfx::Access::Write);
    bgfx::setBuffer(2, mIndicesBH, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeVerticesPH, (pointsSize / 2), 1, 1);
}

} // namespace vcl::lines
