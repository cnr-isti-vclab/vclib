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

#ifndef VCL_BGFX_DRAWABLE_LINES_INDIRECT_BASED_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_INDIRECT_BASED_LINES_H

#include "line_settings.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/lines_common/lines.h>

namespace vcl {

class IndirectBasedLines : public Lines<LineSettings>
{
    static inline const std::vector<float> VERTICES =
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    static inline const std::vector<uint> INDICES = {0, 1, 2, 1, 3, 2};

    bgfx::ProgramHandle mComputeIndirectPH =
        Context::instance()
            .programManager()
            .getComputeProgram<ComputeProgram::LINES_INDIRECT>();

    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::LINES_INSTANCING>();


    VertexBuffer mVertCoords;
    VertexBuffer mVertColors;
    VertexBuffer mVertNormals;
    VertexBuffer mLineColors;

    VertexBuffer mVertices;
    IndexBuffer mIndices;

    VertexBuffer mInstanceData;

    uint mNumPoints;

public:
    IndirectBasedLines();

    IndirectBasedLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors,
        const std::vector<float>& vertNormals,
        const std::vector<uint>& lineColors);

    void swap(IndirectBasedLines& other);

    friend void swap(IndirectBasedLines& a, IndirectBasedLines& b)
    {
        a.swap(b);
    }

    void draw(uint viewId) const;

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors,
        const std::vector<float>& vertNormals,
        const std::vector<uint>& lineColors);

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

    void setCoordsBuffer(const std::vector<float>& vertCoords);

    void setColorsBuffer(const std::vector<uint>& vertColors);

    void setNormalsBuffer(const std::vector<float>& vertNormals);

    void setLineColorsBuffer(const std::vector<uint>& lineColors);

    void allocateInstanceData();       

    void generateInstanceDataBuffer();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_LINES_INDIRECT_BASED_LINES_H
