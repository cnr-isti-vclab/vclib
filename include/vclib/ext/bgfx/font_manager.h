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

#ifndef VCL_EXT_BGFX_FONT_MANAGER_H
#define VCL_EXT_BGFX_FONT_MANAGER_H

#include "font/font_manager.h"

#include <map>
#include <string>

namespace vcl::bgf {

class FontManager
{
    bgfx::FontManager fontManager;
    std::map<std::string, bgfx::TrueTypeHandle> ttMap;
    std::map<std::pair<std::string, uint16_t>, bgfx::FontHandle> fontMap;

public:
    FontManager();
    ~FontManager();

    bgfx::FontManager& getFontManager();

    void loadFont(const std::string& filePath, const std::string& fontName);

    bgfx::FontHandle getFontHandle(
        const std::string& fontName,
        uint16_t           fontSize);

private:
    static bgfx::TrueTypeHandle loadTtf(
        bgfx::FontManager& fontManager,
        const char*        filePath);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_FONT_MANAGER_H
