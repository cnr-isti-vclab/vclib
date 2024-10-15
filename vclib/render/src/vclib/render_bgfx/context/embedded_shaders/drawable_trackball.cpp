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

#include <vclib/render_bgfx/context/embedded_shaders/drawable_trackball.h>

#include <shaders/drawable/drawable_trackball/fs_drawable_trackball.sc.glsl.bin.h>
#include <shaders/drawable/drawable_trackball/vs_drawable_trackball.sc.glsl.bin.h>

#include <shaders/drawable/drawable_trackball/fs_drawable_trackball.sc.essl.bin.h>
#include <shaders/drawable/drawable_trackball/vs_drawable_trackball.sc.essl.bin.h>

#include <shaders/drawable/drawable_trackball/fs_drawable_trackball.sc.spv.bin.h>
#include <shaders/drawable/drawable_trackball/vs_drawable_trackball.sc.spv.bin.h>
#ifdef _WIN32
#include <shaders/drawable/drawable_trackball/fs_drawable_trackball.sc.dx11.bin.h>
#include <shaders/drawable/drawable_trackball/vs_drawable_trackball.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <shaders/drawable/drawable_trackball/fs_drawable_trackball.sc.mtl.bin.h>
#include <shaders/drawable/drawable_trackball/vs_drawable_trackball.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data vcl::EmbeddedShader<VclProgram::DRAWABLE_TRACKBALL>::
    vertexEmbeddedShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            vs_drawable_trackball_essl,
            sizeof(vs_drawable_trackball_essl)};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            vs_drawable_trackball_glsl,
            sizeof(vs_drawable_trackball_glsl)};
    case bgfx::RendererType::Vulkan:
        return {
            type, vs_drawable_trackball_spv, sizeof(vs_drawable_trackball_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            vs_drawable_trackball_dx11,
            sizeof(vs_drawable_trackball_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type, vs_drawable_trackball_mtl, sizeof(vs_drawable_trackball_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::EmbeddedShader<VclProgram::DRAWABLE_TRACKBALL>::
    fragmentEmbeddedShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            fs_drawable_trackball_essl,
            sizeof(fs_drawable_trackball_essl)};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            fs_drawable_trackball_glsl,
            sizeof(fs_drawable_trackball_glsl)};
    case bgfx::RendererType::Vulkan:
        return {
            type, fs_drawable_trackball_spv, sizeof(fs_drawable_trackball_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            fs_drawable_trackball_dx11,
            sizeof(fs_drawable_trackball_dx11)};

    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type, fs_drawable_trackball_mtl, sizeof(fs_drawable_trackball_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::EmbeddedShader<VclProgram::DRAWABLE_TRACKBALL>::
    computeEmbeddedShader(bgfx::RendererType::Enum type)
{
    static_assert(true, "Invalid shader type for Drawable Trackball");
    return {type, nullptr, 0};
}

} // namespace vcl
