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

#ifndef VCL_BGFX_CONTEXT_EMBEDDED_PROGRAMS_DRAWABLE_MESH_SURFACE_H
#define VCL_BGFX_CONTEXT_EMBEDDED_PROGRAMS_DRAWABLE_MESH_SURFACE_H

#include <vclib/bgfx/context/embedded_program.h>

namespace vcl {

template<>
struct EmbeddedProgram<VclProgram::DRAWABLE_MESH_SURFACE>
{
    static bgfx::EmbeddedShader::Data vertexEmbeddedShader(
        bgfx::RendererType::Enum type);

    static bgfx::EmbeddedShader::Data fragmentEmbeddedShader(
        bgfx::RendererType::Enum type);

    static bgfx::EmbeddedShader::Data computeEmbeddedShader(
        bgfx::RendererType::Enum type);
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_EMBEDDED_PROGRAMS_DRAWABLE_MESH_SURFACE_H
