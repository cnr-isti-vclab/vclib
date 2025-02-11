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

#include <vclib/bgfx/programs/embedded_vf_programs/font_distance_field_drop_shadow.h>

#include <shaders/font/fs_font_distance_field_drop_shadow.sc.glsl.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow.sc.glsl.bin.h>

#include <shaders/font/fs_font_distance_field_drop_shadow.sc.essl.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow.sc.essl.bin.h>

#include <shaders/font/fs_font_distance_field_drop_shadow.sc.spv.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow.sc.spv.bin.h>
#ifdef _WIN32
#include <shaders/font/fs_font_distance_field_drop_shadow.sc.dx11.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <shaders/font/fs_font_distance_field_drop_shadow.sc.mtl.bin.h>
#include <shaders/font/vs_font_distance_field_drop_shadow.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data VertFragLoader<VertFragProgram::FONT_DISTANCE_FIELD_DROP_SHADOW>::
    vertexShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            vs_font_distance_field_drop_shadow_essl,
            sizeof(vs_font_distance_field_drop_shadow_essl)};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            vs_font_distance_field_drop_shadow_glsl,
            sizeof(vs_font_distance_field_drop_shadow_glsl)};
    case bgfx::RendererType::Vulkan:
        return {
            type,
            vs_font_distance_field_drop_shadow_spv,
            sizeof(vs_font_distance_field_drop_shadow_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            vs_font_distance_field_drop_shadow_dx11,
            sizeof(vs_font_distance_field_drop_shadow_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type,
            vs_font_distance_field_drop_shadow_mtl,
            sizeof(vs_font_distance_field_drop_shadow_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data VertFragLoader<VertFragProgram::FONT_DISTANCE_FIELD_DROP_SHADOW>::
    fragmentShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            fs_font_distance_field_drop_shadow_essl,
            sizeof(fs_font_distance_field_drop_shadow_essl)};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            fs_font_distance_field_drop_shadow_glsl,
            sizeof(fs_font_distance_field_drop_shadow_glsl)};
    case bgfx::RendererType::Vulkan:
        return {
            type,
            fs_font_distance_field_drop_shadow_spv,
            sizeof(fs_font_distance_field_drop_shadow_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            fs_font_distance_field_drop_shadow_dx11,
            sizeof(fs_font_distance_field_drop_shadow_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type,
            fs_font_distance_field_drop_shadow_mtl,
            sizeof(fs_font_distance_field_drop_shadow_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl
