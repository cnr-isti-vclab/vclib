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

#ifndef VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_AXIS_SHADER_PROGRAM_H
#define VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_AXIS_SHADER_PROGRAM_H

#include "load_program.h"

namespace vcl::bgf {

class DrawableAxisShaderProgram
{
    bgfx::ProgramHandle p = BGFX_INVALID_HANDLE;

public:
    DrawableAxisShaderProgram()
    {
        p = vcl::bgf::loadProgram(
            "vclib/ext/bgfx/drawable_axis/vs_drawable_axis",
            "vclib/ext/bgfx/drawable_axis/fs_drawable_axis");
    }

    ~DrawableAxisShaderProgram()
    {
        if (bgfx::isValid(p)) {
            bgfx::destroy(p);
        }
    };

    DrawableAxisShaderProgram(const DrawableAxisShaderProgram&) = delete;

    DrawableAxisShaderProgram& operator=(const DrawableAxisShaderProgram&) =
        delete;

    bgfx::ProgramHandle getProgram() const { return p; }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_AXIS_SHADER_PROGRAM_H
