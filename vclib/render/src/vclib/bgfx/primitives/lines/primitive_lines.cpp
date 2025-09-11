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
    const std::vector<uint> indices;
    setPoints(vertCoords, indices, vertNormals, vertColors, lineColors);
}

void PrimitiveLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    reinitBuffers(OWNED); // we own the buffers now

    assert(vertCoords.size() % 3 == 0);
    assert(lineIndices.size() % 2 == 0);

    const bool setLineIndices = lineIndices.size() != 0;
    const bool setColors      = vertColors.size() != 0;
    const bool setNormals     = vertNormals.size() != 0;
    const bool setLineColors  = lineColors.size() != 0;

    const uint numVertices = vertCoords.size() / 3;
    const uint numLines =
        setLineIndices ? lineIndices.size() / 2 : numVertices / 2;
    const uint numElements = setLineIndices ? lineIndices.size() : numVertices;

    assert(!setColors || vertCoords.size() == vertColors.size() * 3);
    assert(!setNormals || vertCoords.size() == vertNormals.size());
    assert(!setLineColors || vertColors.size() == lineColors.size() * 2);

    if (numElements > 1) {
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

        if (setLineIndices) {
            std::get<OWNED>(mIndices).create(
                bgfx::makeRef(
                    lineIndices.data(), sizeof(uint32_t) * lineIndices.size()),
                BGFX_BUFFER_INDEX32);
        }

        if (setNormals) {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float, true)
                .end();

            std::get<OWNED>(mVertexNormals)
                .create(
                    bgfx::makeRef(
                        vertNormals.data(), sizeof(float) * vertNormals.size()),
                    layout);
        }

        if (setColors) {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

            std::get<OWNED>(mVertexColors)
                .create(
                    bgfx::makeRef(
                        vertColors.data(),
                        sizeof(uint32_t) * vertColors.size()),
                    layout);
        }

        if (setLineColors) {
            std::vector<uint32_t> expandedLineColors;
            expandedLineColors.reserve(numLines * 2);

            if (setLineIndices) {
                expandedLineColors.resize(numVertices);
                for (uint i = 0; i < numLines; ++i) {
                    const uint32_t color       = lineColors[i];
                    const uint     index1      = lineIndices[i * 2 + 0];
                    const uint     index2      = lineIndices[i * 2 + 1];
                    expandedLineColors[index1] = color;
                    expandedLineColors[index2] = color;
                }
            }
            else {
                for (const auto& color : lineColors) {
                    expandedLineColors.push_back(color);
                    expandedLineColors.push_back(color);
                }
            }

            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
                .end();

            // TODO: change to IndexBuffer
            std::get<OWNED>(mLineColors)
                .create(
                    bgfx::makeRef(
                        expandedLineColors.data(),
                        sizeof(uint32_t) * expandedLineColors.size()),
                    layout);
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

void PrimitiveLines::draw(uint viewId) const
{
    const VertexBuffer& vcoords  = mOwnsBuffers ?
                                       std::get<OWNED>(mVertexCoords) :
                                       *std::get<NOT_OWNED>(mVertexCoords);
    const VertexBuffer& vnormals = mOwnsBuffers ?
                                       std::get<OWNED>(mVertexNormals) :
                                       *std::get<NOT_OWNED>(mVertexNormals);
    const VertexBuffer& vcolors  = mOwnsBuffers ?
                                       std::get<OWNED>(mVertexColors) :
                                       *std::get<NOT_OWNED>(mVertexColors);
    const VertexBuffer& lcolors  = mOwnsBuffers ? // TODO: change to IndexBuffer
                                      std::get<OWNED>(mLineColors) :
                                       *std::get<NOT_OWNED>(mLineColors);
    const IndexBuffer&  inds     = mOwnsBuffers ? std::get<OWNED>(mIndices) :
                                                  *std::get<NOT_OWNED>(mIndices);

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
        mLineColors    = VertexBuffer(); // TODO: change to IndexBuffer
        mIndices       = IndexBuffer();
    }
    else {
        mVertexCoords  = static_cast<const VertexBuffer*>(nullptr);
        mVertexNormals = static_cast<const VertexBuffer*>(nullptr);
        mVertexColors  = static_cast<const VertexBuffer*>(nullptr);
        mLineColors    = static_cast<const VertexBuffer*>(
            nullptr); // TODO: change to IndexBuffer
        mIndices = static_cast<const IndexBuffer*>(nullptr);
    }
}

} // namespace vcl::detail
