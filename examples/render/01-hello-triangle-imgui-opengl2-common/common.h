// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMMON_H
#define COMMON_H

#include <vclib/space/core/color.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

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

#endif // COMMON_H
