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

#include <vclib/bgfx/drawable/drawable_trackball.h>

#include <vclib/algorithms/core/polygon/create.h>
#include <vclib/math/transform.h>

namespace vcl {

DrawableTrackBall::DrawableTrackBall()
{
    mUniforms.setNumberOfVerticesPerAxis(N_POINTS);

    createBuffers();
}

DrawableTrackBall::~DrawableTrackBall()
{
    if (bgfx::isValid(mVertexCoordBH)) {
        bgfx::destroy(mVertexCoordBH);
    }
}

void DrawableTrackBall::updateDragging(bool isDragging)
{
    mUniforms.setDragging(isDragging);
}

void DrawableTrackBall::setTransform(const Matrix44f& mtx)
{
    mTransform = mtx;
}

void DrawableTrackBall::draw(uint viewId) const
{
    if (isVisible()) {
        if (bgfx::isValid(mProgram)) {
            bgfx::setState(
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z |
                BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_PT_LINES |
                BGFX_STATE_BLEND_NORMAL);

            bgfx::setVertexBuffer(0, mVertexCoordBH);
            bgfx::setIndexBuffer(mEdgeIndexBH);

            bgfx::setTransform(mTransform.data());

            mUniforms.bind();

            bgfx::submit(viewId, mProgram);
        }
    }
}

void DrawableTrackBall::createBuffers()
{
    // vertex buffer
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();

    mVertexCoordBH = bgfx::createVertexBuffer(
        bgfx::makeRef(
            TRACKBALL_DATA.first.data(),
            TRACKBALL_DATA.first.size() * 3 * sizeof(float)),
        layout);

    mEdgeIndexBH = bgfx::createIndexBuffer(bgfx::makeRef(
        TRACKBALL_DATA.second.data(),
        TRACKBALL_DATA.second.size() * sizeof(uint16_t)));
}

} // namespace vcl
