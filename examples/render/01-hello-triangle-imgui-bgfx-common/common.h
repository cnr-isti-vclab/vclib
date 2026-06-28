// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
        "shaders/vs_vertex_shader.sc", "shaders/fs_fragment_shader.sc");

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
