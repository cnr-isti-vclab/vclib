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

#include <vclib/bgfx/context/embedded_programs/drawable_mesh_points.h>

#include <shaders/drawable/drawable_mesh/points/fs_points.sc.glsl.bin.h>
#include <shaders/drawable/drawable_mesh/points/vs_points.sc.glsl.bin.h>

#include <shaders/drawable/drawable_mesh/points/fs_points.sc.essl.bin.h>
#include <shaders/drawable/drawable_mesh/points/vs_points.sc.essl.bin.h>

#include <shaders/drawable/drawable_mesh/points/fs_points.sc.spv.bin.h>
#include <shaders/drawable/drawable_mesh/points/vs_points.sc.spv.bin.h>
#ifdef _WIN32
#include <shaders/drawable/drawable_mesh/points/fs_points.sc.dx11.bin.h>
#include <shaders/drawable/drawable_mesh/points/vs_points.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <shaders/drawable/drawable_mesh/points/fs_points.sc.mtl.bin.h>
#include <shaders/drawable/drawable_mesh/points/vs_points.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data vcl::
    EmbeddedProgram<VclProgram::DRAWABLE_MESH_POINTS>::vertexEmbeddedShader(
        bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, vs_points_essl, sizeof(vs_points_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, vs_points_glsl, sizeof(vs_points_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, vs_points_spv, sizeof(vs_points_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, vs_points_dx11, sizeof(vs_points_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, vs_points_mtl, sizeof(vs_points_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::
    EmbeddedProgram<VclProgram::DRAWABLE_MESH_POINTS>::fragmentEmbeddedShader(
        bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, fs_points_essl, sizeof(fs_points_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, fs_points_glsl, sizeof(fs_points_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, fs_points_spv, sizeof(fs_points_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, fs_points_dx11, sizeof(fs_points_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, fs_points_mtl, sizeof(fs_points_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::
    EmbeddedProgram<VclProgram::DRAWABLE_MESH_POINTS>::computeEmbeddedShader(
        bgfx::RendererType::Enum type)
{
    static_assert(true, "Invalid shader type for Drawable Mesh Points");
    return {type, nullptr, 0};
}

} // namespace vcl
