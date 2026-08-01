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

/**
 * @class ProgramManager
 * @brief Manages the creation, retrieval, and lifetime of bgfx programs.
 *
 * This class handles both vertex-fragment programs and compute programs,
 * loading them lazily (on demand) and caching their handles.
 */
class ProgramManager
{
    /// The bgfx renderer type used to load the correct shader binaries.
    bgfx::RendererType::Enum mRenderType = bgfx::RendererType::Count;

    /// Cache of Vertex/Fragment programs.
    std::array<bgfx::ProgramHandle, toUnderlying(VertFragProgram::COUNT)>
        mVFPrograms;

    /// Cache of Compute programs.
    std::array<bgfx::ProgramHandle, toUnderlying(ComputeProgram::COUNT)>
        mCPrograms;

public:
    /**
     * @brief Constructs a ProgramManager.
     * @param[in] renderType: The bgfx renderer type used to select the correct
     * shader binaries.
     */
    ProgramManager(bgfx::RendererType::Enum renderType) :
            mRenderType(renderType)
    {
        mVFPrograms.fill(BGFX_INVALID_HANDLE);
        mCPrograms.fill(BGFX_INVALID_HANDLE);
    }

    /**
     * @brief Destructor. Destroys all cached bgfx programs.
     */
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

    /**
     * @brief Retrieves the bgfx program handle for a specific Vertex/Fragment
     * program template argument.
     *
     * Creates and caches the program if it has not been loaded yet.
     *
     * @tparam PROGRAM: The Vertex/Fragment program enum value.
     * @return The bgfx program handle.
     */
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

    /**
     * @brief Retrieves the bgfx program handle for a given Vertex/Fragment
     * program enum value.
     *
     * Creates and caches the program if it has not been loaded yet.
     *
     * @param[in] p: The Vertex/Fragment program enum value.
     * @return The bgfx program handle.
     */
    bgfx::ProgramHandle getProgram(VertFragProgram p)
    {
        return getProgramImpl(
            p,
            std::make_index_sequence<toUnderlying(VertFragProgram::COUNT)> {});
    }

    /**
     * @brief Retrieves the bgfx program handle for a specific Compute program
     * template argument.
     *
     * Creates and caches the program if it has not been loaded yet.
     *
     * @tparam PROGRAM: The Compute program enum value.
     * @return The bgfx program handle.
     */
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

    /**
     * @brief Retrieves the bgfx program handle for a given Compute program enum
     * value.
     *
     * Creates and caches the program if it has not been loaded yet.
     *
     * @param[in] p: The Compute program enum value.
     * @return The bgfx program handle.
     */
    bgfx::ProgramHandle getComputeProgram(ComputeProgram p)
    {
        return getComputeProgramImpl(
            p,
            std::make_index_sequence<toUnderlying(ComputeProgram::COUNT)> {});
    }

private:
    /**
     * @brief Helper template method to retrieve the program handle dynamically.
     *
     * Uses index sequences to map the dynamic enum value to the static template
     * version.
     *
     * @tparam Is Index sequence pack.
     * @param[in] p: The Vertex/Fragment program enum value.
     * @return The bgfx program handle.
     */
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

    /**
     * @brief Helper template method to retrieve the compute program handle
     * dynamically.
     *
     * Uses index sequences to map the dynamic enum value to the static template
     * version.
     *
     * @tparam Is Index sequence pack.
     * @param[in] p: The Compute program enum value.
     * @return The bgfx program handle.
     */
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
