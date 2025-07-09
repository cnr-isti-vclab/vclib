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

#include <vclib/render/primitives/lines/cpu_generated_lines.h>

namespace vcl::detail {

CPUGeneratedLines::CPUGeneratedLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors,
        const std::vector<float>& vertNormals,
        const std::vector<uint>& lineColors)
{
    setPoints(vertCoords, vertColors, vertNormals, lineColors);
}

void CPUGeneratedLines::swap(CPUGeneratedLines& other)
{
    using std::swap;

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
}

void CPUGeneratedLines::setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors,
        const std::vector<float>& vertNormals,
        const std::vector<uint>& lineColors)
{
    const uint nPoints = vertCoords.size() / 3;
    if (nPoints > 1) {
        // generate memory buffers
        uint bufferVertsSize = (nPoints / 2) * 4 * 13;
        uint bufferIndsSize = (nPoints / 2) * 6;

        auto [vertices, vReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

        auto [indices, iReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<uint>(bufferIndsSize);

        uint vi = 0;
        uint ii = 0;
        for (uint i = 1; i < nPoints; i += 2) {
            for (uint k = 0; k < 2; k++) {
                for (uint j = 0; j < 2; j++) {
                    vertices[vi++] = vertCoords[((i - 1) * 3)];
                    vertices[vi++] = vertCoords[((i - 1) * 3) + 1];
                    vertices[vi++] = vertCoords[((i - 1) * 3) + 2];

                    vertices[vi++] = vertCoords[(i * 3)];
                    vertices[vi++] = vertCoords[(i * 3) + 1];
                    vertices[vi++] = vertCoords[(i * 3) + 2];

                    vertices[vi++] = std::bit_cast<float>(vertColors[i - (1 - k)]);

                    vertices[vi++] = vertNormals[((i - (1 - k)) * 3)];
                    vertices[vi++] = vertNormals[((i - (1 - k)) * 3) + 1];
                    vertices[vi++] = vertNormals[((i - (1 - k)) * 3) + 2];

                    vertices[vi++] = k;
                    vertices[vi++] = j;

                    vertices[vi++] = std::bit_cast<float>(lineColors[i/2]);
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
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
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
    mVertices.bind(0);
    mIndices.bind();
    bgfx::setState(linesDrawState());
    bgfx::submit(viewId, mLinesPH);
}

} // namespace vcl
