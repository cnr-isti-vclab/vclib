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

#include <vclib/bgfx/programs/embedded_c_programs.h>
#include <vclib/bgfx/programs/embedded_vf_programs.h>
#include <vclib/bgfx/programs/load_program.h>
#include <vclib/base.h>

#include <array>

namespace vcl {

class ProgramManager
{
    bgfx::RendererType::Enum mRenderType = bgfx::RendererType::Count;

    std::array<bgfx::ProgramHandle, toUnderlying(VertFragProgram::COUNT)>
        mVFPrograms;

    std::array<bgfx::ProgramHandle, toUnderlying(ComputeProgram::COUNT)>
        mCPrograms;

public:
    ProgramManager(bgfx::RendererType::Enum renderType) :
            mRenderType(renderType)
    {
        mVFPrograms.fill(BGFX_INVALID_HANDLE);
        mCPrograms.fill(BGFX_INVALID_HANDLE);
    }

    ~ProgramManager()
    {
        for (const auto& program : mVFPrograms) {
            if (bgfx::isValid(program)) {
                bgfx::destroy(program);
            }
        }

        for (const auto& program : mCPrograms) {
            if (bgfx::isValid(program)) {
                bgfx::destroy(program);
            }
        }
    }

    template<VertFragProgram PROGRAM>
    bgfx::ProgramHandle getProgram()
    {
        uint p = toUnderlying(PROGRAM);
        if (!bgfx::isValid(mVFPrograms[p])) {
            mVFPrograms[p] = vcl::createProgram(
                vcl::loadShader(
                    VertFragLoader<PROGRAM>::vertexShader(mRenderType)),
                vcl::loadShader(
                    VertFragLoader<PROGRAM>::fragmentShader(mRenderType)));
            assert(bgfx::isValid(mVFPrograms[p]));
        }
        return mVFPrograms[p];
    }

    template<ComputeProgram PROGRAM>
    bgfx::ProgramHandle getComputeProgram()
    {
        uint p = toUnderlying(PROGRAM);
        if (!bgfx::isValid(mCPrograms[p])) {
            mCPrograms[p] = vcl::createProgram(vcl::loadShader(
                ComputeLoader<PROGRAM>::computeShader(mRenderType)));
        }
        return mCPrograms[p];
    }
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H
