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

#ifndef VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_DIRECTIONAL_LIGHT_SHADER_PROGRAM_H
#define VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_DIRECTIONAL_LIGHT_SHADER_PROGRAM_H

#include "load_program.h"

#include <vclib/render/interfaces/shader_program_i.h>

namespace vcl::bgf {

class DrawableDirectionalLightShaderProgram : public ShaderProgramI
{
    bgfx::ProgramHandle p = BGFX_INVALID_HANDLE;

public:
    DrawableDirectionalLightShaderProgram()
    {
        p = vcl::bgf::loadProgram(
            "vclib/ext/bgfx/drawable_directional_light/"
            "vs_drawable_directional_light",
            "vclib/ext/bgfx/drawable_directional_light/"
            "fs_drawable_directional_light");
    }

    ~DrawableDirectionalLightShaderProgram()
    {
        if (bgfx::isValid(p)) {
            bgfx::destroy(p);
        }
    };

    DrawableDirectionalLightShaderProgram(
        const DrawableDirectionalLightShaderProgram&) = delete;

    DrawableDirectionalLightShaderProgram& operator=(
        const DrawableDirectionalLightShaderProgram&) = delete;

    bgfx::ProgramHandle program() const { return p; }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_SHADER_PROGRAMS_DRAWABLE_DIRECTIONAL_LIGHT_SHADER_PROGRAM_H
