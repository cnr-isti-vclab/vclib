/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_BGFX_CONTEXT_FONT_MANAGER_H
#define VCL_BGFX_CONTEXT_FONT_MANAGER_H

#include <vclib/bgfx/text/embedded_fonts.h>
#include <vclib/bgfx/text/font/font_manager.h>

#include <map>
#include <string>

namespace vcl {

class FontManager
{
    friend class TextManager;

    bgfx::FontManager                                            mFontManager;
    std::map<std::string, bgfx::TrueTypeHandle>                  mTTMap;
    std::map<std::pair<std::string, uint16_t>, bgfx::FontHandle> mFontMap;

public:
    FontManager();
    ~FontManager();

    FontManager(const FontManager&)            = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&)                 = delete;
    FontManager& operator=(FontManager&&)      = delete;

    void loadFont(VclFont::Enum font);
    void loadFont(const FontData& fd, const std::string& fontName);

    void loadFont(const std::string& filePath, const std::string& fontName);

    bgfx::FontHandle getFontHandle(VclFont::Enum font, uint16_t fontSize);
    bgfx::FontHandle getFontHandle(
        const std::string& fontName,
        uint16_t           fontSize);

private:
    bgfx::FontManager& getBGFXFontManager();

    static bgfx::TrueTypeHandle loadTtf(
        bgfx::FontManager& fontManager,
        const char*        filePath);

    static bgfx::TrueTypeHandle loadTtf(
        bgfx::FontManager& fontManager,
        const uint8_t*     data,
        std::size_t        size);
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_FONT_MANAGER_H
