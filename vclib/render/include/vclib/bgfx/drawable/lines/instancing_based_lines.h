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

#ifndef VCL_BGFX_DRAWABLE_LINES_INSTANCING_BASED_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_INSTANCING_BASED_LINES_H

#include "line_settings.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/lines_common/lines.h>

namespace vcl {

class InstancingBasedLines : public Lines<LineSettings>
{
    static inline const std::vector<float> VERTICES =
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    static inline const std::vector<uint> INDICES = {0, 1, 2, 1, 3, 2};

    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::LINES_INSTANCING_BASED_VSFS>();

    std::vector<LinesVertex> mPoints;

    VertexBuffer mVertices;
    IndexBuffer  mIndices;

    mutable bgfx::InstanceDataBuffer mInstanceDB;

public:
    InstancingBasedLines();

    InstancingBasedLines(const std::vector<LinesVertex>& points);

    void swap(InstancingBasedLines& other);

    void draw(uint viewId) const;

    void setPoints(const std::vector<LinesVertex>& points);

private:
    void checkCaps() const
    {
        const bgfx::Caps* caps = bgfx::getCaps();
        const bool instancingSupported =
            bool(caps->supported & BGFX_CAPS_INSTANCING);

        if (!instancingSupported) {
            throw std::runtime_error("Instancing or compute are not supported");
        }
    }

    void generateInstanceDataBuffer() const;
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_LINES_INSTANCING_BASED_LINES_H
