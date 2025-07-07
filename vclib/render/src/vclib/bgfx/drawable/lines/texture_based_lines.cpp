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
    const uint nVertices, 
    const VertexBuffer& vertCoords,
    const VertexBuffer& vertColors,
    const VertexBuffer& vertNormals,
    const VertexBuffer& lineColors) :
        TextureBasedLines()
{
    setPoints(nVertices, vertCoords, vertColors, vertNormals, lineColors);
}

TextureBasedLines::TextureBasedLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals,
    const std::vector<uint>& lineColors) :
        TextureBasedLines()
{
    setPoints(vertCoords, vertColors, vertNormals, lineColors);
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
    swap(mLineColors, other.mLineColors);

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
    const std::vector<float>& vertNormals,
    const std::vector<uint>& lineColors)
{
    mNumPoints = vertCoords.size() / 3;
    setCoordsBuffer(vertCoords);

    if (!vertColors.empty())
        setColorsBuffer(vertColors);

    if (!vertNormals.empty())
        setNormalsBuffer(vertNormals);

    if (!lineColors.empty())
        setLineColorsBuffer(lineColors);

    allocateAndGenerateTextureBuffer();
}

void TextureBasedLines::setPoints( 
    const uint nVertices, 
    const VertexBuffer& vertCoords,
    const VertexBuffer& vertColors,
    const VertexBuffer& vertNormals,
    const VertexBuffer& lineColors) 
{
    mNumPoints = nVertices;

    allocateAndGenerateTextureBuffer(vertCoords, vertColors, vertNormals, lineColors);
}

void TextureBasedLines::setCoordsBuffer(const std::vector<float>& vertCoords)
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

void TextureBasedLines::setColorsBuffer(const std::vector<uint>& vertColors)
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

void TextureBasedLines::setNormalsBuffer(const std::vector<float>& vertNormals)
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

void TextureBasedLines::setLineColorsBuffer(const std::vector<uint>& lineColors)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<uint>(lineColors.size());

    std::copy(lineColors.begin(), lineColors.end(), buffer);

    mLineColors.createForCompute(
        buffer,
        lineColors.size(),
        bgfx::Attrib::Color0,
        4,
        PrimitiveType::UCHAR,
        true,
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
    mLineColors.bind(3);

    mTexture.bind(4, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeTexturePH, (mNumPoints / 2), 1, 1);
}

void TextureBasedLines::allocateAndGenerateTextureBuffer(
    const VertexBuffer& vertCoords,
    const VertexBuffer& vertColors,
    const VertexBuffer& vertNormals,
    const VertexBuffer& lineColors)
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

    vertCoords.bind(0);

    if (!vertColors.isValid())
        vertColors.bind(1);
    else if (mVertColors.isValid())
        mVertColors.bind(1);
    else throw std::runtime_error("No valid Vertex Buffer");

    if (!vertNormals.isValid())
        vertNormals.bind(2);
    else if (mVertNormals.isValid())
        mVertNormals.bind(2);
    else throw std::runtime_error("No valid Vertex Buffer");
      
    if (!lineColors.isValid())
        lineColors.bind(3);
    else if (mLineColors.isValid())
        mLineColors.bind(3);
    else throw std::runtime_error("No valid Vertex Buffer");

    mTexture.bind(4, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeTexturePH, (mNumPoints / 2), 1, 1);
}

} // namespace vcl
