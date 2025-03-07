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

#include <vclib/bgfx/programs/embedded_vf_programs/polylines.h>

#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/fs_cpu_generated_polylines.sc.glsl.bin.h>
#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/vs_cpu_generated_polylines.sc.glsl.bin.h>

#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/fs_cpu_generated_polylines.sc.essl.bin.h>
#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/vs_cpu_generated_polylines.sc.essl.bin.h>

#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/fs_cpu_generated_polylines.sc.spv.bin.h>
#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/vs_cpu_generated_polylines.sc.spv.bin.h>
#ifdef _WIN32
#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/fs_cpu_generated_polylines.sc.dx11.bin.h>
#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/vs_cpu_generated_polylines.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/fs_cpu_generated_polylines.sc.mtl.bin.h>
#include <shaders/drawable/drawable_polylines/cpu_generated_polylines/vs_cpu_generated_polylines.sc.mtl.bin.h>

#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data vcl::VertFragLoader<VertFragProgram::POLYLINES>::
    vertexShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, vs_cpu_generated_polylines_essl, sizeof(vs_cpu_generated_polylines_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, vs_cpu_generated_polylines_glsl, sizeof(vs_cpu_generated_polylines_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, vs_cpu_generated_polylines_spv, sizeof(vs_cpu_generated_polylines_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, vs_cpu_generated_polylines_dx11, sizeof(vs_cpu_generated_polylines_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, vs_cpu_generated_polylines_mtl, sizeof(vs_cpu_generated_polylines_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::VertFragLoader<VertFragProgram::POLYLINES>::
    fragmentShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, fs_cpu_generated_polylines_essl, sizeof(fs_cpu_generated_polylines_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, fs_cpu_generated_polylines_glsl, sizeof(fs_cpu_generated_polylines_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, fs_cpu_generated_polylines_spv, sizeof(fs_cpu_generated_polylines_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, fs_cpu_generated_polylines_dx11, sizeof(fs_cpu_generated_polylines_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, fs_cpu_generated_polylines_mtl, sizeof(fs_cpu_generated_polylines_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl
