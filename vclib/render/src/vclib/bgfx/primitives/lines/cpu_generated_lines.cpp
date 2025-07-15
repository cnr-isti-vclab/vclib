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
    const std::vector<uint>&  lineColors)
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
    const std::vector<uint>&  lineColors)
{
    assert(vertCoords.size() % 3 == 0);
    assert(vertCoords.size() == vertColors.size() * 3);
    assert(vertCoords.size() == vertNormals.size());
    assert(vertColors.size() == lineColors.size() * 2);
    
    const uint nPoints = vertCoords.size() / 3;
    if (nPoints > 1) {
        // generate memory buffers
        // lines x 4 points x 13 attributes
        uint bufferVertsSize = (nPoints / 2) * 4 * 13;
        // lines x 6 indices
        // 2 triangles per line segment, 3 indices per triangle
        uint bufferIndsSize  = (nPoints / 2) * 6;

        auto [vertices, vReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

        auto [indices, iReleaseFn] =
            linesGetAllocatedBufferAndReleaseFn<uint>(bufferIndsSize);

        /// vertices layout (4 points per line segment)
        /// 0-2: position 0
        /// 3-5: position 1
        /// 6: color per vertex
        /// 7-9: normal per vertex
        /// 10: first/second point flag
        /// 11: top/bottom flag
        /// 12: color per line segment

        uint vi = 0; // float index in vertex buffer
        uint ii = 0; // uint index in index buffer
        for (uint i = 0; i < nPoints - 1; i += 2) {
            for (uint k = 0; k < 2; k++) { // k = 0 1st point, k = 1 2nd point
                for (uint j = 0; j < 2; j++) { // j = 0 "top", j = 1 "bottom"
                    vertices[vi++] = vertCoords[(i * 3)];
                    vertices[vi++] = vertCoords[(i * 3) + 1];
                    vertices[vi++] = vertCoords[(i * 3) + 2];

                    vertices[vi++] = vertCoords[((i + 1) * 3)];
                    vertices[vi++] = vertCoords[((i + 1) * 3) + 1];
                    vertices[vi++] = vertCoords[((i + 1) * 3) + 2];

                    vertices[vi++] =
                        std::bit_cast<float>(vertColors[i + k]);

                    vertices[vi++] = vertNormals[((i + k) * 3)];
                    vertices[vi++] = vertNormals[((i + k) * 3) + 1];
                    vertices[vi++] = vertNormals[((i + k) * 3) + 2];

                    vertices[vi++] = k;
                    vertices[vi++] = j;

                    vertices[vi++] = std::bit_cast<float>(lineColors[i / 2]);
                }
            }

            // indices for the two triangles of the line segment
            // each line segment has 4 points, so 2 triangles
            uint index    = (4 * (i / 2));
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
            bgfx::makeRef(
                vertices, sizeof(float) * bufferVertsSize, vReleaseFn),
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

} // namespace vcl::detail
