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

    swap(mComputeCoords, other.mComputeCoords);
    swap(mComputeNormals, other.mComputeNormals);
    swap(mComputeColors, other.mComputeColors);
    swap(mComputeLineIndices, other.mComputeLineIndices);

    swap(mVertexCoords, other.mVertexCoords);
    swap(mVertexNormals, other.mVertexNormals);
    swap(mVertexColors, other.mVertexColors);
    swap(mLineColors, other.mLineColors);
    swap(mIndices, other.mIndices);
}

void GPUGeneratedLines::draw(uint viewId) const
{
    mVertexCoords.bind(0);
    mVertexNormals.bind(1);
    mVertexColors.bind(2);
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
        mComputeCoords.destroy();
        mComputeNormals.destroy(); 
        mComputeColors.destroy();
        mComputeLineIndices.destroy();

        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();
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
        allocateVertexColors(nPoints, vertColors);
        allocateVertexLineColors(lineColors);

        allocateVertexAndIndexBuffer(nPoints);
        generateVertexAndIndexBuffer(
            nPoints,
            mComputeCoords,
            mComputeLineIndices,
            mComputeNormals,
            mComputeColors);
    }
    else {
        mComputeCoords.destroy();
        mComputeColors.destroy();
        mComputeNormals.destroy();
        mComputeLineIndices.destroy();

        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();
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
        4,
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

    mComputeLineIndices.createForCompute(
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

    mComputeNormals.createForCompute(
        buffer,
        bufferSize / 3,
        bgfx::Attrib::Normal,
        4,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn);
}

void GPUGeneratedLines::allocateVertexColors(
    const uint nPoints,
    const std::vector<uint>& vertColors)
{
    const uint bufferSize = std::max(nPoints, 4u);
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(bufferSize);

    for (size_t i = 0; i < vertColors.size(); i++)
        buffer[i] = Color(vertColors[i], Color::Format::ABGR).abgr();

    for (uint i = vertColors.size(); i < bufferSize; ++i)
        buffer[i] = Color(0xffffffff, Color::Format::ABGR).abgr();

    mComputeColors.createForCompute(
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
    const uint numLines = pointsSize / 2;

    {
        bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
                .end();

        mVertexCoords.create(
            bgfx::makeRef(nullptr, sizeof(float) * numLines * 4 * 6),
            layout,
            BGFX_BUFFER_COMPUTE_WRITE,
            true);
    }

    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
            .end();

        mVertexColors.create(
            bgfx::makeRef(nullptr, sizeof(uint) * numLines * 4 * 2),
            layout);
    }

    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .end();

        mVertexNormals.create(
            bgfx::makeRef(nullptr, sizeof(float) * numLines * 4 * 6),
            layout);
    }

    mIndices.create(
        bgfx::makeRef(nullptr, sizeof(uint) * numLines * 6),
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
        static_cast<float>(pointsSize / 2)
    };

    mCustomIndicesUH.bind(data);

    vertexCoords.bind(0, bgfx::Access::Read);

    if (lineIndices.isValid())
        lineIndices.bind(1);

    if (vertexNormals.isValid())
        vertexNormals.bind(2);

    if (vertexColors.isValid())
        vertexColors.bind(3);

    mVertexCoords.bind(4, bgfx::Access::Write);
    mVertexNormals.bind(5, bgfx::Access::Write);
    mVertexColors.bind(6, bgfx::Access::Write);
    mIndices.bind(7, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeVerticesPH, std::ceil((pointsSize / 2) / 512), 1, 1);

    mVertexCoords.setCompute(false);
    mVertexNormals.setCompute(false);
    mVertexColors.setCompute(false);
    mIndices.setCompute(false);
}

} // namespace vcl::detail
