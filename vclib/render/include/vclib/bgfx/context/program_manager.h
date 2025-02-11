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

#ifndef VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H
#define VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H

#include <vclib/bgfx/programs/embedded_vf_programs.h>
#include <vclib/bgfx/programs/load_program.h>
#include <vclib/types.h>

#include <array>

namespace vcl {

class ProgramManager
{
    bgfx::RendererType::Enum mRenderType = bgfx::RendererType::Count;

    std::array<bgfx::ProgramHandle, toUnderlying(VertFragProgram::COUNT)> mPrograms;

public:
    ProgramManager(bgfx::RendererType::Enum renderType) :
            mRenderType(renderType)
    {
        mPrograms.fill(BGFX_INVALID_HANDLE);
    }

    ~ProgramManager()
    {
        for (const auto& program : mPrograms) {
            if (bgfx::isValid(program)) {
                bgfx::destroy(program);
            }
        }
    }

    template<VertFragProgram PROGRAM>
    bgfx::ProgramHandle getProgram()
    {
        uint p = toUnderlying(PROGRAM);
        if (!bgfx::isValid(mPrograms[p])) {
            mPrograms[p] = vcl::createProgram(
                vcl::loadShader(Loader<PROGRAM>::vertexEmbeddedShader(
                    mRenderType)),
                vcl::loadShader(
                    Loader<PROGRAM>::fragmentEmbeddedShader(
                        mRenderType)));
        }
        return mPrograms[p];
    }
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H
