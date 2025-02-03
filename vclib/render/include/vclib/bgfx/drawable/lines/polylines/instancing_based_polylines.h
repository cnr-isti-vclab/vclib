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

#ifndef VCL_BGFX_DRAWABLE_LINES_POLYLINES_INSTANCING_BASED_POLYLINES_H
#define VCL_BGFX_DRAWABLE_LINES_POLYLINES_INSTANCING_BASED_POLYLINES_H

#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {

class InstancingBasedPolylines : public DrawablePolylines
{
    static const inline std::vector<float> VERTICES =
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    static const inline std::vector<uint32_t> INDICES = {0, 3, 1, 0, 2, 3};

    bgfx::ProgramHandle mJoinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INSTANCING_BASED_JOINS_VSFS);

    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INSTANCING_BASED_VSFS);

    std::vector<LinesVertex> mPoints;

    mutable bgfx::InstanceDataBuffer mSegmentsInstanceDB;
    mutable bgfx::InstanceDataBuffer mJoinsInstanceDB;

    bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  mIndexesBH  = BGFX_INVALID_HANDLE;

public:
    InstancingBasedPolylines() = default;

    InstancingBasedPolylines(const std::vector<LinesVertex>& points);

    InstancingBasedPolylines(const InstancingBasedPolylines& other);

    InstancingBasedPolylines(InstancingBasedPolylines&& other);

    ~InstancingBasedPolylines();

    InstancingBasedPolylines& operator=(InstancingBasedPolylines other);

    void swap(InstancingBasedPolylines& other);

    std::shared_ptr<vcl::DrawableObject> clone() const override;

    void draw(uint viewId) const override;

    void update(const std::vector<LinesVertex>& points) override;

private:
    void generateInstanceBuffer() const;

    void allocateVerticesBuffer();

    void allocateIndexesBuffer();
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_POLYLINES_INSTANCING_BASED_POLYLINES_H
