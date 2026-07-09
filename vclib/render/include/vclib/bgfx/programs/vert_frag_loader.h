// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PROGRAMS_VERT_FRAG_LOADER_H
#define VCL_BGFX_PROGRAMS_VERT_FRAG_LOADER_H

#include <vclib/bgfx/programs/vert_frag_program.h>

#include <bgfx/embedded_shader.h>

namespace vcl {

template<VertFragProgram PROGRAM = VertFragProgram::COUNT>
struct VertFragLoader
{
    static bgfx::EmbeddedShader::Data vertexShader(
        bgfx::RendererType::Enum type)
    {
        static_assert(false, "Invalid program");
        switch (type) {
        case bgfx::RendererType::OpenGLES: return {type, nullptr, 0};
        case bgfx::RendererType::OpenGL: return {type, nullptr, 0};
        case bgfx::RendererType::Vulkan: return {type, nullptr, 0};
#ifdef _WIN32
        case bgfx::RendererType::Direct3D11: return {type, nullptr, 0};
        case bgfx::RendererType::Direct3D12: return {type, nullptr, 0};
#endif
#ifdef __APPLE__
        case bgfx::RendererType::Metal: return {type, nullptr, 0};
#endif
        default: return {type, nullptr, 0};
        }
    }

    static bgfx::EmbeddedShader::Data fragmentShader(
        bgfx::RendererType::Enum type)
    {
        static_assert(false, "Invalid program");
        switch (type) {
        case bgfx::RendererType::OpenGLES: return {type, nullptr, 0};
        case bgfx::RendererType::OpenGL: return {type, nullptr, 0};
        case bgfx::RendererType::Vulkan: return {type, nullptr, 0};
#ifdef _WIN32
        case bgfx::RendererType::Direct3D11: return {type, nullptr, 0};
        case bgfx::RendererType::Direct3D12: return {type, nullptr, 0};
#endif
#ifdef __APPLE__
        case bgfx::RendererType::Metal: return {type, nullptr, 0};
#endif
        default: return {type, nullptr, 0};
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_PROGRAMS_VERT_FRAG_LOADER_H
