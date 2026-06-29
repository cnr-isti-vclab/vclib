// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

    inline static const std::array<std::string, VclFont::COUNT> FONT_NAMES = {
        "DroidSans",
    };
};

template<VclFont::Enum FONT = VclFont::COUNT>
struct EmbeddedFont
{
    static FontData embeddedFont()
    {
        static_assert(false, "Invalid Font");
        return {nullptr, 0};
    }
};

} // namespace vcl

#endif // VCL_BGFX_TEXT_EMBEDDED_FONTS_EMBEDDED_FONT_H
