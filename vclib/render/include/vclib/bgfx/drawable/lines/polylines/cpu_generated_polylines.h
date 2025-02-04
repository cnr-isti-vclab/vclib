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

#ifndef VCL_BGFX_DRAWABLE_LINES_POLYLINES_CPU_GENERATED_POLYLINES_H
#define VCL_BGFX_DRAWABLE_LINES_POLYLINES_CPU_GENERATED_POLYLINES_H

#include <vclib/bgfx/drawable/lines/common/lines.h>

#include <vclib/bgfx/context.h>

namespace vcl::lines {

class CPUGeneratedPolylines : public Lines
{
    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_CPU_GENERATED_VSFS);

    uint               mPointsSize;

    bgfx::DynamicVertexBufferHandle mVerticesBH        = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle  mSegmentsIndicesBH = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle  mJoinsIndicesBH    = BGFX_INVALID_HANDLE;

public:
    CPUGeneratedPolylines() = default;

    CPUGeneratedPolylines(const std::vector<LinesVertex>& points);

    CPUGeneratedPolylines(const CPUGeneratedPolylines& other) = delete;

    CPUGeneratedPolylines(CPUGeneratedPolylines&& other);

    ~CPUGeneratedPolylines();

    CPUGeneratedPolylines& operator=(const CPUGeneratedPolylines& other) =
        delete;

    CPUGeneratedPolylines& operator=(CPUGeneratedPolylines&& other);

    void swap(CPUGeneratedPolylines& other);

    void draw(uint viewId) const;

    void update(const std::vector<LinesVertex>& points);

private:
    void generateBuffers(const std::vector<LinesVertex> points);

    void allocateVertexBuffer();

    void allocateIndicesBuffer();
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_POLYLINES_CPU_GENERATED_POLYLINES_H
