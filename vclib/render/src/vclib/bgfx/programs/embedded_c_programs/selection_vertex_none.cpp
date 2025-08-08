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

#include <vclib/bgfx/programs/embedded_c_programs/selection_vertex_none.h>

#include <vclib/shaders/selection/cs_selection_vertex_none.sc.400.bin.h>

#include <vclib/shaders/selection/cs_selection_vertex_none.sc.essl.bin.h>

#include <vclib/shaders/selection/cs_selection_vertex_none.sc.spv.bin.h>

#ifdef _WIN32
#include <vclib/shaders/selection/cs_selection_vertex_none.sc.dx11.bin.h>

#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <vclib/shaders/selection/cs_selection_vertex_none.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data vcl::ComputeLoader<ComputeProgram::SELECTION_VERTEX_NONE>::
    computeShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, cs_selection_vertex_none_essl, sizeof(cs_selection_vertex_none_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, cs_selection_vertex_none_400, sizeof(cs_selection_vertex_none_400)};
    case bgfx::RendererType::Vulkan:
        return {type, cs_selection_vertex_none_spv, sizeof(cs_selection_vertex_none_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, cs_selection_vertex_none_dx11, sizeof(cs_selection_vertex_none_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, cs_selection_vertex_none_mtl, sizeof(cs_selection_vertex_none_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl
