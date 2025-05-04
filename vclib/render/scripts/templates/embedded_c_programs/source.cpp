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

#include <vclib/bgfx/programs/embedded_c_programs/%PR_NAME_LC%.h>

#include <vclib/shaders/%PR_CS_PATH%.400.bin.h>

#include <vclib/shaders/%PR_CS_PATH%.essl.bin.h>

#include <vclib/shaders/%PR_CS_PATH%.spv.bin.h>

#ifdef _WIN32
#include <vclib/shaders/%PR_CS_PATH%.dx11.bin.h>

#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <vclib/shaders/%PR_CS_PATH%.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data vcl::ComputeLoader<ComputeProgram::%PR_NAME_UC%>::
    computeShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, %PR_CS_NAME%_essl, sizeof(%PR_CS_NAME%_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, %PR_CS_NAME%_400, sizeof(%PR_CS_NAME%_400)};
    case bgfx::RendererType::Vulkan:
        return {type, %PR_CS_NAME%_spv, sizeof(%PR_CS_NAME%_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, %PR_CS_NAME%_dx11, sizeof(%PR_CS_NAME%_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, %PR_CS_NAME%_mtl, sizeof(%PR_CS_NAME%_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl
