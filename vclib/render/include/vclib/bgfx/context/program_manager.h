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

#ifndef VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H
#define VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H

#include "embedded_shaders/embedded_shader.h"

#include <map>
#include <string>

namespace vcl {

class ProgramManager
{
    bgfx::RendererType::Enum mRenderType = bgfx::RendererType::Count;
    std::map<std::string, bgfx::ProgramHandle> mPrograms;

public:
    ProgramManager(bgfx::RendererType::Enum renderType) :
            mRenderType(renderType)
    {
    }

    ~ProgramManager();

    bgfx::ProgramHandle getProgram(VclProgram::Enum program);

    bgfx::ProgramHandle getProgram(const std::string& name) const;

    bgfx::ProgramHandle loadProgram(
        const std::string& name,
        const std::string& vs,
        const std::string& fs);

private:
    static bgfx::ProgramHandle loadProgram(
        VclProgram::Enum         program,
        bgfx::RendererType::Enum type);
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H
