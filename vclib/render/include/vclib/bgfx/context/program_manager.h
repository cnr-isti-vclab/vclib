// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H
#define VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H

#include <vclib/base.h>
#include <vclib/bgfx/programs/embedded_c_programs.h>
#include <vclib/bgfx/programs/embedded_vf_programs.h>
#include <vclib/bgfx/programs/load_program.h>

#include <array>
#include <utility>

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

    bgfx::ProgramHandle getProgram(VertFragProgram p)
    {
        return getProgramImpl(
            p,
            std::make_index_sequence<toUnderlying(VertFragProgram::COUNT)> {});
    }

    template<ComputeProgram PROGRAM>
    bgfx::ProgramHandle getComputeProgram()
    {
        uint p = toUnderlying(PROGRAM);
        if (!bgfx::isValid(mCPrograms[p])) {
            mCPrograms[p] = vcl::createProgram(
                vcl::loadShader(
                    ComputeLoader<PROGRAM>::computeShader(mRenderType)));
        }
        return mCPrograms[p];
    }

    bgfx::ProgramHandle getComputeProgram(ComputeProgram p)
    {
        return getComputeProgramImpl(
            p,
            std::make_index_sequence<toUnderlying(ComputeProgram::COUNT)> {});
    }

private:
    template<size_t... Is>
    bgfx::ProgramHandle getProgramImpl(
        VertFragProgram p,
        std::index_sequence<Is...>)
    {
        using GetProgramFn = bgfx::ProgramHandle (ProgramManager::*)();
        static constexpr std::array<GetProgramFn, sizeof...(Is)> funcs = {
            &ProgramManager::getProgram<static_cast<VertFragProgram>(Is)>...};
        return (this->*(funcs[toUnderlying(p)]))();
    }

    template<size_t... Is>
    bgfx::ProgramHandle getComputeProgramImpl(
        ComputeProgram p,
        std::index_sequence<Is...>)
    {
        using GetProgramFn = bgfx::ProgramHandle (ProgramManager::*)();
        static constexpr std::array<GetProgramFn, sizeof...(Is)> funcs = {
            &ProgramManager::getComputeProgram<static_cast<ComputeProgram>(
                Is)>...};
        return (this->*(funcs[toUnderlying(p)]))();
    }
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_PROGRAM_MANAGER_H
