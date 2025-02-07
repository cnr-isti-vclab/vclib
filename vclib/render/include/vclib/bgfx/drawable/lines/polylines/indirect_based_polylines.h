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

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/lines/common/lines.h>

namespace vcl::lines {

class IndirectBasedPolylines : public Lines
{
    static const inline std::vector<float> VERTICES =
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    static const inline std::vector<uint> INDICES = {0, 3, 1, 0, 2, 3};

    bgfx::ProgramHandle mComputeIndirectPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INDIRECT_BASED_CS);

    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INDIRECT_BASED_VSFS);

    bgfx::ProgramHandle mJoinsPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_INDIRECT_BASED_JOINS_VSFS);

    VertexBuffer mVertices;
    IndexBuffer  mIndices;

    VertexBuffer mPoints;

    IndirectBuffer mSegmentsIndirect;
    IndirectBuffer mJointsIndirect;
    Uniform mIndirectData = Uniform("u_IndirectData", bgfx::UniformType::Vec4);

public:
    IndirectBasedPolylines();

    IndirectBasedPolylines(const std::vector<LinesVertex>& points);

    void swap(IndirectBasedPolylines& other);

    friend void swap(IndirectBasedPolylines& a, IndirectBasedPolylines& b)
    {
        a.swap(b);
    }

    void draw(uint viewId) const;

    void update(const std::vector<LinesVertex>& points);

private:
    void checkCaps() const
    {
        const bgfx::Caps* caps = bgfx::getCaps();
        const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
        const bool indirectSupported =
            bool(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
        const bool instancingSupported =
            bool(caps->supported & BGFX_CAPS_INSTANCING);

        if (!(instancingSupported && computeSupported && indirectSupported)) {
            throw std::runtime_error("Instancing or compute are not supported");
        }
    }

    void allocateAndSetPointsBuffer(const std::vector<LinesVertex>& points);

    void generateIndirectBuffers(uint pointSize);
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_POLYLINES_INDIRECT_BASED_POLYLINES_H
