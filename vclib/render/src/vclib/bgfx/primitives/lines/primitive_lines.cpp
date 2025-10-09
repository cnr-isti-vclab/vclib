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

#include <vclib/bgfx/primitives/lines/primitive_lines.h>

#include <vclib/bgfx/primitives/lines/lines_utils.h>

namespace vcl::detail {

PrimitiveLines::PrimitiveLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(vertCoords, vertNormals, vertColors, lineColors);
}

PrimitiveLines::PrimitiveLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

PrimitiveLines::PrimitiveLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors)
{
    setPoints(
        pointsSize, vertexCoords, vertexNormals, vertexColors, lineColors);
}

PrimitiveLines::PrimitiveLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors)
{
    setPoints(
        pointsSize,
        vertexCoords,
        lineIndices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void PrimitiveLines::swap(PrimitiveLines& other)
{
    using std::swap;

    swap(mVertexCoords, other.mVertexCoords);
    swap(mVertexNormals, other.mVertexNormals);
    swap(mVertexColors, other.mVertexColors);
    swap(mLineColors, other.mLineColors);
    swap(mIndices, other.mIndices);
}

void PrimitiveLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    const std::vector<uint> lineIndices;
    setPoints(
        false, vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void PrimitiveLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(
        true, vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void PrimitiveLines::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors)
{
    IndexBuffer indices;
    setPoints(
        pointsSize,
        vertexCoords,
        indices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void PrimitiveLines::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer& lineColors)
{
    reinitBuffers(NOT_OWNED);
    if (vertexCoords.isValid())
        std::get<NOT_OWNED>(mVertexCoords) = &vertexCoords;
    if (vertexNormals.isValid())
        std::get<NOT_OWNED>(mVertexNormals) = &vertexNormals;
    if (vertexColors.isValid())
        std::get<NOT_OWNED>(mVertexColors) = &vertexColors;
    if (lineColors.isValid())
        std::get<NOT_OWNED>(mLineColors) = &lineColors;
    if (lineIndices.isValid())
        std::get<NOT_OWNED>(mIndices) = &lineIndices;
}

void PrimitiveLines::draw(uint viewId) const
{
    if (mOwnsBuffers) {
        const VertexBuffer& vcoords  = std::get<OWNED>(mVertexCoords);
        const VertexBuffer& vnormals = std::get<OWNED>(mVertexNormals);
        const VertexBuffer& vcolors  = std::get<OWNED>(mVertexColors);
        const IndexBuffer& lcolors   = std::get<OWNED>(mLineColors);
        const IndexBuffer& inds      = std::get<OWNED>(mIndices);

        if (vcoords.isValid())
            vcoords.bind(0);
        if (vnormals.isValid())
            vnormals.bind(1);
        if (vcolors.isValid())
            vcolors.bind(2);
        if (lcolors.isValid())
            lcolors.bind(3);
        if (inds.isValid())
            inds.bind();
    }
    else {
        const VertexBuffer* vcoords  = std::get<NOT_OWNED>(mVertexCoords);
        const VertexBuffer* vnormals = std::get<NOT_OWNED>(mVertexNormals);
        const VertexBuffer* vcolors  = std::get<NOT_OWNED>(mVertexColors);
        const IndexBuffer* lcolors   = std::get<NOT_OWNED>(mLineColors);
        const IndexBuffer* inds      = std::get<NOT_OWNED>(mIndices);

        if (vcoords && vcoords->isValid())
            vcoords->bind(0);
        if (vnormals && vnormals->isValid())
            vnormals->bind(1);
        if (vcolors && vcolors->isValid())
            vcolors->bind(2);
        if (lcolors && lcolors->isValid())
            lcolors->bind(3);
        if (inds && inds->isValid())
            inds->bind();
    }

    bgfx::setState(linesDrawState() | BGFX_STATE_PT_LINES);
    bgfx::submit(viewId, mLinesPH);
}

void PrimitiveLines::reinitBuffers(Ownership owned)
{
    mOwnsBuffers = (owned == OWNED);

    if (mOwnsBuffers) {
        mVertexCoords  = VertexBuffer();
        mVertexNormals = VertexBuffer();
        mVertexColors  = VertexBuffer();
        mLineColors    = IndexBuffer();
        mIndices       = IndexBuffer();
    }
    else {
        mVertexCoords  = static_cast<const VertexBuffer*>(nullptr);
        mVertexNormals = static_cast<const VertexBuffer*>(nullptr);
        mVertexColors  = static_cast<const VertexBuffer*>(nullptr);
        mLineColors    = static_cast<const IndexBuffer*>(nullptr);
        mIndices       = static_cast<const IndexBuffer*>(nullptr);
    }
}

void PrimitiveLines::setPoints(
    bool                      setLineIndices,
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    reinitBuffers(OWNED); // we own the buffers now

    assert(vertCoords.size() % 3 == 0);
    assert(lineIndices.size() % 2 == 0);

    const bool setColors     = vertColors.size() != 0;
    const bool setNormals    = vertNormals.size() != 0;
    const bool setLineColors = lineColors.size() != 0;

    const uint numVertices = vertCoords.size() / 3;
    const uint numLines =
        setLineIndices ? lineIndices.size() / 2 : numVertices / 2;
    const uint numElements = setLineIndices ? lineIndices.size() : numVertices;

    assert(!setColors || vertCoords.size() == vertColors.size() * 3);
    assert(!setNormals || vertCoords.size() == vertNormals.size());
    assert(!setLineColors || vertColors.size() == lineColors.size() * 2);


    if (numElements > 1) {
        auto [vNormals, vNormalsReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<float>(vertCoords.size());

        auto [vColors, vColorsReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<uint>(numVertices);

        if (!setNormals) {
            for (uint i = 0; i < vertCoords.size(); i ++)
                vNormals[i] = 0.0f;
        } else {
            std::copy(
                vertNormals.begin(), vertNormals.end(), vNormals);
        }

        if (!setColors) {
            for (uint i = 0; i < numVertices; ++i) 
                vColors[i] = 0xffffffff;
        } else {
            std::copy(
                vertColors.begin(), vertColors.end(), vColors);
        }

        {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .end();

            std::get<OWNED>(mVertexCoords)
                .create(
                    bgfx::makeRef(
                        vertCoords.data(), sizeof(float) * vertCoords.size()),
                    layout);
        }

        {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float, true)
                .end();

            std::get<OWNED>(mVertexNormals)
                .create(
                    bgfx::makeRef(
                        vNormals, 
                        sizeof(float) * vertCoords.size(), 
                        vNormalsReleaseFn),
                    layout);
        }

        {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

            std::get<OWNED>(mVertexColors)
                .create(
                    bgfx::makeRef(
                        vColors,
                        sizeof(uint32_t) * numVertices,
                        vColorsReleaseFn),
                    layout);
        }

        if (setLineIndices) {
            std::get<OWNED>(mIndices).create(
                bgfx::makeRef(
                    lineIndices.data(), sizeof(uint32_t) * lineIndices.size()),
                BGFX_BUFFER_INDEX32);
        }

        if (setLineColors) { 
            std::get<OWNED>(mLineColors)
                .create(
                    bgfx::makeRef(
                        lineColors.data(),
                        sizeof(uint32_t) * lineColors.size()),
                    BGFX_BUFFER_INDEX32);
        }
    }
    else {
        std::get<OWNED>(mVertexCoords).destroy();
        std::get<OWNED>(mIndices).destroy();
        std::get<OWNED>(mVertexNormals).destroy();
        std::get<OWNED>(mVertexColors).destroy();
        std::get<OWNED>(mLineColors).destroy();
    }
}

} // namespace vcl::detail
