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

TextureBasedPolylines::TextureBasedPolylines()
{
    checkCaps();
    assert(bgfx::isValid(mComputeTexturePH));

    mVertices.create(
        VERTICES.data(),
        VERTICES.size(),
        bgfx::Attrib::Position,
        2,
        PrimitiveType::FLOAT);

    mIndices.create(INDICES.data(), INDICES.size());

    mSegmentsIndirect.create(1);
    mJoinsIndirect.create(1);
}

TextureBasedPolylines::TextureBasedPolylines(
    const std::vector<LinesVertex>& points) : TextureBasedPolylines()
{
    update(points);
}

void TextureBasedPolylines::swap(TextureBasedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
    swap(mPoints, other.mPoints);

    swap(mSegmentsIndirect, other.mSegmentsIndirect);
    swap(mJoinsIndirect, other.mJoinsIndirect);

    swap(mSegmentsTexture, other.mSegmentsTexture);
    swap(mJoinsTexture, other.mJoinsTexture);

    swap(mIndirectData, other.mIndirectData);
}

void TextureBasedPolylines::draw(uint viewId) const
{
    bindSettingsUniformPolylines();

    mVertices.bind(0);
    mIndices.bind();
    mSegmentsTexture.bind(0, bgfx::Access::Read);
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH, mSegmentsIndirect.handle(), 0);

    // mJoinsTexture is valid only if there are more than 2 points
    if (settings().getJoin() != 0 && mJoinsTexture.isValid()) {
        mVertices.bind(0);
        mIndices.bind();
        mJoinsTexture.bind(0, bgfx::Access::Read);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mJoinsPH, mJoinsIndirect.handle(), 0);
    }
}

void TextureBasedPolylines::update(const std::vector<LinesVertex>& points)
{
    allocateAndSetPointsBuffer(points);
    allocateAndGenerateTextureBuffer(points.size());
}

void TextureBasedPolylines::allocateAndSetPointsBuffer(
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

void TextureBasedPolylines::allocateAndGenerateTextureBuffer(uint pointSize)
{
    uint16_t Y_Segments = ((pointSize - 1) * 5) / (mMaxTextureSize + 1);
    uint16_t X_Segments =
        Y_Segments == 0 ? ((pointSize - 1) * 5) : mMaxTextureSize;

    mSegmentsTexture.create(
        X_Segments,
        Y_Segments + 1,
        bgfx::TextureFormat::RGBA32F,
        BGFX_TEXTURE_COMPUTE_WRITE);

    // Joints texture is valid only if there are more than 2 points
    if (pointSize > 2) {
        uint16_t Y_Joins = ((pointSize - 2) * 4) / (mMaxTextureSize + 1);
        uint16_t X_Joins =
            Y_Joins == 0 ? ((pointSize - 2) * 4) : mMaxTextureSize;

        mJoinsTexture.create(
            X_Joins,
            Y_Joins + 1,
            bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE);
    }
    else {
        mJoinsTexture.destroy();
    }

    float data[] = {
                    static_cast<float>(mMaxTextureSize),
                    static_cast<float>(pointSize - 1),
                    0,
                    0};
    mIndirectData.bind(data);

    mPoints.bind(0);
    mSegmentsTexture.bind(1, bgfx::Access::Write);
    mJoinsTexture.bind(2, bgfx::Access::Write);
    mSegmentsIndirect.bind(3, bgfx::Access::Write);
    mJoinsIndirect.bind(4, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeTexturePH, pointSize - 1, 1, 1);

    // now, bind uniform for draw
    // TODO - this should be the same of before..........
    float indirectData[] = {
                            static_cast<float>(pointSize - 1),
                            static_cast<float>(mMaxTextureSize),
                            0,
                            0};
    mIndirectData.bind(indirectData);
}

} // namespace vcl::lines
