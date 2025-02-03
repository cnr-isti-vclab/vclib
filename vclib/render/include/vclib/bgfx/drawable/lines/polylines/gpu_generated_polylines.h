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

#ifndef VCL_BGFX_DRAWABLE_LINES_POLYLINES_GPU_GENERATED_POLYLINES_H
#define VCL_BGFX_DRAWABLE_LINES_POLYLINES_GPU_GENERATED_POLYLINES_H

#include <vclib/bgfx/drawable/lines/lines_settings.h>
#include <vclib/bgfx/context.h>

namespace vcl::lines {

class GPUGeneratedPolylines
{
    bgfx::ProgramHandle mComputeVertexPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_GPU_GENERATED_CS);

    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_CPU_GENERATED_VSFS);

    LinesSettings mSettings;

    std::vector<LinesVertex> mPoints;

    bgfx::DynamicVertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle mVertexBH = BGFX_INVALID_HANDLE;

    bgfx::DynamicIndexBufferHandle mSegmentsIndexesBH = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle mJoinesIndexesBH   = BGFX_INVALID_HANDLE;

    bgfx::UniformHandle mComputeDataUH = BGFX_INVALID_HANDLE;

public:
    GPUGeneratedPolylines() { checkCaps(); }

    GPUGeneratedPolylines(const std::vector<LinesVertex>& points);

    GPUGeneratedPolylines(const GPUGeneratedPolylines& other) = delete;

    GPUGeneratedPolylines(GPUGeneratedPolylines&& other);

    ~GPUGeneratedPolylines();

    GPUGeneratedPolylines& operator=(const GPUGeneratedPolylines& other) =
        delete;

    GPUGeneratedPolylines& operator=(GPUGeneratedPolylines&& other);

    void swap(GPUGeneratedPolylines& other);

    LinesSettings& settings() { return mSettings; }

    const LinesSettings& settings() const { return mSettings; }

    void draw(uint viewId) const;

    void update(const std::vector<LinesVertex>& points);

private:
    void generateBuffers();

    void allocateVertexBuffer();

    void allocateIndexBuffer();

    void allocatePointsBuffer();

    void checkCaps() const
    {
        const bgfx::Caps* caps = bgfx::getCaps();
        const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
        if (!computeSupported) {
            throw std::runtime_error("GPU compute not supported");
        }
    }
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_POLYLINES_GPU_GENERATED_POLYLINES_H
