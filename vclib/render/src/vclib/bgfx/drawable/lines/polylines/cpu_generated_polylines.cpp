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
    const std::vector<LinesVertex>& points) : mPointsSize(points.size())
{
    assert(bgfx::isValid(mLinesPH));
    allocateVertexBuffer();
    allocateIndicesBuffer();
    generateBuffers(points);
}

CPUGeneratedPolylines::CPUGeneratedPolylines(CPUGeneratedPolylines&& other)
{
    swap(other);
}

CPUGeneratedPolylines::~CPUGeneratedPolylines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mSegmentsIndicesBH))
        bgfx::destroy(mSegmentsIndicesBH);

    if (bgfx::isValid(mJoinsIndicesBH))
        bgfx::destroy(mJoinsIndicesBH);
}

CPUGeneratedPolylines& CPUGeneratedPolylines::operator=(
    CPUGeneratedPolylines&& other)
{
    swap(other);
    return *this;
}

void CPUGeneratedPolylines::swap(CPUGeneratedPolylines& other)
{
    Lines::swap(other);

    std::swap(mPointsSize, other.mPointsSize);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mSegmentsIndicesBH, other.mSegmentsIndicesBH);
    std::swap(mJoinsIndicesBH, other.mJoinsIndicesBH);
}

void CPUGeneratedPolylines::draw(uint viewId) const
{
    bindSettingsUniformPolylines();

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mSegmentsIndicesBH);
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);

    if (settings().getJoin() != 0) {
        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mJoinsIndicesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }
}

void CPUGeneratedPolylines::update(const std::vector<LinesVertex>& points)
{
    if (mPointsSize > points.size()) {
        bgfx::destroy(mVerticesBH);
        bgfx::destroy(mSegmentsIndicesBH);
        bgfx::destroy(mJoinsIndicesBH);

        allocateVertexBuffer();
        allocateIndicesBuffer();
    }

    mPointsSize = points.size();
    generateBuffers(points);
}

void CPUGeneratedPolylines::generateBuffers(
    const std::vector<LinesVertex> points)
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
    bgfx::update(
        mVerticesBH,
        0,
        bgfx::makeRef(vertices, sizeof(float) * bufferVertsSize, vReleaseFn));
    bgfx::update(
        mSegmentsIndicesBH,
        0,
        bgfx::makeRef(
            segmIndices, sizeof(uint) * bufferSegmetIndicesSize, siReleaseFn));
    bgfx::update(
        mJoinsIndicesBH,
        0,
        bgfx::makeRef(
            joinIndices, sizeof(uint) * bufferJoinsIndicesSize, jiReleaseFn));
}

void CPUGeneratedPolylines::allocateVertexBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
        .end();

    mVerticesBH = bgfx::createDynamicVertexBuffer(
        (mPointsSize - 1) * 4, layout, BGFX_BUFFER_ALLOW_RESIZE);
}

void CPUGeneratedPolylines::allocateIndicesBuffer()
{
    mSegmentsIndicesBH = bgfx::createDynamicIndexBuffer(
        (mPointsSize - 1) * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);

    mJoinsIndicesBH = bgfx::createDynamicIndexBuffer(
        (mPointsSize - 2) * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
}

} // namespace vcl::lines
