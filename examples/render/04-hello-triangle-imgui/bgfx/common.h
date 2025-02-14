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

#ifndef COMMON_H
#define COMMON_H

#include <vclib/bgfx/programs/load_program.h>
#include <vclib/space/core/color.h>

#include <bgfx/bgfx.h>

struct Vertex
{
    float    pos[2];
    uint32_t abgr;
};

static const Vertex vertices[] {
    {{-1.0f, -1.0f}, vcl::Color(vcl::Color::Red).abgr()  },
    {{1.0f, -1.0f},  vcl::Color(vcl::Color::Green).abgr()},
    {{0.0f, 1.0f},   vcl::Color(vcl::Color::Blue).abgr() },
};

inline void setUpBGFX(
    bgfx::ViewId              viewId,
    bgfx::VertexBufferHandle& vbh,
    bgfx::ProgramHandle&      program)
{
    vcl::Color backgroundColor = vcl::Color::Black;

    bgfx::setViewClear(
        viewId,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        backgroundColor.rgba(),
        1.0f,
        0);

    bgfx::VertexLayout layout;

    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();

    vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices, sizeof(vertices)), layout);

    program = vcl::loadProgram(
        "shaders/vs_vertex_shader", "shaders/fs_fragment_shader");

    bgfx::touch(viewId);
}

inline void drawOnView(
    bgfx::ViewId                    viewId,
    const bgfx::VertexBufferHandle& vbh,
    const bgfx::ProgramHandle&      program)
{
    bgfx::setVertexBuffer(0, vbh);

    bgfx::submit(viewId, program);
}

#endif // COMMON_H
