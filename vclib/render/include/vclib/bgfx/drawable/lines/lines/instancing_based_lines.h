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

#ifndef VCL_BGFX_DRAWABLE_LINES_LINES_INSTANCING_BASED_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_LINES_INSTANCING_BASED_LINES_H

#include <vclib/bgfx/drawable/lines/lines_settings.h>
#include <vclib/bgfx/context.h>

namespace vcl::lines {

class InstancingBasedLines
{
    static inline const std::vector<float> VERTICES =
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    static inline const std::vector<uint32_t> INDICES = {0, 1, 2, 1, 3, 2};

    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::LINES_INSTANCING_BASED_VSFS);

    mutable LinesSettings mSettings;

    std::vector<LinesVertex>         mPoints;
    mutable bgfx::InstanceDataBuffer mInstanceDB;

    bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  mIndexesBH  = BGFX_INVALID_HANDLE;

public:
    InstancingBasedLines() { checkCaps(); }

    InstancingBasedLines(const std::vector<LinesVertex>& points);

    InstancingBasedLines(const InstancingBasedLines& other) = delete;

    InstancingBasedLines(InstancingBasedLines&& other);

    ~InstancingBasedLines();

    InstancingBasedLines& operator=(const InstancingBasedLines& other) = delete;

    InstancingBasedLines& operator=(InstancingBasedLines&& other);

    void swap(InstancingBasedLines& other);

    LinesSettings& settings() { return mSettings; }

    const LinesSettings& settings() const { return mSettings; }

    void draw(uint viewId) const;

    void update(const std::vector<LinesVertex>& points);

private:
    void generateInstanceDataBuffer() const;

    void allocateVerticesBuffer();

    void allocateIndexesBuffer();

    void checkCaps() const
    {
        const bgfx::Caps* caps = bgfx::getCaps();
        const bool instancingSupported =
            bool(caps->supported & BGFX_CAPS_INSTANCING);

        if (!instancingSupported) {
            throw std::runtime_error("Instancing or compute are not supported");
        }
    }
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_LINES_INSTANCING_BASED_LINES_H
