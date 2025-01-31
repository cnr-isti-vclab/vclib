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

#ifndef VCL_BGFX_DRAWABLE_LINES_LINES_GPU_GENERATED_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_LINES_GPU_GENERATED_LINES_H

#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {

class GPUGeneratedLines : public DrawableLines
{
    bgfx::ProgramHandle mComputeVerticesPH =
        Context::instance().programManager().getProgram(
            VclProgram::LINES_GPU_GENERATED_CS);

    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::LINES_CPU_GENERATED_VSFS);

    std::vector<LinesVertex> mPoints;

    bgfx::DynamicIndexBufferHandle  mIndexesBH  = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle mPointsBH   = BGFX_INVALID_HANDLE;

public:
    GPUGeneratedLines() = default;

    GPUGeneratedLines(const std::vector<LinesVertex>& points);

    GPUGeneratedLines(const GPUGeneratedLines& other);

    GPUGeneratedLines(GPUGeneratedLines&& other);

    ~GPUGeneratedLines();

    GPUGeneratedLines& operator=(GPUGeneratedLines other);

    void swap(GPUGeneratedLines& other);

    std::shared_ptr<vcl::DrawableObject> clone() const override;

    void draw(uint viewId) const override;

    void update(const std::vector<LinesVertex>& points) override;

private:
    void generateBuffers();

    void allocateVertexBuffer();

    void allocateIndexBuffer();

    void allocatePointsBuffer();
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_LINES_GPU_GENERATED_LINES_H
