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

#ifndef VCL_BGFX_DRAWABLE_LINES_LINES_INDIRECT_BASED_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_LINES_INDIRECT_BASED_LINES_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/lines/common/lines.h>
#include <vclib/bgfx/uniform.h>

namespace vcl::lines {

class IndirectBasedLines : public Lines
{
    static inline const std::vector<float> VERTICES =
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    static inline const std::vector<uint> INDICES = {0, 1, 2, 1, 3, 2};

    bgfx::ProgramHandle mComputeIndirectPH =
        Context::instance().programManager().getProgram(
            VclProgram::LINES_INDIRECT_BASED_CS);
    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::LINES_INDIRECT_BASED_VSFS);

    VertexBuffer mVertices;
    IndexBuffer  mIndices;

    bgfx::DynamicVertexBufferHandle mPointsBH   = BGFX_INVALID_HANDLE;

    bgfx::IndirectBufferHandle mIndirectBH     = BGFX_INVALID_HANDLE;
    Uniform mIndirectData = Uniform("u_IndirectData", bgfx::UniformType::Vec4);

public:
    IndirectBasedLines();

    IndirectBasedLines(const std::vector<LinesVertex>& points);

    IndirectBasedLines(const IndirectBasedLines& other) = delete;

    IndirectBasedLines(IndirectBasedLines&& other);

    ~IndirectBasedLines();

    IndirectBasedLines& operator=(const IndirectBasedLines& other) = delete;

    IndirectBasedLines& operator=(IndirectBasedLines&& other);

    void swap(IndirectBasedLines& other);

    friend void swap(IndirectBasedLines& a, IndirectBasedLines& b)
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

    void generateIndirectBuffer(uint pointSize);
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_LINES_INDIRECT_BASED_LINES_H
