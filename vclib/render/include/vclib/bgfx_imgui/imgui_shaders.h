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

#ifndef VCL_BGFX_IMGUI_IMGUI_SHADERS_H
#define VCL_BGFX_IMGUI_IMGUI_SHADERS_H

#include <vclib/bgfx/context/load_program.h>

#include <bgfx_imgui/fs_imgui_image.sc.glsl.bin.h>
#include <bgfx_imgui/vs_imgui_image.sc.glsl.bin.h>
#include <bgfx_imgui/fs_ocornut_imgui.sc.glsl.bin.h>
#include <bgfx_imgui/vs_ocornut_imgui.sc.glsl.bin.h>

#include <bgfx_imgui/fs_imgui_image.sc.essl.bin.h>
#include <bgfx_imgui/vs_imgui_image.sc.essl.bin.h>
#include <bgfx_imgui/fs_ocornut_imgui.sc.essl.bin.h>
#include <bgfx_imgui/vs_ocornut_imgui.sc.essl.bin.h>

#include <bgfx_imgui/fs_imgui_image.sc.spv.bin.h>
#include <bgfx_imgui/vs_imgui_image.sc.spv.bin.h>
#include <bgfx_imgui/fs_ocornut_imgui.sc.spv.bin.h>
#include <bgfx_imgui/vs_ocornut_imgui.sc.spv.bin.h>

#ifdef _WIN32
#include <bgfx_imgui/fs_imgui_image.sc.dx11.bin.h>
#include <bgfx_imgui/vs_imgui_image.sc.dx11.bin.h>
#include <bgfx_imgui/fs_ocornut_imgui.sc.dx11.bin.h>
#include <bgfx_imgui/vs_ocornut_imgui.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <bgfx_imgui/fs_imgui_image.sc.mtl.bin.h>
#include <bgfx_imgui/vs_imgui_image.sc.mtl.bin.h>
#include <bgfx_imgui/fs_ocornut_imgui.sc.mtl.bin.h>
#include <bgfx_imgui/vs_ocornut_imgui.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

struct ImGuiShaders
{
    enum Enum {ORCONUT_IMGUI, IMGUI_IMAGE};
};

namespace detail {

bgfx::EmbeddedShader::Data vertOrconutImgui(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, vs_ocornut_imgui_essl, sizeof(vs_ocornut_imgui_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, vs_ocornut_imgui_glsl, sizeof(vs_ocornut_imgui_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, vs_ocornut_imgui_spv, sizeof(vs_ocornut_imgui_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, vs_ocornut_imgui_dx11, sizeof(vs_ocornut_imgui_dx11)};
    case bgfx::RendererType::Direct3D12:
        // TODO
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, vs_ocornut_imgui_mtl, sizeof(vs_ocornut_imgui_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data fragOrconutImgui(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, fs_ocornut_imgui_essl, sizeof(fs_ocornut_imgui_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, fs_ocornut_imgui_glsl, sizeof(fs_ocornut_imgui_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, fs_ocornut_imgui_spv, sizeof(fs_ocornut_imgui_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, fs_ocornut_imgui_dx11, sizeof(fs_ocornut_imgui_dx11)};
    case bgfx::RendererType::Direct3D12:
        // TODO
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, fs_ocornut_imgui_mtl, sizeof(fs_ocornut_imgui_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::ProgramHandle loadOrconutImgui(bgfx::RendererType::Enum type)
{
    return vcl::createProgram(
        vcl::loadShader(vertOrconutImgui(type)),
        vcl::loadShader(fragOrconutImgui(type)));
}

bgfx::EmbeddedShader::Data vertImguiImage(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, vs_imgui_image_essl, sizeof(vs_imgui_image_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, vs_imgui_image_glsl, sizeof(vs_imgui_image_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, vs_imgui_image_spv, sizeof(vs_imgui_image_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, vs_imgui_image_dx11, sizeof(vs_imgui_image_dx11)};
    case bgfx::RendererType::Direct3D12:
        // TODO
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, vs_imgui_image_mtl, sizeof(vs_imgui_image_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data fragImguiImage(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, fs_imgui_image_essl, sizeof(fs_imgui_image_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, fs_imgui_image_glsl, sizeof(fs_imgui_image_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, fs_imgui_image_spv, sizeof(fs_imgui_image_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, fs_imgui_image_dx11, sizeof(fs_imgui_image_dx11)};
    case bgfx::RendererType::Direct3D12:
        // TODO
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, fs_imgui_image_mtl, sizeof(fs_imgui_image_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::ProgramHandle loadImguiImage(bgfx::RendererType::Enum type)
{
    return vcl::createProgram(
        vcl::loadShader(vertImguiImage(type)),
        vcl::loadShader(fragImguiImage(type)));
}

} // namespace detail

bgfx::ProgramHandle loadProgram(ImGuiShaders::Enum program, bgfx::RendererType::Enum type)
{
    switch (program) {
    case ImGuiShaders::ORCONUT_IMGUI:
        return detail::loadOrconutImgui(type);
    case ImGuiShaders::IMGUI_IMAGE:
        return detail::loadImguiImage(type);
    default: return BGFX_INVALID_HANDLE;
    }
}

} // namespace vcl

#endif // VCL_BGFX_IMGUI_IMGUI_SHADERS_H
