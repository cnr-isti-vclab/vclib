/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_EXT_BGFX_DRAWABLE_MESH_PROGRAM_H
#define VCL_EXT_BGFX_DRAWABLE_MESH_PROGRAM_H

#include "load_program.h"

namespace vcl::bgf {

class DrawableMeshProgram
{
    bgfx::ProgramHandle p = BGFX_INVALID_HANDLE;

public:
    DrawableMeshProgram()
    {
        p = vcl::bgf::loadProgram(
                "include/vclib/ext/bgfx/drawable_mesh/shaders/vs_mesh",
                "include/vclib/ext/bgfx/drawable_mesh/shaders/fs_mesh",
                VCLIB_RELATIVE_SHADERS_PATH);
    }

    DrawableMeshProgram(
        const std::string& vs,
        const std::string& fs,
        const std::string& basePath = "")
    {
        p = loadProgram(vs, fs, basePath);
    };

    ~DrawableMeshProgram()
    {
        if (bgfx::isValid(p)) {
            bgfx::destroy(p);
        }
    };

    DrawableMeshProgram(const DrawableMeshProgram&) = delete;

    DrawableMeshProgram& operator=(const DrawableMeshProgram&) = delete;

    bgfx::ProgramHandle program() const
    {
        return p;
    };
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_MESH_PROGRAM_H
