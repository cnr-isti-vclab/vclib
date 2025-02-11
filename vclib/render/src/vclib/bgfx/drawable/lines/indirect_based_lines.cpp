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

#include <vclib/bgfx/drawable/lines/indirect_based_lines.h>

namespace vcl {

IndirectBasedLines::IndirectBasedLines()
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

    mIndirect.create(1);
}

IndirectBasedLines::IndirectBasedLines(const std::vector<LinesVertex>& points) :
        IndirectBasedLines()
{
    setPoints(points);
}

void IndirectBasedLines::swap(IndirectBasedLines& other)
{
    using std::swap;
    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
    swap(mPoints, other.mPoints);

    swap(mIndirect, other.mIndirect);
    swap(mIndirectData, other.mIndirectData);
}

void IndirectBasedLines::draw(uint viewId) const
{
    bindSettingsUniform();

    mVertices.bind(0);
    mIndices.bind();

    mPoints.bind(1, bgfx::Access::Read);

    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH, mIndirect.handle(), 0);
}

void IndirectBasedLines::setPoints(const std::vector<LinesVertex>& points)
{
    generateIndirectBuffer(points.size());
    allocateAndSetPointsBuffer(points);
}

void IndirectBasedLines::allocateAndSetPointsBuffer(
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
        BGFX_BUFFER_COMPUTE_READ,
        true);
}

void IndirectBasedLines::generateIndirectBuffer(uint pointSize)
{
    float data[] = {static_cast<float>(pointSize / 2), 0, 0, 0};
    mIndirectData.bind(data);
    mIndirect.bind(0, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeIndirectPH);
}

} // namespace vcl
