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
#include <vclib/bgfx/drawable/lines/polylines/texture_based_polylines.h>

namespace vcl::lines {

TextureBasedPolylines::TextureBasedPolylines(
    const std::vector<LinesVertex>& points,
    const uint32_t                  maxTextureSize) :
        mPoints(points), mMaxTextureSize(maxTextureSize),
        mJoinesIndirectBH(bgfx::createIndirectBuffer(1)),
        mSegmentsIndirectBH(bgfx::createIndirectBuffer(1)),
        mComputeDataUH(
            bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
{
    assert(bgfx::isValid(mComputeTexturePH));

    allocateVerticesBuffer();
    allocateIndexesBuffer();
    allocateTextureBuffer();
    allocatePointsBuffer();

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    generateTextureBuffer();
}

TextureBasedPolylines::TextureBasedPolylines(
    const TextureBasedPolylines& other) : DrawablePolylines(other)
{
    mPoints             = other.mPoints;
    mMaxTextureSize     = other.mMaxTextureSize;
    mJoinesIndirectBH   = bgfx::createIndirectBuffer(1);
    mSegmentsIndirectBH = bgfx::createIndirectBuffer(1);
    mComputeDataUH =
        bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);

    assert(bgfx::isValid(mComputeTexturePH));

    allocateVerticesBuffer();
    allocateIndexesBuffer();
    allocateTextureBuffer();
    allocatePointsBuffer();

    bgfx::update(
        mPointsBH,
        0,
        bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    generateTextureBuffer();
}

TextureBasedPolylines::TextureBasedPolylines(TextureBasedPolylines&& other) :
        DrawablePolylines(other)
{
    swap(other);
}

TextureBasedPolylines::~TextureBasedPolylines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mIndexesBH))
        bgfx::destroy(mIndexesBH);

    if (bgfx::isValid(mSegmentsIndirectBH))
        bgfx::destroy(mSegmentsIndirectBH);

    if (bgfx::isValid(mJoinesIndirectBH))
        bgfx::destroy(mJoinesIndirectBH);

    if (bgfx::isValid(mComputeDataUH))
        bgfx::destroy(mComputeDataUH);

    if (bgfx::isValid(mSegmentsTextureBH))
        bgfx::destroy(mSegmentsTextureBH);

    if (bgfx::isValid(mJoinesTextureBH))
        bgfx::destroy(mJoinesTextureBH);

    if (bgfx::isValid(mPointsBH))
        bgfx::destroy(mPointsBH);
}

TextureBasedPolylines& TextureBasedPolylines::operator=(
    TextureBasedPolylines other)
{
    swap(other);
    return *this;
}

void TextureBasedPolylines::swap(TextureBasedPolylines& other)
{
    std::swap(mSettings, other.mSettings);
    std::swap(mVisible, other.mVisible);

    std::swap(mMaxTextureSize, other.mMaxTextureSize);
    std::swap(mPoints, other.mPoints);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mIndexesBH, other.mIndexesBH);
    std::swap(mPointsBH, other.mPointsBH);

    std::swap(mSegmentsIndirectBH, other.mSegmentsIndirectBH);
    std::swap(mJoinesIndirectBH, other.mJoinesIndirectBH);

    std::swap(mSegmentsTextureBH, other.mSegmentsTextureBH);
    std::swap(mJoinesTextureBH, other.mJoinesTextureBH);

    std::swap(mComputeDataUH, other.mComputeDataUH);
}

std::shared_ptr<vcl::DrawableObject> TextureBasedPolylines::clone() const
{
    return std::make_shared<TextureBasedPolylines>(*this);
}

void TextureBasedPolylines::draw(uint viewId) const
{
    mSettings.bindUniformPolylines();

    float indirectData[] = {
        static_cast<float>(mPoints.size() - 1),
        static_cast<float>(mMaxTextureSize),
        0,
        0};
    bgfx::setUniform(mComputeDataUH, indirectData);

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                     BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                     UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndexesBH);
    bgfx::setImage(
        0,
        mSegmentsTextureBH,
        0,
        bgfx::Access::Read,
        bgfx::TextureFormat::RGBA32F);
    bgfx::setState(state);
    bgfx::submit(viewId, mLinesPH, mSegmentsIndirectBH, 0);

    if (mSettings.getJoin() != 0 && mPoints.size() > 2) {
        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setImage(
            0,
            mJoinesTextureBH,
            0,
            bgfx::Access::Read,
            bgfx::TextureFormat::RGBA32F);
        bgfx::setState(state);
        bgfx::submit(viewId, mJoinesPH, mJoinesIndirectBH, 0);
    }
}

void TextureBasedPolylines::update(const std::vector<LinesVertex>& points)
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

void TextureBasedPolylines::generateTextureBuffer()
{
    float data[] = {
        static_cast<float>(mMaxTextureSize),
        static_cast<float>(mPoints.size() - 1),
        0,
        0};
    bgfx::setUniform(mComputeDataUH, data);

    bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
    bgfx::setImage(1, mSegmentsTextureBH, 0, bgfx::Access::Write);
    bgfx::setImage(2, mJoinesTextureBH, 0, bgfx::Access::Write);
    bgfx::setBuffer(3, mSegmentsIndirectBH, bgfx::Access::Write);
    bgfx::setBuffer(4, mJoinesIndirectBH, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeTexturePH, mPoints.size() - 1, 1, 1);
}

void TextureBasedPolylines::allocateTextureBuffer()
{
    uint16_t Y_Segments = ((mPoints.size() - 1) * 5) / (mMaxTextureSize + 1);
    uint16_t X_Segments =
        Y_Segments == 0 ? ((mPoints.size() - 1) * 5) : mMaxTextureSize;

    mSegmentsTextureBH = bgfx::createTexture2D(
        X_Segments,
        Y_Segments + 1,
        false,
        1,
        bgfx::TextureFormat::RGBA32F,
        BGFX_TEXTURE_COMPUTE_WRITE);

    if (mPoints.size() > 2) {
        uint16_t Y_Joins = ((mPoints.size() - 2) * 4) / (mMaxTextureSize + 1);
        uint16_t X_Joins =
            Y_Joins == 0 ? ((mPoints.size() - 2) * 4) : mMaxTextureSize;

        mJoinesTextureBH = bgfx::createTexture2D(
            X_Joins,
            Y_Joins + 1,
            false,
            1,
            bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE);
    }
}

void TextureBasedPolylines::allocatePointsBuffer()
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

void TextureBasedPolylines::allocateVerticesBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .end();

    mVerticesBH = bgfx::createVertexBuffer(
        bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()), layout);
}

void TextureBasedPolylines::allocateIndexesBuffer()
{
    mIndexesBH = bgfx::createIndexBuffer(
        bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()),
        BGFX_BUFFER_INDEX32);
}

} // namespace vcl::lines
