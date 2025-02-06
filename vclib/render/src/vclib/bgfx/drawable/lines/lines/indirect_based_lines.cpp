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
        mPointsSize(points.size()), mIndirectBH(bgfx::createIndirectBuffer(1)),
        mIndirectDataUH(
            bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
{
    checkCaps();
    assert(bgfx::isValid(mComputeIndirectPH));
    allocateVerticesBuffer();
    allocateIndicesBuffer();

    generateIndirectBuffer();
    allocatePointsBuffer();
    setPointsBuffer(points);
}

IndirectBasedLines::IndirectBasedLines(IndirectBasedLines&& other)
{
    swap(other);
}

IndirectBasedLines::~IndirectBasedLines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mIndicesBH))
        bgfx::destroy(mIndicesBH);

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

    std::swap(mPointsSize, other.mPointsSize);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mIndicesBH, other.mIndicesBH);
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
        mPointsSize,
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

void IndirectBasedLines::allocateIndicesBuffer()
{
    mIndicesBH = bgfx::createIndexBuffer(
        bgfx::makeRef(&INDICES[0], sizeof(uint) * INDICES.size()),
        BGFX_BUFFER_INDEX32);
}

void IndirectBasedLines::generateIndirectBuffer()
{
    float data[] = {static_cast<float>(mPointsSize / 2), 0, 0, 0};
    bgfx::setUniform(mIndirectDataUH, data);
    bgfx::setBuffer(0, mIndirectBH, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeIndirectPH);
}

void IndirectBasedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndicesBH);

    bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);

    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH, mIndirectBH, 0);
}

void IndirectBasedLines::update(const std::vector<LinesVertex>& points)
{
    int oldSize = mPointsSize;
    mPointsSize     = points.size();

    if (oldSize != mPointsSize) {
        generateIndirectBuffer();
    }

    setPointsBuffer(points);
}

void IndirectBasedLines::setPointsBuffer(const std::vector<LinesVertex>& points)
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
