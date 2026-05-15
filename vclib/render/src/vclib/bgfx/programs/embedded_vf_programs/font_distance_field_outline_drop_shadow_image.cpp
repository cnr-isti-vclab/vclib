/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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
 *****************************************************************************/

#include <vclib/bgfx/programs/embedded_vf_programs/font_distance_field_outline_drop_shadow_image.h>

#include <vclib/bgfx/programs/macros.h>

// clang-format off
#define COMMON_PATH vclib/shaders/font
// clang-format on
#define VS_NAME vs_font_distance_field_outline_drop_shadow_image
#define FS_NAME fs_font_distance_field_outline_drop_shadow_image

#include VCLIB_BGFX_SHADER(COMMON_PATH, glsl, VS_NAME.sc.bin.h)
#include VCLIB_BGFX_SHADER(COMMON_PATH, glsl, FS_NAME.sc.bin.h)

#include VCLIB_BGFX_SHADER(COMMON_PATH, essl, VS_NAME.sc.bin.h)
#include VCLIB_BGFX_SHADER(COMMON_PATH, essl, FS_NAME.sc.bin.h)

#include VCLIB_BGFX_SHADER(COMMON_PATH, spirv, VS_NAME.sc.bin.h)
#include VCLIB_BGFX_SHADER(COMMON_PATH, spirv, FS_NAME.sc.bin.h)

#ifdef _WIN32
#include VCLIB_BGFX_SHADER(COMMON_PATH, dxbc, VS_NAME.sc.bin.h)
#include VCLIB_BGFX_SHADER(COMMON_PATH, dxbc, FS_NAME.sc.bin.h)

// #include VCLIB_BGFX_SHADER(COMMON_PATH, dxil, VS_NAME.sc.bin.h)
// #include VCLIB_BGFX_SHADER(COMMON_PATH, dxil, FS_NAME.sc.bin.h)
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include VCLIB_BGFX_SHADER(COMMON_PATH, mtl, VS_NAME.sc.bin.h)
#include VCLIB_BGFX_SHADER(COMMON_PATH, mtl, FS_NAME.sc.bin.h)
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data VertFragLoader<
    VertFragProgram::FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE>::
    vertexShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, essl),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, essl))};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, glsl),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, glsl))};
    case bgfx::RendererType::Vulkan:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, spv),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, spv))};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, dxbc),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, dxbc))};
    case bgfx::RendererType::Direct3D12:
        // return {
        //     type,
        //     VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, dxil),
        //     sizeof(VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, dxil))};
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, mtl),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(VS_NAME, mtl))};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data VertFragLoader<
    VertFragProgram::FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE>::
    fragmentShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, essl),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, essl))};
    case bgfx::RendererType::OpenGL:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, glsl),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, glsl))};
    case bgfx::RendererType::Vulkan:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, spv),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, spv))};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, dxbc),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, dxbc))};
    case bgfx::RendererType::Direct3D12:
        // return {
        //     type,
        //     VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, dxil),
        //     sizeof(VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, dxil))};
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type,
            VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, mtl),
            sizeof(VCLIB_JOIN_WITH_UNDERSCORE(FS_NAME, mtl))};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl
