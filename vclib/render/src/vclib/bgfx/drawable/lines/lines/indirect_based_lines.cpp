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

IndirectBasedLines::IndirectBasedLines() :
        mIndirectBH(bgfx::createIndirectBuffer(1))
{
    checkCaps();
    assert(bgfx::isValid(mComputeIndirectPH));

    mVertices.create(
        VERTICES.data(),
        VERTICES.size(),
        bgfx::Attrib::Position,
        2,
        PrimitiveType::FLOAT);

    mIndices.create(INDICES.data(), INDICES.size());
}

IndirectBasedLines::IndirectBasedLines(const std::vector<LinesVertex>& points) :
        IndirectBasedLines()
{
    update(points);
}

IndirectBasedLines::IndirectBasedLines(IndirectBasedLines&& other)
{
    swap(other);
}

IndirectBasedLines::~IndirectBasedLines()
{
    if (bgfx::isValid(mPointsBH))
        bgfx::destroy(mPointsBH);

    if (bgfx::isValid(mIndirectBH))
        bgfx::destroy(mIndirectBH);
}

IndirectBasedLines& IndirectBasedLines::operator=(IndirectBasedLines&& other)
{
    swap(other);
    return *this;
}

void IndirectBasedLines::swap(IndirectBasedLines& other)
{
    using std::swap;
    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
    swap(mPointsBH, other.mPointsBH);

    swap(mIndirectBH, other.mIndirectBH);
    swap(mIndirectData, other.mIndirectData);
}

void IndirectBasedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    mVertices.bind(0);
    mIndices.bind();

    bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);

    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH, mIndirectBH, 0);
}

void IndirectBasedLines::update(const std::vector<LinesVertex>& points)
{
    generateIndirectBuffer(points.size());
    allocateAndSetPointsBuffer(points);
}

void IndirectBasedLines::allocateAndSetPointsBuffer(
    const std::vector<LinesVertex>& points)
{
    if (bgfx::isValid(mPointsBH))
        bgfx::destroy(mPointsBH);

    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .end();

    mPointsBH = bgfx::createDynamicVertexBuffer(
        points.size(),
        layout,
        BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);

    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<LinesVertex>(points.size());

    std::copy(points.begin(), points.end(), buffer);

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(buffer, sizeof(LinesVertex) * points.size(), releaseFn));
}

void IndirectBasedLines::generateIndirectBuffer(uint pointSize)
{
    float data[] = {static_cast<float>(pointSize / 2), 0, 0, 0};
    mIndirectData.bind(data);
    bgfx::setBuffer(0, mIndirectBH, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeIndirectPH);
}

} // namespace vcl::lines
