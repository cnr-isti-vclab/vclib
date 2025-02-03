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

#ifndef VCL_BGFX_DRAWABLE_LINES_POLYLINES_INDIRECT_BASED_POLYLINES_H
#define VCL_BGFX_DRAWABLE_LINES_POLYLINES_INDIRECT_BASED_POLYLINES_H

#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {

class IndirectBasedPolylines : public DrawablePolylines
{
    static const inline std::vector<float> VERTICES =
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    static const inline std::vector<uint32_t> INDICES = {0, 3, 1, 0, 2, 3};

    bgfx::ProgramHandle mJoinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INDIRECT_BASED_JOINS_VSFS);

    bgfx::ProgramHandle mComputeIndirectPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INDIRECT_BASED_CS);

    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INDIRECT_BASED_VSFS);

    std::vector<LinesVertex> mPoints;

    bgfx::VertexBufferHandle        mVerticesBH         = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle         mIndexesBH          = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle mPointsBH           = BGFX_INVALID_HANDLE;
    bgfx::IndirectBufferHandle      mSegmentsIndirectBH = BGFX_INVALID_HANDLE;
    bgfx::IndirectBufferHandle      mJoinesIndirectBH   = BGFX_INVALID_HANDLE;

    bgfx::UniformHandle mComputeIndirectDataUH = BGFX_INVALID_HANDLE;

public:
    IndirectBasedPolylines() = default;

    IndirectBasedPolylines(const std::vector<LinesVertex>& points);

    IndirectBasedPolylines(const IndirectBasedPolylines& other);

    IndirectBasedPolylines(IndirectBasedPolylines&& other);

    ~IndirectBasedPolylines();

    IndirectBasedPolylines& operator=(IndirectBasedPolylines other);

    void swap(IndirectBasedPolylines& other);

    std::shared_ptr<vcl::DrawableObject> clone() const override;

    void draw(uint viewId) const override;

    void update(const std::vector<LinesVertex>& points) override;

private:
    void generateIndirectBuffers();

    void allocatePointsBuffer();

    void allocateVerticesBuffer();

    void allocateIndexesBuffers();
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_POLYLINES_INDIRECT_BASED_POLYLINES_H
