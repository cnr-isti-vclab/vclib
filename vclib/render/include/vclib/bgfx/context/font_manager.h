// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_CONTEXT_FONT_MANAGER_H
#define VCL_BGFX_CONTEXT_FONT_MANAGER_H

#include <vclib/bgfx/text/embedded_fonts.h>
#include <vclib/bgfx/text/font/font_manager.h>
#include <vclib/bgfx/text/text_manager.h>

#include <map>
#include <string>

namespace vcl {

class FontManager
{
    friend TextManager;

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
