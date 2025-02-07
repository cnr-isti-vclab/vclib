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

#include <vclib/bgfx/context/load_program.h>
#include <vclib/bgfx/drawable/lines/polylines/instancing_based_polylines.h>

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
    const std::vector<LinesVertex>& points) : InstancingBasedPolylines()
{
    update(points);
}

void InstancingBasedPolylines::swap(InstancingBasedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mPoints, other.mPoints);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
    swap(mSegmentsInstanceDB, other.mSegmentsInstanceDB);
    swap(mJoinsInstanceDB, other.mJoinsInstanceDB);
}

void InstancingBasedPolylines::draw(uint viewId) const
{
    if (mPoints.size() > 1) {
        bindSettingsUniformPolylines();

        generateInstanceBuffer();

        mVertices.bind(0);
        mIndices.bind();
        bgfx::setInstanceDataBuffer(&mSegmentsInstanceDB);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (settings().getJoin() != PolyLineJoin::ROUND_JOIN) {
            mVertices.bind(0);
            mIndices.bind();
            bgfx::setInstanceDataBuffer(&mJoinsInstanceDB);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mJoinsPH);
        }
    }
}

void InstancingBasedPolylines::update(const std::vector<LinesVertex>& points)
{
    mPoints = points;
}

void InstancingBasedPolylines::generateInstanceBuffer() const
{
    const uint16_t strideSegments = sizeof(float) * 20;
    uint       linesNumSegments =
        bgfx::getAvailInstanceDataBuffer(mPoints.size() - 1, strideSegments);
    bgfx::allocInstanceDataBuffer(
        &mSegmentsInstanceDB, linesNumSegments, strideSegments);

    const uint16_t strideJoins = sizeof(float) * 16;
    if (mPoints.size() > 2) {
        uint linesNumJoins =
            bgfx::getAvailInstanceDataBuffer(mPoints.size() - 2, strideJoins);
        bgfx::allocInstanceDataBuffer(
            &mJoinsInstanceDB, linesNumJoins, strideJoins);
    }

    uint8_t* dataSegments = mSegmentsInstanceDB.data;
    uint8_t* dataJoins    = mJoinsInstanceDB.data;

    for (uint i = 0; i < linesNumSegments; i++) {
        float* prevSegments = reinterpret_cast<float*>(dataSegments);
        prevSegments[0]     = mPoints[i - !!i].X;
        prevSegments[1]     = mPoints[i - !!i].Y;
        prevSegments[2]     = mPoints[i - !!i].Z;
        prevSegments[3]     = mPoints[i].xN;

        float* currSegments = reinterpret_cast<float*>(&dataSegments[16]);
        currSegments[0]     = mPoints[i].X;
        currSegments[1]     = mPoints[i].Y;
        currSegments[2]     = mPoints[i].Z;

        uint* color0 = reinterpret_cast<uint*>(&dataSegments[28]);
        color0[0]        = mPoints[i].getUintColor();

        float* nextSegments = reinterpret_cast<float*>(&dataSegments[32]);
        nextSegments[0]     = mPoints[i + 1].X;
        nextSegments[1]     = mPoints[i + 1].Y;
        nextSegments[2]     = mPoints[i + 1].Z;

        uint* color1 = reinterpret_cast<uint*>(&dataSegments[44]);
        color1[0]        = mPoints[i + 1].getUintColor();

        float* next_nextSegments = reinterpret_cast<float*>(&dataSegments[48]);
        next_nextSegments[0] =
            mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].X;
        next_nextSegments[1] =
            mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].Y;
        next_nextSegments[2] =
            mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].Z;
        next_nextSegments[3] = mPoints[i].yN;

        float* normalSegments = reinterpret_cast<float*>(&dataSegments[64]);
        normalSegments[0]     = mPoints[i].zN;
        normalSegments[1]     = mPoints[i + 1].xN;
        normalSegments[2]     = mPoints[i + 1].yN;
        normalSegments[3]     = mPoints[i + 1].zN;

        if (i > 0) {
            float* prevJoin = reinterpret_cast<float*>(dataJoins);
            prevJoin[0]     = mPoints[i - 1].X;
            prevJoin[1]     = mPoints[i - 1].Y;
            prevJoin[2]     = mPoints[i - 1].Z;
            prevJoin[3]     = 0.0f;

            float* currJoin = reinterpret_cast<float*>(&dataJoins[16]);
            currJoin[0]     = mPoints[i].X;
            currJoin[1]     = mPoints[i].Y;
            currJoin[2]     = mPoints[i].Z;

            uint* colorJoin = reinterpret_cast<uint*>(&dataJoins[28]);
            colorJoin[0]        = mPoints[i].getUintColor();

            float* nextJoin = reinterpret_cast<float*>(&dataJoins[32]);
            nextJoin[0]     = mPoints[i + 1].X;
            nextJoin[1]     = mPoints[i + 1].Y;
            nextJoin[2]     = mPoints[i + 1].Z;
            nextJoin[3]     = 0.0f;

            float* normalJoin = reinterpret_cast<float*>(&dataJoins[48]);
            normalJoin[0]     = mPoints[i].xN;
            normalJoin[1]     = mPoints[i].yN;
            normalJoin[2]     = mPoints[i].zN;
            normalJoin[3]     = 0;

            dataJoins += strideJoins;
        }

        dataSegments += strideSegments;
    }
}

} // namespace vcl
