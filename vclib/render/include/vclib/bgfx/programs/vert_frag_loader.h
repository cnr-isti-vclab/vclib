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

#ifndef VCL_BGFX_PROGRAMS_VERT_FRAG_LOADER_H
#define VCL_BGFX_PROGRAMS_VERT_FRAG_LOADER_H

#include "vert_frag_program.h"

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
