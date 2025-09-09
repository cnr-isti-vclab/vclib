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

            mVertexCoords.create(
                bgfx::makeRef(
                    vertCoords.data(), sizeof(float) * vertCoords.size()),
                layout);
        }

        if (setLineIndices) {
            mIndices.create(
                bgfx::makeRef(
                    lineIndices.data(), sizeof(uint32_t) * lineIndices.size()),
                BGFX_BUFFER_INDEX32);
        }

        if (setNormals) {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float, true)
                .end();

            mVertexNormals.create(
                bgfx::makeRef(
                    vertNormals.data(), sizeof(float) * vertNormals.size()),
                layout);
        }

        if (setColors) {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

            mVertexColors.create(
                bgfx::makeRef(
                    vertColors.data(), sizeof(uint32_t) * vertColors.size()),
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

            mLineColors.create(
                bgfx::makeRef(
                    expandedLineColors.data(),
                    sizeof(uint32_t) * expandedLineColors.size()),
                layout);
        }
    }
    else {
        mVertexCoords.destroy();
        mIndices.destroy();
        mVertexNormals.destroy();
        mVertexColors.destroy();
        mLineColors.destroy();
    }
}

void PrimitiveLines::draw(uint viewId) const
{
    if (mVertexCoords.isValid())
        mVertexCoords.bind(0);

    if (mVertexNormals.isValid())
        mVertexNormals.bind(1);

    if (mVertexColors.isValid())
        mVertexColors.bind(2);

    if (mLineColors.isValid())
        mLineColors.bind(3);

    if (mIndices.isValid())
        mIndices.bind();

    bgfx::setState(linesDrawState() | BGFX_STATE_PT_LINES);
    bgfx::submit(viewId, mLinesPH);
}

} // namespace vcl::detail
