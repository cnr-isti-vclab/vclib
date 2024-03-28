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

namespace vcl::bgf {

DrawableTrackball::DrawableTrackball()
{
    const uint cSize = 64;

    vcl::Polygon2f circle = vcl::createCircle<vcl::Polygon2f>(cSize, 1.0f);

    vertices.reserve(cSize * 3);

    // x
    uint first = 0;
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertices.push_back(vcl::Point3f(0, p.x(), p.y()));
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    // y
    first = circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertices.push_back(vcl::Point3f(p.x(), 0, p.y()));
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    // z
    first = 2 * circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertices.push_back(vcl::Point3f(p.x(), p.y(), 0));
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    // vertex buffer
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();

    vertexCoordBH = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices.data(), vertices.size() * 3 * sizeof(float)),
        layout);

    edgeIndexBH = bgfx::createIndexBuffer(
        bgfx::makeRef(edges.data(), edges.size() * sizeof(uint16_t)));
}

DrawableTrackball::~DrawableTrackball()
{
    if (bgfx::isValid(vertexCoordBH)) {
        bgfx::destroy(vertexCoordBH);
    }
}

void DrawableTrackball::draw(uint viewId)
{
    if (isVisible()) {
        if (bgfx::isValid(program)) {
            bgfx::setState(
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                BGFX_STATE_PT_LINES);

            bgfx::setVertexBuffer(0, vertexCoordBH);
            bgfx::setIndexBuffer(edgeIndexBH);

            bgfx::submit(viewId, program);
        }
    }
}

} // namespace vcl::bgf
