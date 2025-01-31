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

#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/drawable/lines/lines/gpu_generated_lines.h>
#include <vclib/bgfx/drawable/lines/lines/indirect_based_lines.h>
#include <vclib/bgfx/drawable/lines/lines/instancing_based_lines.h>
#include <vclib/bgfx/drawable/lines/lines/texture_based_lines.h>

namespace vcl::lines {

std::unique_ptr<DrawableLines> DrawableLines::create(
    const std::vector<LinesVertex>& points,
    LinesTypes                      type)
{
    const bgfx::Caps* caps = bgfx::getCaps();

    switch (type) {
    case LinesTypes::CPU_GENERATED: {
        return std::make_unique<CPUGeneratedLines>(points);
    }

    case LinesTypes::GPU_GENERATED: {
        const bool computeSupported = !!(caps->supported & BGFX_CAPS_COMPUTE);
        assert((void("GPU compute not supported"), computeSupported));
        return std::make_unique<GPUGeneratedLines>(points);
    }

    case LinesTypes::INSTANCING_BASED: {
        const bool instancingSupported =
            !!(caps->supported & BGFX_CAPS_INSTANCING);
        assert((void("Instancing not supported"), instancingSupported));
        return std::make_unique<InstancingBasedLines>(points);
    }

    case LinesTypes::INDIRECT_BASED: {
        const bool computeSupported = !!(caps->supported & BGFX_CAPS_COMPUTE);
        const bool indirectSupported =
            !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
        const bool instancingSupported =
            !!(caps->supported & BGFX_CAPS_INSTANCING);

        assert(
            (void("Instancing or compute are not supported"),
             instancingSupported && computeSupported && indirectSupported));
        return std::make_unique<IndirectBasedLines>(points);
    }

    case LinesTypes::TEXTURE_BASED: {
        const bool computeSupported = !!(caps->supported & BGFX_CAPS_COMPUTE);
        const bool indirectSupported =
            !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
        const bool instancingSupported =
            !!(caps->supported & BGFX_CAPS_INSTANCING);
        const bool textureSupported =
            !!(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

        assert(
            (void("Instancing or compute or indirect or texture are not "
                  "supported"),
             instancingSupported && computeSupported && indirectSupported &&
                 textureSupported));
        return std::make_unique<TextureBasedLines>(
            points, caps->limits.maxTextureSize);
    }
    }

    assert((void("Lines type is incorrect"), true));
    throw std::invalid_argument("Invalid enum case");
}

} // namespace vcl::lines
