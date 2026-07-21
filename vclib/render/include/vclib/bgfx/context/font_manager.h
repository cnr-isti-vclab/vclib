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

/**
 * @class FontManager
 * @brief Manages loading, caching, and retrieval of fonts using bgfx.
 *
 * This class handles both predefined/embedded fonts and custom fonts loaded
 * from files or memory. It caches the loaded TrueType handles and generated
 * FontHandle objects for specific pixel sizes.
 */
class FontManager
{
    friend TextManager;

    /// The underlying bgfx font manager.
    bgfx::FontManager mFontManager;

    /// Map caching loaded TrueType font handles by font name.
    std::map<std::string, bgfx::TrueTypeHandle> mTTMap;

    /// Map caching generated bgfx font handles by a pair of font name and font
    /// size (in pixels).
    std::map<std::pair<std::string, uint16_t>, bgfx::FontHandle> mFontMap;

public:
    /**
     * @brief Constructs a FontManager.
     */
    FontManager();

    /**
     * @brief Destructor. Destroys all cached font handles and TrueType font
     * handles.
     */
    ~FontManager();

    FontManager(const FontManager&)            = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&)                 = delete;
    FontManager& operator=(FontManager&&)      = delete;

    /**
     * @brief Loads an embedded font.
     * @param[in] font: The predefined embedded font identifier.
     */
    void loadFont(VclFont::Enum font);

    /**
     * @brief Loads a font from memory data.
     * @param[in] fd: The font data structure containing a pointer to the font
     * buffer and its size.
     * @param[in] fontName: The name to register the font with.
     */
    void loadFont(const FontData& fd, const std::string& fontName);

    /**
     * @brief Loads a font from a TrueType file path.
     * @param[in] filePath: The system file path to the TTF file.
     * @param[in] fontName: The name to register the font with.
     */
    void loadFont(const std::string& filePath, const std::string& fontName);

    /**
     * @brief Retrieves the bgfx font handle for a predefined embedded font at a
     * specific pixel size.
     *
     * If the font or the specific size has not been loaded/created yet, it
     * loads and caches them automatically.
     *
     * @param[in] font: The predefined embedded font identifier.
     * @param[in] fontSize: The desired font size in pixels.
     * @return The bgfx font handle.
     */
    bgfx::FontHandle getFontHandle(VclFont::Enum font, uint16_t fontSize);

    /**
     * @brief Retrieves the bgfx font handle for a registered custom font at a
     * specific pixel size.
     *
     * If the font size has not been created yet, it creates and caches it.
     *
     * @param[in] fontName: The name of the registered font.
     * @param[in] fontSize: The desired font size in pixels.
     * @return The bgfx font handle.
     */
    bgfx::FontHandle getFontHandle(
        const std::string& fontName,
        uint16_t           fontSize);

private:
    /**
     * @brief Retrieves the reference to the underlying bgfx font manager.
     * @return Reference to the bgfx::FontManager.
     */
    bgfx::FontManager& getBGFXFontManager();

    /**
     * @brief Helper to load a TrueType font from a file path.
     * @param[in] fontManager: The bgfx font manager.
     * @param[in] filePath: The system file path to the TTF file.
     * @return The bgfx TrueType handle.
     */
    static bgfx::TrueTypeHandle loadTtf(
        bgfx::FontManager& fontManager,
        const char*        filePath);

    /**
     * @brief Helper to load a TrueType font from memory data.
     * @param[in] fontManager: The bgfx font manager.
     * @param[in] data: Pointer to the raw font data.
     * @param[in] size: The size of the raw font data in bytes.
     * @return The bgfx TrueType handle.
     */
    static bgfx::TrueTypeHandle loadTtf(
        bgfx::FontManager& fontManager,
        const uint8_t*     data,
        std::size_t        size);
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_FONT_MANAGER_H
