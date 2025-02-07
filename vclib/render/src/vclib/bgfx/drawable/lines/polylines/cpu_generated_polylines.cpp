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

#include <vclib/bgfx/drawable/lines/polylines/cpu_generated_polylines.h>

namespace vcl::lines {

CPUGeneratedPolylines::CPUGeneratedPolylines(
    const std::vector<LinesVertex>& points)
{
    assert(bgfx::isValid(mLinesPH));
    update(points);
}

void CPUGeneratedPolylines::swap(CPUGeneratedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mSegmentIndices, other.mSegmentIndices);
    swap(mJoinIndices, other.mJoinIndices);
}

void CPUGeneratedPolylines::update(const std::vector<LinesVertex>& points)
{
    uint bufferVertsSize = (points.size() - 1) * 4 * 15;
    uint bufferSegmetIndicesSize = (points.size() - 1) * 6;
    uint bufferJoinsIndicesSize = (points.size() - 2) * 6;

    auto [vertices, vReleaseFn] =
        getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

    auto [segmIndices, siReleaseFn] =
        getAllocatedBufferAndReleaseFn<uint>(bufferSegmetIndicesSize);

    auto [joinIndices, jiReleaseFn] =
        getAllocatedBufferAndReleaseFn<uint>(bufferJoinsIndicesSize);

    uint vi = 0;
    uint si = 0;
    uint ji = 0;

    for (uint i = 0; i < points.size() - 1; i++) {

        for (uint k = 0; k < 2; k++) {
            for (uint j = 0; j < 2; j++) {
                uint curr_index = i + k;
                uint prev_index = curr_index - (curr_index == 0 ? 0 : 1);
                uint next_index =
                    curr_index + (curr_index == points.size() - 1 ? 0 : 1);

                vertices[vi++] = points[prev_index].X;
                vertices[vi++] = points[prev_index].Y;
                vertices[vi++] = points[prev_index].Z;

                vertices[vi++] = points[curr_index].X;
                vertices[vi++] = points[curr_index].Y;
                vertices[vi++] = points[curr_index].Z;

                vertices[vi++] = points[next_index].X;
                vertices[vi++] = points[next_index].Y;
                vertices[vi++] = points[next_index].Z;

                vertices[vi++] = points[curr_index].getReverseColor();

                vertices[vi++] = points[curr_index].xN;
                vertices[vi++] = points[curr_index].yN;
                vertices[vi++] = points[curr_index].zN;

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

        if (i != points.size() - 2) {
            joinIndices[ji++] = (i * 4) + 3;
            joinIndices[ji++] = (i * 4) + 4;
            joinIndices[ji++] = (i * 4) + 5;

            joinIndices[ji++] = (i * 4) + 4;
            joinIndices[ji++] = (i * 4) + 2;
            joinIndices[ji++] = (i * 4) + 5;
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

    mJoinIndices.create(
        bgfx::makeRef(
            joinIndices, sizeof(uint) * bufferJoinsIndicesSize, jiReleaseFn),
        BGFX_BUFFER_INDEX32);
}

void CPUGeneratedPolylines::draw(uint viewId) const
{
    bindSettingsUniformPolylines();

    mVertices.bind(0);
    mSegmentIndices.bind();
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);

    if (settings().getJoin() != 0) {
        mVertices.bind(0);
        mJoinIndices.bind();
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }
}

} // namespace vcl::lines
