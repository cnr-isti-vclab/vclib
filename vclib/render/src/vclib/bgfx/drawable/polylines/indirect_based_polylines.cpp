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

#include <vclib/bgfx/drawable/polylines/indirect_based_polylines.h>

namespace vcl {

IndirectBasedPolylines::IndirectBasedPolylines()
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

IndirectBasedPolylines::IndirectBasedPolylines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals) : IndirectBasedPolylines()
{
    setPoints(vertCoords, vertColors, vertNormals);
}

void IndirectBasedPolylines::swap(IndirectBasedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);

    swap(mIndirectData, other.mIndirectData);

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);

    swap(mInstanceData, other.mInstanceData);
    swap(mNumPoints, other.mNumPoints);
}

void IndirectBasedPolylines::draw(uint viewId) const
{
    bindSettingsUniform();

    mVertices.bind(0);
    mIndices.bind();
    mInstanceData.setInstance(0, mNumPoints - 1);

    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);

    if (settings().getJoint() != PolyLineJoint::ROUND_JOINT) {
        mVertices.bind(0);
        mIndices.bind();
        mInstanceData.setInstance(1, mNumPoints - 1);

        bgfx::setState(drawState());
        bgfx::submit(viewId, mJointsPH);
    }
}

void IndirectBasedPolylines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    mNumPoints = vertCoords.size() / 3;

    setCoordsBuffers(vertCoords);   
    setColorsBuffers(vertColors);
    setNormalsBuffers(vertNormals);

    allocateInstanceData();
    generateInstanceDataBuffers();
}

void IndirectBasedPolylines::setCoordsBuffers(const std::vector<float>& vertCoords)
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

void IndirectBasedPolylines::setColorsBuffers(const std::vector<uint>& vertColors)
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

void IndirectBasedPolylines::setNormalsBuffers(const std::vector<float>& vertNormals)
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

void IndirectBasedPolylines::allocateInstanceData() 
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord4, 4, bgfx::AttribType::Float)
        .end();

    const uint sz = (mNumPoints - 1) * 20;

    mInstanceData.create(
        bgfx::makeRef(nullptr, sizeof(float) * sz),
        layout,
        BGFX_BUFFER_COMPUTE_WRITE,
        true);
}

void IndirectBasedPolylines::generateInstanceDataBuffers() 
{
    float data[] = {static_cast<float>(mNumPoints - 1), 0, 0, 0};
    mIndirectData.bind(data);
    mVertCoords.bind(0);
    mVertColors.bind(1);
    mVertNormals.bind(2);

    mInstanceData.bind(3, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeIndirectPH, mNumPoints - 1, 1, 1);
}

} // namespace vcl
