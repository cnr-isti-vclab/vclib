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

#include <vclib/bgfx/primitives/lines/gpu_generated_lines.h>

namespace vcl::detail {

GPUGeneratedLines::GPUGeneratedLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(vertCoords, vertNormals, vertColors, lineColors);
}

GPUGeneratedLines::GPUGeneratedLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

GPUGeneratedLines::GPUGeneratedLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(
        pointsSize, vertexCoords, vertexNormals, vertexColors, lineColors);
}

GPUGeneratedLines::GPUGeneratedLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(
        pointsSize,
        vertexCoords,
        lineIndices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void GPUGeneratedLines::swap(GPUGeneratedLines& other)
{
    using std::swap;

    swap(mCustomIndicesUH, other.mCustomIndicesUH);

    swap(mVertexCoords, other.mVertexCoords);
    swap(mVertexNormals, other.mVertexNormals);
    swap(mVertexColors, other.mVertexColors);
    swap(mLineColors, other.mLineColors);
    swap(mLineIndices, other.mLineIndices);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
}

void GPUGeneratedLines::draw(uint viewId) const
{
    mVertices.bind(0);
    mLineColors.bind(0);
    mIndices.bind();
    bgfx::setState(linesDrawState());
    bgfx::submit(viewId, mLinesPH);
}

void GPUGeneratedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    std::vector<uint> emptyIndices;
    setPoints(
        false, vertCoords, emptyIndices, vertNormals, vertColors, lineColors);
}

void GPUGeneratedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(
        true, vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void GPUGeneratedLines::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors)
{
    IndexBuffer emptyIndices;
    setPoints(
        pointsSize,
        vertexCoords,
        emptyIndices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void GPUGeneratedLines::setPoints(
    const uint          nPoints,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors)
{
    // TODO: handle the lineColors copy in mLineColors
    if (nPoints > 1) {
        allocateVertexAndIndexBuffer(nPoints);
        generateVertexAndIndexBuffer(
            nPoints,
            vertexCoords,
            lineIndices,
            vertexNormals,
            vertexColors);
    }
    else {
        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();
        mLineIndices.destroy();

        mVertices.destroy();
        mIndices.destroy();
    }
}

void GPUGeneratedLines::setPoints(
    bool                      setLineIndices,
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    assert(vertCoords.size() % 3 == 0);
    assert(lineIndices.size() % 2 == 0);

    const bool setColors     = vertColors.size() != 0;
    const bool setNormals    = vertNormals.size() != 0;
    const bool setLineColors = lineColors.size() != 0;

    const uint nPoints =
        setLineIndices ? lineIndices.size() : vertCoords.size() / 3;

    assert(!setColors || vertCoords.size() == vertColors.size() * 3);
    assert(!setNormals || vertCoords.size() == vertNormals.size());
    assert(!setLineColors || vertColors.size() == lineColors.size() * 2);

    if (nPoints > 1) {
        allocateVertexCoords(vertCoords);

        if (setLineIndices)
            allocateLineIndices(lineIndices);

        allocateVertexNormals(nPoints, vertNormals);
        // allocateVertexColors(nPoints, vertColors);
        allocateVertexLineColors(lineColors);

        allocateVertexAndIndexBuffer(nPoints);
        generateVertexAndIndexBuffer(
            nPoints,
            mVertexCoords,
            mLineIndices,
            mVertexNormals,
            mVertexColors);
    }
    else {
        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();
        mLineIndices.destroy();

        mVertices.destroy();
        mIndices.destroy();
    }
}

void GPUGeneratedLines::allocateVertexCoords(
    const std::vector<float>& vertCoords)
{
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<float>(vertCoords.size());

    std::copy(vertCoords.begin(), vertCoords.end(), buffer);

    mVertexCoords.createForCompute(
        buffer,
        vertCoords.size() / 3,
        bgfx::Attrib::Position,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn);
}

void GPUGeneratedLines::allocateLineIndices(
    const std::vector<uint>& lineIndices)
{
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(lineIndices.size());

    std::copy(lineIndices.begin(), lineIndices.end(), buffer);

    mLineIndices.createForCompute(
        buffer,
        lineIndices.size(),
        PrimitiveType::UINT,
        bgfx::Access::Read,
        releaseFn);
}

void GPUGeneratedLines::allocateVertexNormals(
    const uint nPoints,
    const std::vector<float>& vertNormals)
{
    const uint bufferSize = vertNormals.size() == 0 ? nPoints * 3 : vertNormals.size();
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<float>(bufferSize);

    if (vertNormals.size() == 0) {
        for (uint i = 0; i < bufferSize; ++i)
            buffer[i] = 0.0f;
    } else {
        std::copy(vertNormals.begin(), vertNormals.end(), buffer);
    }

    mVertexNormals.createForCompute(
        buffer,
        bufferSize / 3,
        bgfx::Attrib::Normal,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn);
}

void GPUGeneratedLines::allocateVertexColors(
    const uint nPoints,
    const std::vector<uint>& vertColors)
{
    const uint bufferSize = vertColors.size() == 0 ? nPoints : vertColors.size();
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(bufferSize);

    if (vertColors.size() == 0) {
        for (uint i = 0; i < bufferSize; ++i)
            buffer[i] = 0xffffffff;
    } else {
        std::copy(vertColors.begin(), vertColors.end(), buffer);
    }

    mVertexColors.createForCompute(
        buffer,
        bufferSize,
        bgfx::Attrib::Color0,
        4,
        PrimitiveType::UCHAR,
        true,
        bgfx::Access::Read,
        releaseFn);
}

void GPUGeneratedLines::allocateVertexLineColors(
    const std::vector<uint>& lineColors)
{
    mLineColors.createForCompute(
        lineColors.data(),
        lineColors.size(),
        PrimitiveType::UINT,
        bgfx::Access::Read
    );
}

void GPUGeneratedLines::allocateVertexAndIndexBuffer(const uint pointsSize)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float) // padding
        .end();

    const uint szv = (pointsSize / 2) * 4 * 16;

    mVertices.create(
        bgfx::makeRef(nullptr, sizeof(float) * szv),
        layout,
        BGFX_BUFFER_COMPUTE_WRITE,
        true);

    const uint szi = (pointsSize / 2) * 6;

    mIndices.create(
        bgfx::makeRef(nullptr, sizeof(uint) * szi),
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32,
        true);
}

void GPUGeneratedLines::generateVertexAndIndexBuffer(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors)
{
    float data[] = {
        static_cast<float>(lineIndices.isValid()),
        static_cast<float>(vertexNormals.isValid()),
        static_cast<float>(vertexColors.isValid()), 
        0.0f};

    mCustomIndicesUH.bind(data);
    vertexCoords.bindCompute(0, bgfx::Access::Read);

    if (lineIndices.isValid())
        lineIndices.bind(1);

    if (vertexNormals.isValid())
        vertexNormals.bind(2);

    if (vertexColors.isValid())
        vertexColors.bind(3);

    mVertices.bind(4, bgfx::Access::Write);
    mIndices.bind(5, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeVerticesPH, (pointsSize / 2), 1, 1);

    mVertices.setCompute(false);
    mIndices.setCompute(false);
}

} // namespace vcl::detail
