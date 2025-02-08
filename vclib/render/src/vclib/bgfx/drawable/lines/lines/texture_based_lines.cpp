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
#include <vclib/bgfx/drawable/lines/lines/texture_based_lines.h>

namespace vcl {

TextureBasedLines::TextureBasedLines()
{
    checkCaps();

    mVertices.create(
        VERTICES.data(),
        VERTICES.size(),
        bgfx::Attrib::Position,
        2,
        PrimitiveType::FLOAT);

    mIndices.create(INDICES.data(), INDICES.size());

    mIndirect.create(1);
}

TextureBasedLines::TextureBasedLines(const std::vector<LinesVertex>& points) :
        TextureBasedLines()
{
    setPoints(points);
}

void TextureBasedLines::swap(TextureBasedLines& other)
{
    using std::swap;
    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);

    swap(mPoints, other.mPoints);
    swap(mTexture, other.mTexture);

    swap(mIndirect, other.mIndirect);
    swap(mIndirectData, other.mIndirectData);
}

void TextureBasedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    mVertices.bind(0);
    mIndices.bind();
    mTexture.bind(0, bgfx::Access::Read);
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH, mIndirect.handle(), 0);
}

void TextureBasedLines::setPoints(const std::vector<LinesVertex>& points)
{
    allocateAndSetPointsBuffer(points);
    allocateAndGenerateTextureBuffer(points.size());
}

void TextureBasedLines::allocateAndSetPointsBuffer(
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

void TextureBasedLines::allocateAndGenerateTextureBuffer(uint pointSize)
{
    uint16_t Y = (pointSize * 3) / (mMaxTextureSize + 1);
    uint16_t X = Y == 0 ? (pointSize * 3) : mMaxTextureSize;

    mTexture.create(
        X, Y + 1, bgfx::TextureFormat::RGBA32F, BGFX_TEXTURE_COMPUTE_WRITE);

    float data[] = {
                    static_cast<float>(mMaxTextureSize),
                    static_cast<float>(pointSize / 2),
                    0,
                    0};
    mIndirectData.bind(data);

    mPoints.bind(0);
    mTexture.bind(1, bgfx::Access::Write);
    mIndirect.bind(2, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeTexturePH, (pointSize / 2), 1, 1);
}

} // namespace vcl
