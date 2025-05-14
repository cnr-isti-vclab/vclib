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

#include <vclib/bgfx/drawable/polylines/instancing_based_polylines.h>

namespace vcl {

InstancingBasedPolylines::InstancingBasedPolylines()
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

InstancingBasedPolylines::InstancingBasedPolylines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals) : InstancingBasedPolylines()
{
    setPoints(vertCoords, vertColors, vertNormals);
}

void InstancingBasedPolylines::swap(InstancingBasedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
    swap(mInstanceDB, other.mInstanceDB);
}

void InstancingBasedPolylines::draw(uint viewId) const
{
    const uint nPoints = mVertCoords.size() / 3;
    if (nPoints > 1) {
        bindSettingsUniform();

        generateInstanceBuffer();

        mVertices.bind(0);
        mIndices.bind();
        bgfx::setInstanceDataBuffer(&mInstanceDB, 0, nPoints - 1);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (settings().getJoint() != PolyLineJoint::ROUND_JOINT) {
            mVertices.bind(0);
            mIndices.bind();
            bgfx::setInstanceDataBuffer(&mInstanceDB, 1, nPoints - 1);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mJointsPH);
        }
    }
}

void InstancingBasedPolylines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    mVertCoords = vertCoords;
    mVertColors = vertColors;
    mVertNormals = vertNormals;
}

void InstancingBasedPolylines::generateInstanceBuffer() const
{
    const uint nPoints = mVertCoords.size() / 3;

    const uint16_t strideSegments = sizeof(float) * 20;
    uint       linesNumSegments =
        bgfx::getAvailInstanceDataBuffer(nPoints - 1, strideSegments);
    bgfx::allocInstanceDataBuffer(
        &mInstanceDB, linesNumSegments, strideSegments);

    uint8_t* dataSegments = mInstanceDB.data;

    for (uint i = 0; i < linesNumSegments; i++) {
        float* prevSegments = reinterpret_cast<float*>(dataSegments);
        prevSegments[0]     = mVertCoords[((i - !!i) * 3)];
        prevSegments[1]     = mVertCoords[((i - !!i) * 3) + 1];
        prevSegments[2]     = mVertCoords[((i - !!i) * 3) + 2];
        prevSegments[3]     = mVertNormals[(i * 3)];

        float* currSegments = reinterpret_cast<float*>(&dataSegments[16]);
        currSegments[0]     = mVertCoords[(i * 3)];
        currSegments[1]     = mVertCoords[(i * 3) + 1];
        currSegments[2]     = mVertCoords[(i * 3) + 2];

        uint* color0 = reinterpret_cast<uint*>(&dataSegments[28]);
        color0[0]           = mVertColors[i];

        float* nextSegments = reinterpret_cast<float*>(&dataSegments[32]);
        nextSegments[0]     = mVertCoords[((i + 1) * 3)];
        nextSegments[1]     = mVertCoords[((i + 1) * 3) + 1];
        nextSegments[2]     = mVertCoords[((i + 1) * 3) + 2];

        uint* color1 = reinterpret_cast<uint*>(&dataSegments[44]);
        color1[0]           = mVertColors[i + 1];

        float* next_nextSegments = reinterpret_cast<float*>(&dataSegments[48]);
        next_nextSegments[0] =
            mVertCoords[((i + 1 + (!!(linesNumSegments - 1 - i))) * 3)];
        next_nextSegments[1] =
            mVertCoords[((i + 1 + (!!(linesNumSegments - 1 - i))) * 3) + 1];
        next_nextSegments[2] =
            mVertCoords[((i + 1 + (!!(linesNumSegments - 1 - i))) * 3) + 2];
        next_nextSegments[3] = mVertNormals[(i * 3) + 1];

        float* normalSegments = reinterpret_cast<float*>(&dataSegments[64]);
        normalSegments[0]     = mVertNormals[(i * 3) + 2];
        normalSegments[1]     = mVertNormals[((i + 1) * 3)];
        normalSegments[2]     = mVertNormals[((i + 1) * 3) + 1];
        normalSegments[3]     = mVertNormals[((i + 1) * 3) + 2];

        dataSegments += strideSegments;
    }
}

} // namespace vcl
