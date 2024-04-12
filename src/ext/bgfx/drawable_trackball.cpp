/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/ext/bgfx/drawable_trackball.h>

#include <vclib/algorithms/polygon/create.h>

#include <vclib/math/transform.h>

namespace vcl::bgf {

DrawableTrackBall::DrawableTrackBall()
{
    const uint cSize = 64;

    mUniforms.setNumberOfVerticesPerAxis(cSize);

    vcl::Polygon2f circle = vcl::createCircle<vcl::Polygon2f>(cSize, 1.0f);

    mVertices.reserve(cSize * 3);

    // x
    uint first = 0;
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        mVertices.push_back(vcl::Point3f(0, p.x(), p.y()));
        mEdges.push_back(i + first);
        mEdges.push_back((i + 1) % circle.size() + first);
    }

    // y
    first = circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        mVertices.push_back(vcl::Point3f(p.x(), 0, p.y()));
        mEdges.push_back(i + first);
        mEdges.push_back((i + 1) % circle.size() + first);
    }

    // z
    first = 2 * circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        mVertices.push_back(vcl::Point3f(p.x(), p.y(), 0));
        mEdges.push_back(i + first);
        mEdges.push_back((i + 1) % circle.size() + first);
    }

    createBuffers();
}

DrawableTrackBall::~DrawableTrackBall()
{
    if (bgfx::isValid(mVertexCoordBH)) {
        bgfx::destroy(mVertexCoordBH);
    }
}

void DrawableTrackBall::updateRotation(const vcl::Matrix44f& rot)
{
    mTransform = rot;
}

void DrawableTrackBall::updateDragging(bool isDragging)
{
    mUniforms.setDragging(isDragging);
}

void DrawableTrackBall::draw(uint viewId)
{
    if (isVisible()) {
        if (bgfx::isValid(mProgram)) {
            bgfx::setState(
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                BGFX_STATE_PT_LINES |
                BGFX_STATE_BLEND_FUNC(
                    BGFX_STATE_BLEND_SRC_ALPHA,
                    BGFX_STATE_BLEND_INV_SRC_ALPHA));

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
        bgfx::makeRef(mVertices.data(), mVertices.size() * 3 * sizeof(float)),
        layout);

    mEdgeIndexBH = bgfx::createIndexBuffer(
        bgfx::makeRef(mEdges.data(), mEdges.size() * sizeof(uint16_t)));
}

} // namespace vcl::bgf
