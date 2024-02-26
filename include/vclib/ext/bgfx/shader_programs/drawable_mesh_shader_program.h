/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_MESH_SHADER_PROGRAM_H
#define VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_MESH_SHADER_PROGRAM_H

#include <vclib/render/interfaces/shader_program_i.h>

#include "load_program.h"

namespace vcl::bgf {

class DrawableMeshShaderProgram : public ShaderProgramI
{
    bgfx::ProgramHandle p = BGFX_INVALID_HANDLE;

public:
    DrawableMeshShaderProgram()
    {
        p = vcl::bgf::loadProgram(
            "vclib/ext/bgfx/drawable_mesh/vs_drawable_mesh",
            "vclib/ext/bgfx/drawable_mesh/fs_drawable_mesh");
    }

    DrawableMeshShaderProgram(const std::string& vs, const std::string& fs)
    {
        p = loadProgram(vs, fs);
    };

    ~DrawableMeshShaderProgram()
    {
        if (bgfx::isValid(p)) {
            bgfx::destroy(p);
        }
    };

    DrawableMeshShaderProgram(const DrawableMeshShaderProgram&) = delete;

    DrawableMeshShaderProgram& operator=(const DrawableMeshShaderProgram&) =
        delete;

    bgfx::ProgramHandle program() const { return p; };
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_MESH_SHADER_PROGRAM_H
