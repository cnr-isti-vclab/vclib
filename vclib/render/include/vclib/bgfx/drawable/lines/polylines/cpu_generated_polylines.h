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

#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {

class CPUGeneratedPolylines : public DrawablePolylines
{
    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_CPU_GENERATED_VSFS);

    uint32_t              mPointsSize;
    std::vector<float>    mVertices;
    std::vector<uint32_t> mSegmentsIndexes;
    std::vector<uint32_t> mJoinsIndexes;

    bgfx::DynamicVertexBufferHandle mVerticesBH        = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle  mSegmentsIndexesBH = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle  mJoinsIndexesBH    = BGFX_INVALID_HANDLE;

public:
    CPUGeneratedPolylines() = default;

    CPUGeneratedPolylines(const std::vector<LinesVertex>& points);

    CPUGeneratedPolylines(const CPUGeneratedPolylines& other);

    CPUGeneratedPolylines(CPUGeneratedPolylines&& other);

    ~CPUGeneratedPolylines();

    CPUGeneratedPolylines& operator=(CPUGeneratedPolylines other);

    void swap(CPUGeneratedPolylines& other);

    std::shared_ptr<vcl::DrawableObject> clone() const override;

    void draw(uint viewId) const override;

    void update(const std::vector<LinesVertex>& points) override;

private:
    void generateBuffers(const std::vector<LinesVertex> points);

    void allocateVertexBuffer();

    void allocateIndexesBuffer();
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_POLYLINES_CPU_GENERATED_POLYLINES_H
