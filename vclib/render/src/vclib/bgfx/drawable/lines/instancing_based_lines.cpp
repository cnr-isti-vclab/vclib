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

#include <vclib/bgfx/drawable/lines/instancing_based_lines.h>

namespace vcl {

InstancingBasedLines::InstancingBasedLines()
{
    checkCaps();

    mVertices.create(
        VERTICES.data(),
        VERTICES.size(),
        bgfx::Attrib::Position,
        2,
        PrimitiveType::FLOAT);

    mIndices.create(INDICES.data(), INDICES.size());
}

InstancingBasedLines::InstancingBasedLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals,
    const std::vector<uint>& lineColors) : InstancingBasedLines()
{
    setPoints(vertCoords, vertColors, vertNormals, lineColors);
}

void InstancingBasedLines::swap(InstancingBasedLines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);
    swap(mLineColors, other.mLineColors);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
    swap(mInstanceDB, other.mInstanceDB);
}

void InstancingBasedLines::draw(uint viewId) const
{
    if ((mVertCoords.size() / 3) > 1) {
        bindSettingsUniform();

        generateInstanceDataBuffer();

        mVertices.bind(0);
        mIndices.bind();
        bgfx::setInstanceDataBuffer(&mInstanceDB);

        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }
}

void InstancingBasedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals,
    const std::vector<uint>& lineColors)
{
    mVertCoords = vertCoords;

    if (!vertColors.empty())
        mVertColors = vertColors;

    if (!vertNormals.empty())
        mVertNormals = vertNormals;

    if (!lineColors.empty())
        mLineColors = lineColors;
}

void InstancingBasedLines::generateInstanceDataBuffer() const
{
    const uint nPoints = mVertCoords.size() / 3;
    const uint16_t stride = sizeof(float) * 16;
    uint           size   = (nPoints / 2);

    uint linesNum = bgfx::getAvailInstanceDataBuffer(size, stride);
    bgfx::allocInstanceDataBuffer(&mInstanceDB, size, stride);

    uint8_t* data = mInstanceDB.data;
    for (uint i = 1; i < nPoints; i += 2) {
        float* p0 = reinterpret_cast<float*>(data);
        p0[0]     = mVertCoords[((i - 1) * 3)];
        p0[1]     = mVertCoords[((i - 1) * 3) + 1];
        p0[2]     = mVertCoords[((i - 1) * 3) + 2];

        uint* color0 = reinterpret_cast<uint*>(&data[12]);
        color0[0]    = mVertColors[i - 1];

        float* p1 = reinterpret_cast<float*>(&data[16]);
        p1[0]     = mVertCoords[(i * 3)];
        p1[1]     = mVertCoords[(i * 3) + 1];
        p1[2]     = mVertCoords[(i * 3) + 2];

        uint* color1 = reinterpret_cast<uint*>(&data[28]);
        color1[0]    = mVertColors[i];

        float* n0 = reinterpret_cast<float*>(&data[32]);
        n0[0]     = mVertNormals[((i - 1) * 3)];
        n0[1]     = mVertNormals[((i - 1) * 3) + 1];
        n0[2]     = mVertNormals[((i - 1) * 3) + 2];

        uint* lineColor = reinterpret_cast<uint*>(&data[44]);
        lineColor[0] = mLineColors[i / 2];

        float* n1 = reinterpret_cast<float*>(&data[48]);
        n1[0]     = mVertNormals[(i * 3)];
        n1[1]     = mVertNormals[(i * 3) + 1];
        n1[2]     = mVertNormals[(i * 3) + 2];
        n1[3]     = 0;

        data += stride;
    }
}

} // namespace vcl
