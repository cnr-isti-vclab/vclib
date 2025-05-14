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

#include <vclib/bgfx/drawable/polylines/texture_based_polylines.h>

namespace vcl {

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
}

TextureBasedPolylines::TextureBasedPolylines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals) : TextureBasedPolylines()
{
    setPoints(vertCoords, vertColors, vertNormals);
}

void TextureBasedPolylines::swap(TextureBasedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);

    swap(mNumPoints, other.mNumPoints);

    swap(mTexture, other.mTexture);
    swap(mIndirectData, other.mIndirectData);
}

void TextureBasedPolylines::draw(uint viewId) const
{
    if (mVertices.isValid()) {
        bindSettingsUniform();

        mVertices.bind(0);
        mIndices.bind();
        mTexture.bind(0, bgfx::Access::Read);
        bgfx::setInstanceCount(mNumPoints - 1);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        // mJointsTexture is valid only if there are more than 2 points
        if (settings().getJoint() != PolyLineJoint::ROUND_JOINT) {
            mVertices.bind(0);
            mIndices.bind();
            mTexture.bind(0, bgfx::Access::Read);
            bgfx::setInstanceCount(mNumPoints - 2);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mJointsPH);
        }
    }
}

void TextureBasedPolylines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    mNumPoints = vertCoords.size() / 3;
    if (mNumPoints > 1) {
        setCoordsBuffers(vertCoords);
        setColorsBuffers(vertColors);
        setNormalsBuffers(vertNormals);
        
        allocateAndGenerateTextureBuffer();
    }
    else {
        mVertCoords.destroy();
        mVertColors.destroy();
        mVertNormals.destroy();

        mVertices.destroy();
        mIndices.destroy();
        mTexture.destroy();
    }
}

void TextureBasedPolylines::setCoordsBuffers(const std::vector<float>& vertCoords)
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

void TextureBasedPolylines::setColorsBuffers(const std::vector<uint>& vertColors)
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

void TextureBasedPolylines::setNormalsBuffers(const std::vector<float>& vertNormals)
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

void TextureBasedPolylines::allocateAndGenerateTextureBuffer()
{
    uint16_t Y_Segments = ((mNumPoints - 1) * 5) / (mMaxTextureSize + 1);
    uint16_t X_Segments =
        Y_Segments == 0 ? ((mNumPoints - 1) * 5) : mMaxTextureSize;

    mTexture.create(
        X_Segments,
        Y_Segments + 1,
        bgfx::TextureFormat::RGBA32F,
        BGFX_TEXTURE_COMPUTE_WRITE);

    float data[] = {
                    static_cast<float>(mMaxTextureSize),
                    static_cast<float>(mNumPoints - 1),
                    0,
                    0};
    mIndirectData.bind(data);

    mVertCoords.bind(0);
    mVertColors.bind(1);
    mVertNormals.bind(2);

    mTexture.bind(3, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeTexturePH, mNumPoints - 1, 1, 1);

    // now, bind uniform for draw
    // TODO - this should be the same of before..........
    float indirectData[] = {
                            static_cast<float>(mNumPoints - 1),
                            static_cast<float>(mMaxTextureSize),
                            0,
                            0};
    mIndirectData.bind(indirectData);
}

} // namespace vcl
