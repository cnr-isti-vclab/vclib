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

#include <vclib/bgfx/drawable/lines/lines/indirect_based_lines.h>

#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {

IndirectBasedLines::IndirectBasedLines(const std::vector<LinesVertex>& points) :
        mPoints(points), mIndirectBH(bgfx::createIndirectBuffer(1)),
        mIndirectDataUH(
            bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
{
    checkCaps();
    assert(bgfx::isValid(mComputeIndirectPH));
    allocateVerticesBuffer();
    allocateIndexesBuffer();

    generateIndirectBuffer();
    allocatePointsBuffer();
    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
}

IndirectBasedLines::IndirectBasedLines(IndirectBasedLines&& other)
{
    swap(other);
}

IndirectBasedLines::~IndirectBasedLines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mIndexesBH))
        bgfx::destroy(mIndexesBH);

    if (bgfx::isValid(mPointsBH))
        bgfx::destroy(mPointsBH);

    if (bgfx::isValid(mIndirectBH))
        bgfx::destroy(mIndirectBH);

    if (bgfx::isValid(mIndirectDataUH))
        bgfx::destroy(mIndirectDataUH);
}

IndirectBasedLines& IndirectBasedLines::operator=(IndirectBasedLines&& other)
{
    swap(other);
    return *this;
}

void IndirectBasedLines::swap(IndirectBasedLines& other)
{
    Lines::swap(other);

    std::swap(mPoints, other.mPoints);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mIndexesBH, other.mIndexesBH);
    std::swap(mPointsBH, other.mPointsBH);

    std::swap(mIndirectBH, other.mIndirectBH);
    std::swap(mIndirectDataUH, other.mIndirectDataUH);
}

void IndirectBasedLines::allocatePointsBuffer()
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

void IndirectBasedLines::allocateVerticesBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .end();

    mVerticesBH = bgfx::createVertexBuffer(
        bgfx::makeRef(&VERTICES[0], sizeof(float) * VERTICES.size()), layout);
}

void IndirectBasedLines::allocateIndexesBuffer()
{
    mIndexesBH = bgfx::createIndexBuffer(
        bgfx::makeRef(&INDICES[0], sizeof(uint) * INDICES.size()),
        BGFX_BUFFER_INDEX32);
}

void IndirectBasedLines::generateIndirectBuffer()
{
    float data[] = {static_cast<float>(mPoints.size() / 2), 0, 0, 0};
    bgfx::setUniform(mIndirectDataUH, data);
    bgfx::setBuffer(0, mIndirectBH, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeIndirectPH);
}

void IndirectBasedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                     BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                     UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndexesBH);

    bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);

    bgfx::setState(state);
    bgfx::submit(viewId, mLinesPH, mIndirectBH, 0);
}

void IndirectBasedLines::update(const std::vector<LinesVertex>& points)
{
    int oldSize = mPoints.size();
    mPoints     = points;

    if (oldSize != mPoints.size()) {
        generateIndirectBuffer();
    }

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
}

} // namespace vcl::lines
