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

#include <vclib/bgfx/programs/embedded_c_programs/selection_face.h>

#include <vclib/bgfx/programs/macros.h>

// clang-format off
#define COMMON_PATH vclib/shaders/selection
// clang-format on
#define CS_NAME cs_selection_face

#include VCLIB_BGFX_SHADER(COMMON_PATH, glsl, CS_NAME.sc.bin.h)

#include VCLIB_BGFX_SHADER(COMMON_PATH, essl, CS_NAME.sc.bin.h)

#include VCLIB_BGFX_SHADER(COMMON_PATH, spirv, CS_NAME.sc.bin.h)

#ifdef _WIN32
#include VCLIB_BGFX_SHADER(COMMON_PATH, dxbc, CS_NAME.sc.bin.h)

// #include VCLIB_BGFX_SHADER(COMMON_PATH, dxil, CS_NAME.sc.bin.h)
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include VCLIB_BGFX_SHADER(COMMON_PATH, mtl, CS_NAME.sc.bin.h)
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data vcl::
    ComputeLoader<ComputeProgram::SELECTION_FACE>::computeShader(
        bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {
            type, VCLIB_JOIN(CS_NAME, essl), sizeof(VCLIB_JOIN(CS_NAME, essl))};
    case bgfx::RendererType::OpenGL:
        return {
            type, VCLIB_JOIN(CS_NAME, glsl), sizeof(VCLIB_JOIN(CS_NAME, glsl))};
    case bgfx::RendererType::Vulkan:
        return {
            type, VCLIB_JOIN(CS_NAME, spv), sizeof(VCLIB_JOIN(CS_NAME, spv))};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type, VCLIB_JOIN(CS_NAME, dxbc), sizeof(VCLIB_JOIN(CS_NAME, dxbc))};
    case bgfx::RendererType::Direct3D12:
        // return {
        //     type, VCLIB_JOIN(CS_NAME, dxil), sizeof(VCLIB_JOIN(CS_NAME,
        //     dxil))};
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {
            type, VCLIB_JOIN(CS_NAME, mtl), sizeof(VCLIB_JOIN(CS_NAME, mtl))};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl
