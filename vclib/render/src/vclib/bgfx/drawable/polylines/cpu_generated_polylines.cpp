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

#include <vclib/bgfx/drawable/polylines/cpu_generated_polylines.h>

namespace vcl {

CPUGeneratedPolylines::CPUGeneratedPolylines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    assert(bgfx::isValid(mLinesPH));
    setPoints(vertCoords, vertColors, vertNormals);
}

void CPUGeneratedPolylines::swap(CPUGeneratedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mSegmentIndices, other.mSegmentIndices);
    swap(mJointIndices, other.mJointIndices);
}

void CPUGeneratedPolylines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    const uint nPoints = vertCoords.size() / 3;
    if (nPoints > 1) {
        uint bufferVertsSize = (nPoints - 1) * 4 * 15;
        uint bufferSegmetIndicesSize = (nPoints - 1) * 6;
        uint bufferJointsIndicesSize = (nPoints - 2) * 6;

        auto [vertices, vReleaseFn] =
            getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

        auto [segmIndices, siReleaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(bufferSegmetIndicesSize);

        auto [jointIndices, jiReleaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(bufferJointsIndicesSize);

        uint vi = 0;
        uint si = 0;
        uint ji = 0;

        for (uint i = 0; i < nPoints - 1; i++) {
            for (uint k = 0; k < 2; k++) {
                for (uint j = 0; j < 2; j++) {
                    uint curr_index = i + k;
                    uint prev_index = curr_index - (curr_index == 0 ? 0 : 1);
                    uint next_index =
                        curr_index + (curr_index == nPoints - 1 ? 0 : 1);

                    vertices[vi++] = vertCoords[(prev_index * 3)];
                    vertices[vi++] = vertCoords[(prev_index * 3) + 1];
                    vertices[vi++] = vertCoords[(prev_index * 3) + 2];

                    vertices[vi++] = vertCoords[(curr_index * 3)];
                    vertices[vi++] = vertCoords[(curr_index * 3) + 1];
                    vertices[vi++] = vertCoords[(curr_index * 3) + 2];

                    vertices[vi++] = vertCoords[(next_index * 3)];
                    vertices[vi++] = vertCoords[(next_index * 3) + 1];
                    vertices[vi++] = vertCoords[(next_index * 3) + 2];

                    vertices[vi++] = std::bit_cast<float>(vertColors[curr_index]);

                    vertices[vi++] = vertNormals[(curr_index * 3)];
                    vertices[vi++] = vertNormals[(curr_index * 3) + 1];
                    vertices[vi++] = vertNormals[(curr_index * 3) + 2];

                    vertices[vi++] = static_cast<float>(k);
                    vertices[vi++] = static_cast<float>(j);
                }
            }

            segmIndices[si++] = (i * 4);
            segmIndices[si++] = (i * 4) + 3;
            segmIndices[si++] = (i * 4) + 1;

            segmIndices[si++] = (i * 4);
            segmIndices[si++] = (i * 4) + 2;
            segmIndices[si++] = (i * 4) + 3;

            if (nPoints > 2 && i != nPoints - 2) {
                jointIndices[ji++] = (i * 4) + 3;
                jointIndices[ji++] = (i * 4) + 4;
                jointIndices[ji++] = (i * 4) + 5;

                jointIndices[ji++] = (i * 4) + 4;
                jointIndices[ji++] = (i * 4) + 2;
                jointIndices[ji++] = (i * 4) + 5;
            }
        }

        // vertices

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();

        mVertices.create(
            bgfx::makeRef(vertices, sizeof(float) * bufferVertsSize, vReleaseFn),
            layout);

        // indices
        mSegmentIndices.create(
            bgfx::makeRef(
                segmIndices, sizeof(uint) * bufferSegmetIndicesSize, siReleaseFn),
            BGFX_BUFFER_INDEX32);

        if (nPoints > 2) {
            mJointIndices.create(
                bgfx::makeRef(
                    jointIndices, sizeof(uint) * bufferJointsIndicesSize, jiReleaseFn),
                BGFX_BUFFER_INDEX32);
        }
        else {
            delete[] jointIndices;
            mJointIndices.destroy();
        }
    }
}

void CPUGeneratedPolylines::draw(uint viewId) const
{
    bindSettingsUniform();

    mVertices.bind(0);
    mSegmentIndices.bind();
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);

    // mJointIndices is valid only if there are more than 2 points
    if (mJointIndices.isValid() && settings().getJoint() != PolyLineJoint::ROUND_JOINT) {
        mVertices.bind(0);
        mJointIndices.bind();
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }
}

} // namespace vcl
