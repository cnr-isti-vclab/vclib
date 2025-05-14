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

#include <vclib/bgfx/drawable/lines/texture_based_lines.h>

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
}

TextureBasedLines::TextureBasedLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals) :
        TextureBasedLines()
{
    setPoints(vertCoords, vertColors, vertNormals);
}

void TextureBasedLines::swap(TextureBasedLines& other)
{
    using std::swap;
    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);

    swap(mTexture, other.mTexture);
    swap(mNumPoints, other.mNumPoints);

    swap(mIndirectData, other.mIndirectData);
}

void TextureBasedLines::draw(uint viewId) const
{
    bindSettingsUniform();

    mVertices.bind(0);
    mIndices.bind();
    mTexture.bind(0, bgfx::Access::Read);
    bgfx::setInstanceCount(mNumPoints / 2);
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);
}

void TextureBasedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    mNumPoints = vertCoords.size() / 3;
    setCoordsBuffers(vertCoords);
    setColorsBuffers(vertColors);
    setNormalsBuffers(vertNormals);

    allocateAndGenerateTextureBuffer();
}

void TextureBasedLines::setCoordsBuffers(const std::vector<float>& vertCoords)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<float>(vertCoords.size());

    std::copy(vertCoords.begin(), vertCoords.end(), buffer);

    mVertCoords.createForCompute(
        buffer,
        vertCoords.size() / 3,
        bgfx::Attrib::Position,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn
    );
}

void TextureBasedLines::setColorsBuffers(const std::vector<uint>& vertColors)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<uint>(vertColors.size());

    std::copy(vertColors.begin(), vertColors.end(), buffer);

    mVertColors.createForCompute(
        buffer,
        vertColors.size(),
        bgfx::Attrib::Color0,
        4,
        PrimitiveType::UCHAR,
        true,
        bgfx::Access::Read,
        releaseFn
    );
}

void TextureBasedLines::setNormalsBuffers(const std::vector<float>& vertNormals)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<float>(vertNormals.size());

    std::copy(vertNormals.begin(), vertNormals.end(), buffer);

    mVertNormals.createForCompute(
        buffer,
        vertNormals.size() / 3,
        bgfx::Attrib::Normal,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn
    );
}

void TextureBasedLines::allocateAndGenerateTextureBuffer()
{
    uint16_t Y = (mNumPoints * 3) / (mMaxTextureSize + 1);
    uint16_t X = Y == 0 ? (mNumPoints * 3) : mMaxTextureSize;

    mTexture.create(
        X, Y + 1, bgfx::TextureFormat::RGBA32F, BGFX_TEXTURE_COMPUTE_WRITE);

    float data[] = {
                    static_cast<float>(mMaxTextureSize),
                    static_cast<float>(mNumPoints / 2),
                    0,
                    0};
    mIndirectData.bind(data);

    mVertCoords.bind(0);
    mVertColors.bind(1);
    mVertNormals.bind(2);

    mTexture.bind(3, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeTexturePH, (mNumPoints / 2), 1, 1);
}

} // namespace vcl
