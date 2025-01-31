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

namespace vcl::lines {
TextureBasedLines::TextureBasedLines(
    const std::vector<LinesVertex>& points,
    const uint32_t                  maxTextureSize) :
        mMaxTextureSize(maxTextureSize), mPoints(points),
        mIndirectBH(bgfx::createIndirectBuffer(1)),
        mIndirectDataUH(
            bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
{
    allocateIndexesBuffer();
    allocateVerticesBuffer();
    allocatePointsBuffer();
    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));

    allocateTextureBuffer();
    generateTextureBuffer();
}

TextureBasedLines::TextureBasedLines(const TextureBasedLines& other) :
        DrawableLines(other)
{
    mPoints         = other.mPoints;
    mMaxTextureSize = other.mMaxTextureSize;
    mIndirectBH     = bgfx::createIndirectBuffer(1);
    mIndirectDataUH =
        bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);

    allocateIndexesBuffer();
    allocateVerticesBuffer();
    allocatePointsBuffer();
    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));

    allocateTextureBuffer();
    generateTextureBuffer();
}

TextureBasedLines::TextureBasedLines(TextureBasedLines&& other) :
        DrawableLines(other)
{
    swap(other);
}

TextureBasedLines::~TextureBasedLines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mIndexesBH))
        bgfx::destroy(mIndexesBH);

    if (bgfx::isValid(mPointsBH))
        bgfx::destroy(mPointsBH);

    if (bgfx::isValid(mIndirectBH))
        bgfx::destroy(mIndirectBH);

    if (bgfx::isValid(mTextureBH))
        bgfx::destroy(mTextureBH);

    if (bgfx::isValid(mIndirectDataUH))
        bgfx::destroy(mIndirectDataUH);
}

TextureBasedLines& TextureBasedLines::operator=(TextureBasedLines other)
{
    swap(other);
    return *this;
}

void TextureBasedLines::swap(TextureBasedLines& other)
{
    std::swap(mSettings, other.mSettings);
    std::swap(mVisible, other.mVisible);

    std::swap(mMaxTextureSize, other.mMaxTextureSize);
    std::swap(mPoints, other.mPoints);

    std::swap(mTextureBH, other.mTextureBH);
    std::swap(mPointsBH, other.mPointsBH);

    std::swap(mIndirectBH, other.mIndirectBH);
    std::swap(mIndirectDataUH, other.mIndirectDataUH);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mIndexesBH, other.mIndexesBH);
}

std::shared_ptr<vcl::DrawableObject> TextureBasedLines::clone() const
{
    return std::make_shared<TextureBasedLines>(*this);
}

void TextureBasedLines::draw(uint viewId) const
{
    mSettings.bindUniformLines();

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                     BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                     UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndexesBH);
    bgfx::setImage(
        0, mTextureBH, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
    bgfx::setState(state);
    bgfx::submit(viewId, mLinesPH, mIndirectBH, 0);
}

void TextureBasedLines::update(const std::vector<LinesVertex>& points)
{
    int oldSize = mPoints.size();
    mPoints     = points;

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));

    if (oldSize < mPoints.size()) {
        allocateTextureBuffer();
    }

    generateTextureBuffer();
}

void TextureBasedLines::generateTextureBuffer()
{
    float data[] = {
        static_cast<float>(mMaxTextureSize),
        static_cast<float>(mPoints.size() / 2),
        0,
        0};
    bgfx::setUniform(mIndirectDataUH, data);

    bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
    bgfx::setImage(1, mTextureBH, 0, bgfx::Access::Write);
    bgfx::setBuffer(2, mIndirectBH, bgfx::Access::ReadWrite);
    bgfx::dispatch(0, mComputeTexturePH, (mPoints.size() / 2), 1, 1);
}

void TextureBasedLines::allocateTextureBuffer()
{
    uint16_t Y = (mPoints.size() * 3) / (mMaxTextureSize + 1);
    uint16_t X = Y == 0 ? (mPoints.size() * 3) : mMaxTextureSize;

    mTextureBH = bgfx::createTexture2D(
        X,
        Y + 1,
        false,
        1,
        bgfx::TextureFormat::RGBA32F,
        BGFX_TEXTURE_COMPUTE_WRITE);
}

void TextureBasedLines::allocatePointsBuffer()
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

void TextureBasedLines::allocateVerticesBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .end();

    mVerticesBH = bgfx::createVertexBuffer(
        bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()), layout);
}

void TextureBasedLines::allocateIndexesBuffer()
{
    mIndexesBH = bgfx::createIndexBuffer(
        bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()),
        BGFX_BUFFER_INDEX32);
}

} // namespace vcl::lines
