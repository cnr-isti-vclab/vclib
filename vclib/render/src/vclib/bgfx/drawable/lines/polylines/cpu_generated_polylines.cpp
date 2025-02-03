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
    allocateIndexesBuffer();
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

    if (bgfx::isValid(mSegmentsIndexesBH))
        bgfx::destroy(mSegmentsIndexesBH);

    if (bgfx::isValid(mJoinsIndexesBH))
        bgfx::destroy(mJoinsIndexesBH);
}

CPUGeneratedPolylines& CPUGeneratedPolylines::operator=(
    CPUGeneratedPolylines&& other)
{
    swap(other);
    return *this;
}

void CPUGeneratedPolylines::swap(CPUGeneratedPolylines& other)
{
    std::swap(mSettings, other.mSettings);

    std::swap(mPointsSize, other.mPointsSize);

    std::swap(mVertices, other.mVertices);
    std::swap(mSegmentsIndexes, other.mSegmentsIndexes);
    std::swap(mJoinsIndexes, other.mJoinsIndexes);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mSegmentsIndexesBH, other.mSegmentsIndexesBH);
    std::swap(mJoinsIndexesBH, other.mJoinsIndexesBH);
}

void CPUGeneratedPolylines::draw(uint viewId) const
{
    mSettings.bindUniformPolylines();

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                     BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                     UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mSegmentsIndexesBH);
    bgfx::setState(state);
    bgfx::submit(viewId, mLinesPH);

    if (mSettings.getJoin() != 0) {
        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mJoinsIndexesBH);
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH);
    }
}

void CPUGeneratedPolylines::update(const std::vector<LinesVertex>& points)
{
    mVertices.clear();
    mSegmentsIndexes.clear();
    mJoinsIndexes.clear();

    if (mPointsSize > points.size()) {
        bgfx::destroy(mVerticesBH);
        bgfx::destroy(mSegmentsIndexesBH);
        bgfx::destroy(mJoinsIndexesBH);

        allocateVertexBuffer();
        allocateIndexesBuffer();
    }

    mPointsSize = points.size();
    generateBuffers(points);
}

void CPUGeneratedPolylines::generateBuffers(
    const std::vector<LinesVertex> points)
{
    for (uint i = 0; i < points.size() - 1; i++) {
        for (uint k = 0; k < 2; k++) {
            for (uint j = 0; j < 2; j++) {
                uint curr_index = i + k;
                uint prev_index = curr_index - (curr_index == 0 ? 0 : 1);
                uint next_index =
                    curr_index + (curr_index == points.size() - 1 ? 0 : 1);

                mVertices.push_back(points[prev_index].X);
                mVertices.push_back(points[prev_index].Y);
                mVertices.push_back(points[prev_index].Z);

                mVertices.push_back(points[curr_index].X);
                mVertices.push_back(points[curr_index].Y);
                mVertices.push_back(points[curr_index].Z);

                mVertices.push_back(points[next_index].X);
                mVertices.push_back(points[next_index].Y);
                mVertices.push_back(points[next_index].Z);

                mVertices.push_back(points[curr_index].getReverseColor());

                mVertices.push_back(points[curr_index].xN);
                mVertices.push_back(points[curr_index].yN);
                mVertices.push_back(points[curr_index].zN);

                mVertices.push_back(static_cast<float>(k));
                mVertices.push_back(static_cast<float>(j));
            }
        }

        mSegmentsIndexes.push_back((i * 4));
        mSegmentsIndexes.push_back((i * 4) + 3);
        mSegmentsIndexes.push_back((i * 4) + 1);

        mSegmentsIndexes.push_back((i * 4));
        mSegmentsIndexes.push_back((i * 4) + 2);
        mSegmentsIndexes.push_back((i * 4) + 3);

        if (i != points.size() - 2) {
            mJoinsIndexes.push_back((i * 4) + 3);
            mJoinsIndexes.push_back((i * 4) + 4);
            mJoinsIndexes.push_back((i * 4) + 5);

            mJoinsIndexes.push_back((i * 4) + 4);
            mJoinsIndexes.push_back((i * 4) + 2);
            mJoinsIndexes.push_back((i * 4) + 5);
        }
    }
    bgfx::update(
        mVerticesBH,
        0,
        bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()));
    bgfx::update(
        mSegmentsIndexesBH,
        0,
        bgfx::makeRef(
            &mSegmentsIndexes[0], sizeof(uint32_t) * mSegmentsIndexes.size()));
    bgfx::update(
        mJoinsIndexesBH,
        0,
        bgfx::makeRef(
            &mJoinsIndexes[0], sizeof(uint32_t) * mJoinsIndexes.size()));
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

void CPUGeneratedPolylines::allocateIndexesBuffer()
{
    mSegmentsIndexesBH = bgfx::createDynamicIndexBuffer(
        (mPointsSize - 1) * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);

    mJoinsIndexesBH = bgfx::createDynamicIndexBuffer(
        (mPointsSize - 2) * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
}

} // namespace vcl::lines
