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

#ifndef VCL_BGFX_CONTEXT_EMBEDDED_SHADERS_LINES_INDIRECT_BASED_CS_H
#define VCL_BGFX_CONTEXT_EMBEDDED_SHADERS_LINES_INDIRECT_BASED_CS_H

#include <vclib/bgfx/programs/compute_loader.h>

namespace vcl {

template<>
struct ComputeLoader<ComputeProgram::LINES_INDIRECT_BASED_CS>
{
    static bgfx::EmbeddedShader::Data computeShader(
        bgfx::RendererType::Enum type);
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_EMBEDDED_SHADERS_LINES_INDIRECT_BASED_CS_H
