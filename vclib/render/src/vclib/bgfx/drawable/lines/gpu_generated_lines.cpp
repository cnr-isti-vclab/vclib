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

#include <vclib/bgfx/drawable/lines/gpu_generated_lines.h>

namespace vcl {

GPUGeneratedLines::GPUGeneratedLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    checkCaps();
    assert(bgfx::isValid(mComputeVerticesPH));

    setPoints(vertCoords, vertColors, vertNormals);
}

void GPUGeneratedLines::swap(GPUGeneratedLines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
}

void GPUGeneratedLines::draw(uint viewId) const
{
    bindSettingsUniform();

    mVertices.bind(0);
    mIndices.bind();
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);
}

void GPUGeneratedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    const uint nPoints = vertCoords.size() / 3;
    if (nPoints > 1) {
        setCoordsBuffers(vertCoords);
        setColorsBuffers(vertColors);
        setNormalsBuffers(vertNormals);

        allocateVertexBuffer(nPoints);
        allocateIndexBuffer(nPoints);
        // buffers are set for the compute stage
        generateVerticesAndIndicesBuffers(nPoints);
        // here buffers are computed and ready for rendering
    }
    else {
        mVertCoords.destroy();
        mVertColors.destroy();
        mVertNormals.destroy();
        mVertices.destroy();
        mIndices.destroy();
    }
}

void GPUGeneratedLines::setCoordsBuffers(const std::vector<float>& vertCoords)
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

void GPUGeneratedLines::setColorsBuffers(const std::vector<uint>& vertColors)
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

void GPUGeneratedLines::setNormalsBuffers(const std::vector<float>& vertNormals)
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

void GPUGeneratedLines::allocateVertexBuffer(uint pointsSize)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
        .end();

    const uint sz = (pointsSize / 2) * 4 * 12;

    mVertices.create(
        bgfx::makeRef(nullptr, sizeof(float) * sz),
        layout,
        BGFX_BUFFER_COMPUTE_READ_WRITE,
        true);
}

void GPUGeneratedLines::allocateIndexBuffer(uint pointsSize)
{
    const uint sz = (pointsSize / 2) * 6;

    mIndices.create(
        bgfx::makeRef(nullptr, sizeof(uint) * sz),
        BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32,
        true);
}

void GPUGeneratedLines::generateVerticesAndIndicesBuffers(uint pointsSize)
{
    mVertCoords.bind(0);
    mVertColors.bind(1);
    mVertNormals.bind(2);

    mVertices.bind(3, bgfx::Access::Write);
    mIndices.bind(4, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeVerticesPH, (pointsSize / 2), 1, 1);
    // after the dispatch, the vert and indices buffers are ready to be used in
    // the rendering pipeline
    mVertices.setCompute(false);
    mIndices.setCompute(false);
}

} // namespace vcl
