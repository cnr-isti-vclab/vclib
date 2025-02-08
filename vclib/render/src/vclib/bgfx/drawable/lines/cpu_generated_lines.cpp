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

#include <vclib/bgfx/drawable/lines/cpu_generated_lines.h>

#include <vclib/bgfx/context/load_program.h>

namespace vcl {

CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesVertex>& points)
{
    setPoints(points);
}

void CPUGeneratedLines::swap(CPUGeneratedLines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
}

void CPUGeneratedLines::setPoints(const std::vector<LinesVertex>& points)
{
    if (points.size() > 1) {
        // generate memory buffers
        uint bufferVertsSize = points.size() * 4 * 12;
        uint bufferIndsSize = (points.size() / 2) * 6;

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

               // create vertex buffer
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
            .end();

        mVertices.create(
            bgfx::makeRef(vertices, sizeof(float) * bufferVertsSize, vReleaseFn),
            layout);

               // create index buffer
        mIndices.create(
            bgfx::makeRef(indices, sizeof(uint) * bufferIndsSize, iReleaseFn),
            BGFX_BUFFER_INDEX32);
    }
    else {
        mVertices.destroy();
        mIndices.destroy();
    }
}

void CPUGeneratedLines::draw(uint viewId) const
{
    bindSettingsUniformLines();

    mVertices.bind(0);
    mIndices.bind();
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);
}

} // namespace vcl
