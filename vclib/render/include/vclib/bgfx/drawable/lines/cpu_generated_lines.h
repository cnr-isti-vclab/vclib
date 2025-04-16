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

#ifndef VCL_BGFX_DRAWABLE_LINES_CPU_GENERATED_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_CPU_GENERATED_LINES_H

#include "line_settings.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/lines_common/lines.h>

namespace vcl {

class CPUGeneratedLines : public Lines<LineSettings>
{
    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::LINES>();

    VertexBuffer mVertices;
    IndexBuffer  mIndices;

public:
    CPUGeneratedLines() = default;

    CPUGeneratedLines(const std::vector<LinesVertex>& points);

    void swap(CPUGeneratedLines& other);

    friend void swap(CPUGeneratedLines& a, CPUGeneratedLines& b) { a.swap(b); }

    void setPoints(const std::vector<LinesVertex>& points);

    void draw(uint viewId) const;
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_LINES_CPU_GENERATED_LINES_H
