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

#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>

#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {
CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesVertex>& points) :
        mPointsSize(points.size())
{
    allocateVertexBuffer();
    allocateIndexBuffer();
    generateBuffers(points);
}

CPUGeneratedLines::CPUGeneratedLines(CPUGeneratedLines&& other)
{
    swap(other);
}

CPUGeneratedLines::~CPUGeneratedLines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mIndexesBH))
        bgfx::destroy(mIndexesBH);
}

CPUGeneratedLines& CPUGeneratedLines::operator=(CPUGeneratedLines&& other)
{
    swap(other);
    return *this;
}

void CPUGeneratedLines::swap(CPUGeneratedLines& other)
{
    Lines::swap(other);

    std::swap(mPointsSize, other.mPointsSize);

    std::swap(mIndexesBH, other.mIndexesBH);
    std::swap(mVerticesBH, other.mVerticesBH);
}

void CPUGeneratedLines::update(const std::vector<LinesVertex>& points)
{
    if (mPointsSize > points.size()) {
        bgfx::destroy(mVerticesBH);
        bgfx::destroy(mIndexesBH);

        allocateVertexBuffer();
        allocateIndexBuffer();
    }

    mPointsSize = points.size();
    generateBuffers(points);
}

void CPUGeneratedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                     BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                     UINT64_C(0) | BGFX_STATE_BLEND_ALPHA;

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndexesBH);
    bgfx::setState(state);
    bgfx::submit(viewId, mLinesPH);
}

void CPUGeneratedLines::generateBuffers(const std::vector<LinesVertex> points)
{
    uint bufferVertsSize = points.size() * 4 * 12;
    uint bufferIndsSize = points.size() * 6;

    auto [vertices, vReleaseFn] =
        getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

    auto [indices, iReleaseFn] =
        getAllocatedBufferAndReleaseFn<uint>(bufferIndsSize);

    uint vi = 0;
    uint ii = 0;
    for (uint i = 1; i < points.size(); i += 2) {
        for (uint k = 0; k < 2; k++) {
            for (uint j = 0; j < 2; j++) {
                vertices[vi++] = points[i - 1].X;
                vertices[vi++] = points[i - 1].Y;
                vertices[vi++] = points[i - 1].Z;

                vertices[vi++] = points[i].X;
                vertices[vi++] = points[i].Y;
                vertices[vi++] = points[i].Z;

                vertices[vi++] = points[i - (1 - k)].getReverseColor();

                vertices[vi++] = points[i - (1 - k)].xN;
                vertices[vi++] = points[i - (1 - k)].yN;
                vertices[vi++] = points[i - (1 - k)].zN;

                vertices[vi++] = k;
                vertices[vi++] = j;
            }
        }

        uint index = (4 * (i / 2));
        indices[ii++] = index;
        indices[ii++] = index + 3;
        indices[ii++] = index + 1;

        indices[ii++] = index;
        indices[ii++] = index + 2;
        indices[ii++] = index + 3;
    }

    bgfx::update(
        mVerticesBH,
        0,
        bgfx::makeRef(vertices, sizeof(float) * bufferVertsSize, vReleaseFn));
    bgfx::update(
        mIndexesBH,
        0,
        bgfx::makeRef(indices, sizeof(uint) * bufferIndsSize, iReleaseFn));
}

void CPUGeneratedLines::allocateVertexBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
        .end();

    mVerticesBH = bgfx::createDynamicVertexBuffer(
        (mPointsSize / 2) * 4, layout, BGFX_BUFFER_ALLOW_RESIZE);
}

void CPUGeneratedLines::allocateIndexBuffer()
{
    mIndexesBH = bgfx::createDynamicIndexBuffer(
        (mPointsSize / 2) * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
}

} // namespace vcl::lines
