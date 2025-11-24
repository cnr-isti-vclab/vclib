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

#include <vclib/bgfx/programs/embedded_vf_programs/visible_face_selection_add_p2.h>

#include <vclib/shaders/selection/face_visible_add/pass_2/fs_visible_face_add_p2.sc.glsl.bin.h>
#include <vclib/shaders/selection/face_visible_add/pass_2/vs_visible_face_add_p2.sc.glsl.bin.h>

#include <vclib/shaders/selection/face_visible_add/pass_2/fs_visible_face_add_p2.sc.essl.bin.h>
#include <vclib/shaders/selection/face_visible_add/pass_2/vs_visible_face_add_p2.sc.essl.bin.h>

#include <vclib/shaders/selection/face_visible_add/pass_2/fs_visible_face_add_p2.sc.spv.bin.h>
#include <vclib/shaders/selection/face_visible_add/pass_2/vs_visible_face_add_p2.sc.spv.bin.h>
#ifdef _WIN32
#include <vclib/shaders/selection/face_visible_add/pass_2/fs_visible_face_add_p2.sc.dx11.bin.h>
#include <vclib/shaders/selection/face_visible_add/pass_2/vs_visible_face_add_p2.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <vclib/shaders/selection/face_visible_add/pass_2/fs_visible_face_add_p2.sc.mtl.bin.h>
#include <vclib/shaders/selection/face_visible_add/pass_2/vs_visible_face_add_p2.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data VertFragLoader<
    VertFragProgram::VISIBLE_FACE_SELECTION_ADD_P2>::vertexShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, vs_visible_face_add_p2_essl, sizeof(vs_visible_face_add_p2_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, vs_visible_face_add_p2_glsl, sizeof(vs_visible_face_add_p2_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, vs_visible_face_add_p2_spv, sizeof(vs_visible_face_add_p2_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, vs_visible_face_add_p2_dx11, sizeof(vs_visible_face_add_p2_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, vs_visible_face_add_p2_mtl, sizeof(vs_visible_face_add_p2_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data VertFragLoader<VertFragProgram::VISIBLE_FACE_SELECTION_ADD_P2>::
    fragmentShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, fs_visible_face_add_p2_essl, sizeof(fs_visible_face_add_p2_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, fs_visible_face_add_p2_glsl, sizeof(fs_visible_face_add_p2_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, fs_visible_face_add_p2_spv, sizeof(fs_visible_face_add_p2_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, fs_visible_face_add_p2_dx11, sizeof(fs_visible_face_add_p2_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, fs_visible_face_add_p2_mtl, sizeof(fs_visible_face_add_p2_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl
