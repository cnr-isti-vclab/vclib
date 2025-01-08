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

#ifndef VCL_BGFX_TEXT_EMBEDDED_FONTS_EMBEDDED_FONT_H
#define VCL_BGFX_TEXT_EMBEDDED_FONTS_EMBEDDED_FONT_H

#include <array>
#include <cstdint>
#include <string>

namespace vcl {

struct FontData
{
    const uint8_t* data = nullptr;
    uint32_t       size = 0;
};

struct VclFont
{
    enum Enum {
        DROID_SANS,

        COUNT
    };

    static inline const std::array<std::string, VclFont::COUNT> fontNames = {
        "DroidSans",
    };
};

template<VclFont::Enum FONT = VclFont::COUNT>
struct EmbeddedFont
{
    static FontData embeddedFont()
    {
        static_assert(true, "Invalid Font");
        return {nullptr, 0};
    }
};

} // namespace vcl

#endif // VCL_BGFX_TEXT_EMBEDDED_FONTS_EMBEDDED_FONT_H
