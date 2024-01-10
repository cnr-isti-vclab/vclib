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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_DRAWABLE_MESH_SHADER_PROGRAM_H
#define VCL_EXT_BGFX_DRAWABLE_MESH_SHADER_PROGRAM_H

#include <vclib/render/generic_drawable_mesh_shader_program.h>

#include "load_program.h"

namespace vcl::bgf {

class DrawableMeshShaderProgram : public GenericDrawableMeshShaderProgram
{
    bgfx::ProgramHandle p = BGFX_INVALID_HANDLE;

public:
    DrawableMeshShaderProgram()
    {
        p = vcl::bgf::loadProgram(
            "shaders/ext/bgfx/drawable_mesh/vs_mesh",
            "shaders/ext/bgfx/drawable_mesh/fs_mesh",
            VCLIB_RELATIVE_SHADERS_PATH);
    }

    DrawableMeshShaderProgram(
        const std::string& vs,
        const std::string& fs,
        const std::string& basePath = "")
    {
        p = loadProgram(vs, fs, basePath);
    };

    ~DrawableMeshShaderProgram()
    {
        if (bgfx::isValid(p)) {
            bgfx::destroy(p);
        }
    };

    DrawableMeshShaderProgram(const DrawableMeshShaderProgram&) = delete;

    DrawableMeshShaderProgram& operator=(const DrawableMeshShaderProgram&) = delete;

    bgfx::ProgramHandle program() const { return p; };
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_MESH_SHADER_PROGRAM_H
