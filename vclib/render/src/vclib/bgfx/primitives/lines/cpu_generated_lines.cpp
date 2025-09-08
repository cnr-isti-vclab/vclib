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

#include <vclib/bgfx/primitives/lines/cpu_generated_lines.h>

#include <vclib/bgfx/primitives/lines/lines_utils.h>

namespace vcl::detail {

CPUGeneratedLines::CPUGeneratedLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(vertCoords, vertNormals, vertColors, lineColors);
}

CPUGeneratedLines::CPUGeneratedLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void CPUGeneratedLines::swap(CPUGeneratedLines& other)
{
    using std::swap;

    swap(mVertexCoords, other.mVertexCoords);
    swap(mVertexNormals, other.mVertexNormals);
    swap(mVertexColors, other.mVertexColors);
    swap(mLineColors, other.mLineColors);
    swap(mIndices, other.mIndices);
}

void CPUGeneratedLines::setPoints(
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

    const uint nPoints =
        setLineIndices ? lineIndices.size() : vertCoords.size() / 3;

    assert(!setColors || vertCoords.size() == vertColors.size() * 3);
    assert(!setNormals || vertCoords.size() == vertNormals.size());
    assert(!setLineColors || vertColors.size() == lineColors.size() * 2);

    if (nPoints > 1) {
        uint bufferVertCoordsSize  = (nPoints / 2) * 4 * 6;
        uint bufferVertColorsSize  = (nPoints / 2) * 4 * 2;
        uint bufferVertNormalsSize = (nPoints / 2) * 4 * 6;
        uint bufferLineColorsSize  = (nPoints / 2) * 4 * 1;

        uint bufferIndsSize = (nPoints / 2) * 6;

        auto [vCoords, vCoordsReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<float>(bufferVertCoordsSize);

        auto [vColors, vColorsReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<float>(bufferVertColorsSize);

        auto [vNormals, vNormalsReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<float>(bufferVertNormalsSize);

        auto [lColors, lColorsReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<float>(bufferLineColorsSize);

        auto [indices, iReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<uint>(bufferIndsSize);

        uint viCoords     = 0;
        uint viColors     = 0;
        uint viNormals    = 0;
        uint viLineColors = 0;

        uint ii = 0;
        for (uint i = 0; i < nPoints - 1; i += 2) {
            uint index0 = setLineIndices ? lineIndices[i] : i;
            uint index1 = setLineIndices ? lineIndices[i + 1] : i + 1;

            for (uint k = 0; k < 2; k++) {
                for (uint j = 0; j < 2; j++) {
                    vCoords[viCoords++] = vertCoords[(index0 * 3)];
                    vCoords[viCoords++] = vertCoords[(index0 * 3) + 1];
                    vCoords[viCoords++] = vertCoords[(index0 * 3) + 2];

                    vCoords[viCoords++] = vertCoords[(index1 * 3)];
                    vCoords[viCoords++] = vertCoords[(index1 * 3) + 1];
                    vCoords[viCoords++] = vertCoords[(index1 * 3) + 2];

                    if (setColors) {
                        vColors[viColors++] =
                            std::bit_cast<float>(vertColors[index0]);

                        vColors[viColors++] =
                            std::bit_cast<float>(vertColors[index1]);
                    }

                    if (setNormals) {
                        vNormals[viNormals++] = vertNormals[(index0 * 3)];
                        vNormals[viNormals++] = vertNormals[(index0 * 3) + 1];
                        vNormals[viNormals++] = vertNormals[(index0 * 3) + 2];

                        vNormals[viNormals++] = vertNormals[(index1 * 3)];
                        vNormals[viNormals++] = vertNormals[(index1 * 3) + 1];
                        vNormals[viNormals++] = vertNormals[(index1 * 3) + 2];
                    }

                    if (setLineColors) {
                        lColors[viLineColors++] =
                            std::bit_cast<float>(lineColors[i / 2]);
                    }
                }
            }

            uint index    = (4 * (i / 2));
            indices[ii++] = index;
            indices[ii++] = index + 3;
            indices[ii++] = index + 1;

            indices[ii++] = index;
            indices[ii++] = index + 2;
            indices[ii++] = index + 3;
        }

        {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
                .end();

            mVertexCoords.create(
                bgfx::makeRef(
                    vCoords,
                    sizeof(float) * bufferVertCoordsSize,
                    vCoordsReleaseFn),
                layout);
        }

        if (setColors) {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
                .end();

            mVertexColors.create(
                bgfx::makeRef(
                    vColors,
                    sizeof(float) * bufferVertColorsSize,
                    vColorsReleaseFn),
                layout);
        }

        if (setNormals) {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
                .end();

            mVertexNormals.create(
                bgfx::makeRef(
                    vNormals,
                    sizeof(float) * bufferVertNormalsSize,
                    vNormalsReleaseFn),
                layout);
        }

        if (setLineColors) {
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Color2, 4, bgfx::AttribType::Uint8, true)
                .end();

            mLineColors.create(
                bgfx::makeRef(
                    lColors,
                    sizeof(float) * bufferLineColorsSize,
                    lColorsReleaseFn),
                layout);
        }

        mIndices.create(
            bgfx::makeRef(indices, sizeof(uint) * bufferIndsSize, iReleaseFn),
            BGFX_BUFFER_INDEX32);
    }
    else {
        mVertexCoords.destroy();
        mVertexNormals.destroy();
        mVertexColors.destroy();
        mLineColors.destroy();
        mIndices.destroy();
    }
}

void CPUGeneratedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    const std::vector<uint> indices;
    setPoints(vertCoords, indices, vertNormals, vertColors, lineColors);
}

void CPUGeneratedLines::draw(uint viewId) const
{
    mVertexCoords.bind(0);
    mVertexColors.bind(1);
    mVertexNormals.bind(2);
    mLineColors.bind(3);
    mIndices.bind();
    bgfx::setState(linesDrawState());
    bgfx::submit(viewId, mLinesPH);
}

} // namespace vcl::detail
