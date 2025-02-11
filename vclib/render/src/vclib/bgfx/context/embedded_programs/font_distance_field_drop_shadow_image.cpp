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

#include <vclib/bgfx/context/embedded_programs/font_distance_field_drop_shadow_image.h>

#include <shaders/font/fs_font_distance_field_drop_shadow_image.sc.glsl.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow_image.sc.glsl.bin.h>

#include <shaders/font/fs_font_distance_field_drop_shadow_image.sc.essl.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow_image.sc.essl.bin.h>

#include <shaders/font/fs_font_distance_field_drop_shadow_image.sc.spv.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow_image.sc.spv.bin.h>
#ifdef _WIN32
#include <shaders/font/fs_font_distance_field_drop_shadow_image.sc.dx11.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow_image.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <shaders/font/fs_font_distance_field_drop_shadow_image.sc.mtl.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow_image.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data vcl::EmbeddedProgram<
    VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE>::
    vertexEmbeddedShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            vs_font_distance_field_drop_shadow_image_essl,
            sizeof(vs_font_distance_field_drop_shadow_image_essl)};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            vs_font_distance_field_drop_shadow_image_glsl,
            sizeof(vs_font_distance_field_drop_shadow_image_glsl)};
    case bgfx::RendererType::Vulkan:
        return {
            type,
            vs_font_distance_field_drop_shadow_image_spv,
            sizeof(vs_font_distance_field_drop_shadow_image_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            vs_font_distance_field_drop_shadow_image_dx11,
            sizeof(vs_font_distance_field_drop_shadow_image_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type,
            vs_font_distance_field_drop_shadow_image_mtl,
            sizeof(vs_font_distance_field_drop_shadow_image_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::EmbeddedProgram<
    VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE>::
    fragmentEmbeddedShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            fs_font_distance_field_drop_shadow_image_essl,
            sizeof(fs_font_distance_field_drop_shadow_image_essl)};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            fs_font_distance_field_drop_shadow_image_glsl,
            sizeof(fs_font_distance_field_drop_shadow_image_glsl)};
    case bgfx::RendererType::Vulkan:
        return {
            type,
            fs_font_distance_field_drop_shadow_image_spv,
            sizeof(fs_font_distance_field_drop_shadow_image_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            fs_font_distance_field_drop_shadow_image_dx11,
            sizeof(fs_font_distance_field_drop_shadow_image_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type,
            fs_font_distance_field_drop_shadow_image_mtl,
            sizeof(fs_font_distance_field_drop_shadow_image_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::EmbeddedProgram<
    VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE>::
    computeEmbeddedShader(bgfx::RendererType::Enum type)
{
    static_assert(true, "Invalid shader type for Font Basic");
    return {type, nullptr, 0};
}

} // namespace vcl
