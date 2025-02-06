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

#include <vclib/bgfx/drawable/lines/lines/instancing_based_lines.h>

namespace vcl::lines {
InstancingBasedLines::InstancingBasedLines(
    const std::vector<LinesVertex>& points) : mPoints(points)
{
    checkCaps();
    allocateVerticesBuffer();
    allocateIndicesBuffer();
}

InstancingBasedLines::InstancingBasedLines(InstancingBasedLines&& other)
{
    swap(other);
}

InstancingBasedLines::~InstancingBasedLines()
{
    if (bgfx::isValid(mVerticesBH))
        bgfx::destroy(mVerticesBH);

    if (bgfx::isValid(mIndicesBH))
        bgfx::destroy(mIndicesBH);
}

InstancingBasedLines& InstancingBasedLines::operator=(
    InstancingBasedLines&& other)
{
    swap(other);
    return *this;
}

void InstancingBasedLines::swap(InstancingBasedLines& other)
{
    Lines::swap(other);

    std::swap(mLinesPH, other.mLinesPH);

    std::swap(mPoints, other.mPoints);

    std::swap(mVerticesBH, other.mVerticesBH);
    std::swap(mIndicesBH, other.mIndicesBH);
    std::swap(mInstanceDB, other.mInstanceDB);
}

void InstancingBasedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    generateInstanceDataBuffer();

    bgfx::setVertexBuffer(0, mVerticesBH);
    bgfx::setIndexBuffer(mIndicesBH);
    bgfx::setInstanceDataBuffer(&mInstanceDB);

    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);
}

void InstancingBasedLines::update(const std::vector<LinesVertex>& points)
{
    mPoints = points;
}

void InstancingBasedLines::generateInstanceDataBuffer() const
{
    const uint16_t stride = sizeof(float) * 16;
    uint           size   = (mPoints.size() / 2);

    uint linesNum = bgfx::getAvailInstanceDataBuffer(size, stride);
    bgfx::allocInstanceDataBuffer(&mInstanceDB, size, stride);

    uint8_t* data = mInstanceDB.data;
    for (uint i = 1; i < mPoints.size(); i += 2) {
        float* p0 = reinterpret_cast<float*>(data);
        p0[0]     = mPoints[i - 1].X;
        p0[1]     = mPoints[i - 1].Y;
        p0[2]     = mPoints[i - 1].Z;

        uint* color0 = reinterpret_cast<uint*>(&data[12]);
        color0[0]    = mPoints[i - 1].getUintColor();

        float* p1 = reinterpret_cast<float*>(&data[16]);
        p1[0]     = mPoints[i].X;
        p1[1]     = mPoints[i].Y;
        p1[2]     = mPoints[i].Z;

        uint* color1 = reinterpret_cast<uint*>(&data[28]);
        color1[0]    = mPoints[i].getUintColor();

        float* n0 = reinterpret_cast<float*>(&data[32]);
        n0[0]     = mPoints[i - 1].xN;
        n0[1]     = mPoints[i - 1].yN;
        n0[2]     = mPoints[i - 1].zN;
        n0[3]     = 0;

        float* n1 = reinterpret_cast<float*>(&data[48]);
        n1[0]     = mPoints[i].xN;
        n1[1]     = mPoints[i].yN;
        n1[2]     = mPoints[i].zN;
        n1[3]     = 0;

        data += stride;
    }
}

void InstancingBasedLines::allocateVerticesBuffer()
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .end();

    mVerticesBH = bgfx::createVertexBuffer(
        bgfx::makeRef(&VERTICES[0], sizeof(float) * VERTICES.size()), layout);
}

void InstancingBasedLines::allocateIndicesBuffer()
{
    mIndicesBH = bgfx::createIndexBuffer(
        bgfx::makeRef(&INDICES[0], sizeof(uint) * INDICES.size()),
        BGFX_BUFFER_INDEX32);
}

} // namespace vcl::lines
