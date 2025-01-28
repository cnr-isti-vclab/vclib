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

#include <vclib/bgfx/drawable/drawable_axis.h>

#include <vclib/algorithms/mesh/create.h>

namespace vcl {

DrawableAxis::DrawableAxis(double size)
{
    createAxis();

    updateMatrices(size);
}

void DrawableAxis::setSize(double size)
{
    updateMatrices(size);
}

void DrawableAxis::draw(uint viewId) const
{
    if (isVisible()) {
        if (bgfx::isValid(mProgram)) {
            uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                             BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                             BGFX_STATE_MSAA;
            for (uint i = 0; i < 3; i++) {
                for (uint j = 0; j < 2; j++) {
                    if (j == 0) // cylinders
                        mUniforms.setColor(AXIS_COLORS[i]);
                    else // rest (cone, spheres...)
                        mUniforms.setColor(vcl::Color::White);
                    mUniforms.bind();

                    mArrowBuffers[j].bindVertexBuffers(MeshRenderSettings());
                    mArrowBuffers[j].bindIndexBuffers();

                    bgfx::setTransform(mMatrices[i].data());

                    bgfx::submit(viewId, mProgram);
                }
            }
        }
    }
}

void DrawableAxis::updateMatrices(double size)
{
    mMatrices[0](0, 1) = size;
    mMatrices[0](1, 0) = -size;
    mMatrices[0](2, 2) = size;
    mMatrices[0](3, 3) = 1;

    mMatrices[1](0, 0) = size;
    mMatrices[1](1, 1) = size;
    mMatrices[1](2, 2) = size;
    mMatrices[1](3, 3) = 1;

    mMatrices[2](0, 0) = size;
    mMatrices[2](1, 2) = -size;
    mMatrices[2](2, 1) = size;
    mMatrices[2](3, 3) = 1;
}

void DrawableAxis::createAxis()
{
    BuffersToFill btf = {
        toUnderlying(MeshBufferId::VERTICES),
        toUnderlying(MeshBufferId::VERT_NORMALS),
        toUnderlying(MeshBufferId::TRIANGLES)};

    mArrowBuffers[0] = MeshRenderBuffers<vcl::TriMesh>(AXIS_MESHES.first, btf);

    mArrowBuffers[1] = MeshRenderBuffers<vcl::TriMesh>(AXIS_MESHES.second, btf);
}

} // namespace vcl
