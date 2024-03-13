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

#include <vclib/ext/bgfx/context/font_manager.h>

#include <fstream>
#include <vector>

namespace vcl::bgf {

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
    for (auto& [fontName, handle] : fontMap) {
        fontManager.destroyFont(handle);
    }
    for (auto& [pair, handle] : ttMap) {
        fontManager.destroyTtf(handle);
    }
}

void FontManager::loadFont(
    const std::string& filePath,
    const std::string& fontName)
{
    if (ttMap.find(fontName) == ttMap.end()) {
        bgfx::TrueTypeHandle handle = loadTtf(fontManager, filePath.c_str());
        ttMap[fontName]             = handle;
    }
}

bgfx::FontHandle FontManager::getFontHandle(
    const std::string& fontName,
    uint16_t           fontSize)
{
    auto it = fontMap.find({fontName, fontSize});
    if (it != fontMap.end()) {
        return it->second;
    }
    else {
        bgfx::TrueTypeHandle ttHandle = ttMap.at(fontName);
        bgfx::FontHandle     font =
            fontManager.createFontByPixelSize(ttHandle, 0, fontSize);
        fontMap[{fontName, fontSize}] = font;
        return font;
    }
}

bgfx::FontManager& FontManager::getBGFXFontManager()
{
    return fontManager;
}

bgfx::TrueTypeHandle FontManager::loadTtf(
    bgfx::FontManager& fontManager,
    const char*        filePath)
{
    std::vector<uint8_t> data;

    // create ifstream from filePath
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    // get the size of the file
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the file and place it into the vector data
    data.resize(size);
    file.read((char*) data.data(), size);

    bgfx::TrueTypeHandle handle = fontManager.createTtf(data.data(), size);
    return handle;
}

} // namespace vcl::bgf
