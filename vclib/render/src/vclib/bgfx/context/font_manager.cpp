// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/context/font_manager.h>

#include <algorithm>
#include <fstream>
#include <vector>

namespace vcl {

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
    for (auto& [key, entry] : mFontMap) {
        mFontManager.destroyFont(entry.first);
    }
    for (auto& [fontName, handle] : mBaseFontMap) {
        mFontManager.destroyFont(handle);
    }
    for (auto& [fontName, handle] : mTTMap) {
        mFontManager.destroyTtf(handle);
    }
}

void FontManager::loadFont(VclFont::Enum font)
{
    if (mTTMap.find(VclFont::FONT_NAMES[font]) == mTTMap.end()) {
        FontData fd;
        switch (font) {
        case VclFont::DROID_SANS:
            fd = EmbeddedFont<VclFont::DROID_SANS>::embeddedFont();
            break;
        default: return;
        }
        bgfx::TrueTypeHandle handle = loadTtf(mFontManager, fd.data, fd.size);
        mTTMap[VclFont::FONT_NAMES[font]] = handle;
    }
}

void FontManager::loadFont(const FontData& fd, const std::string& fontName)
{
    if (mTTMap.find(fontName) == mTTMap.end()) {
        bgfx::TrueTypeHandle handle = loadTtf(mFontManager, fd.data, fd.size);
        mTTMap[fontName]            = handle;
    }
}

void FontManager::loadFont(
    const std::string& filePath,
    const std::string& fontName)
{
    if (mTTMap.find(fontName) == mTTMap.end()) {
        bgfx::TrueTypeHandle handle = loadTtf(mFontManager, filePath.c_str());
        mTTMap[fontName]            = handle;
    }
}

bgfx::FontHandle FontManager::getFontHandle(
    VclFont::Enum font,
    uint16_t      fontSize)
{
    std::string fontName = VclFont::FONT_NAMES[font];
    if (mTTMap.find(fontName) == mTTMap.end()) {
        loadFont(font);
    }
    return getFontHandle(fontName, fontSize);
}

bgfx::FontHandle FontManager::getFontHandle(
    const std::string& fontName,
    uint16_t           fontSize)
{
    fontSize    = std::max<uint16_t>(fontSize, 1);
    FontKey key = {fontName, fontSize};
    auto    it  = mFontMap.find(key);
    if (it != mFontMap.end()) {
        mFontLRU.splice(mFontLRU.begin(), mFontLRU, it->second.second);
        return it->second.first;
    }

    auto baseIt = mBaseFontMap.find(fontName);
    if (baseIt == mBaseFontMap.end()) {
        bgfx::TrueTypeHandle ttHandle = mTTMap.at(fontName);
        bgfx::FontHandle     baseFont = mFontManager.createFontByPixelSize(
            ttHandle,
            0,
            BASE_SDF_FONT_SIZE,
            FONT_TYPE_DISTANCE,
            SDF_GLYPH_PADDING,
            SDF_GLYPH_PADDING);
        baseIt = mBaseFontMap.emplace(fontName, baseFont).first;
    }

    while (!mFontLRU.empty() &&
           (mFontMap.size() >= MAX_CACHED_SCALED_FONTS ||
            mBaseFontMap.size() + mFontMap.size() >= MAX_OPENED_FONT)) {
        const FontKey& oldestKey = mFontLRU.back();
        auto           oldestIt  = mFontMap.find(oldestKey);
        mFontManager.destroyFont(oldestIt->second.first);
        mFontMap.erase(oldestIt);
        mFontLRU.pop_back();
    }

    bgfx::FontHandle scaledFont =
        mFontManager.createScaledFontToPixelSize(baseIt->second, fontSize);
    mFontLRU.push_front(key);
    mFontMap.emplace(key, std::make_pair(scaledFont, mFontLRU.begin()));
    return scaledFont;
}

bgfx::FontManager& FontManager::getBGFXFontManager()
{
    return mFontManager;
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

    return loadTtf(fontManager, data.data(), size);
}

bgfx::TrueTypeHandle FontManager::loadTtf(
    bgfx::FontManager& fontManager,
    const uint8_t*     data,
    std::size_t        size)
{
    bgfx::TrueTypeHandle handle = fontManager.createTtf(data, size);
    return handle;
}

} // namespace vcl
