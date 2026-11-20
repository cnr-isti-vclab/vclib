// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_TEXT_TEXT_MANAGER_H
#define VCL_BGFX_TEXT_TEXT_MANAGER_H

#include "embedded_fonts/embedded_font.h"
#include "font/text_buffer_manager.h"

#include <vclib/space/core/color.h>

#include <string>

namespace vcl {

/**
 * @brief Manages font handles, text buffers, and text draw call submission
 * using bgfx.
 *
 * TextManager provides a mid-level interface for rendering 2D text using
 * distance field fonts (@c FONT_TYPE_DISTANCE) backed by the bgfx text
 * extension library.
 *
 * It manages two separate text buffers:
 * - **Static Text Buffer**: Stores persistent text that remains visible across
 * multiple frames until explicitly cleared using @ref clear. Useful for static
 * labels, UI headings, or persistent indicators.
 * - **Transient Text Buffer**: Stores dynamic text intended for the current
 * frame only. When @ref submit is called, transient text is drawn to the
 * specified view and automatically cleared immediately after submission.
 *
 * TextManager coordinates with the global @ref FontManager (accessible via
 * `Context::instance().fontManager()`) to resolve font handles, load custom
 * font files, or use embedded fonts (such as Droid Sans).
 */
class TextManager
{
    bgfx::TextBufferManager* mTextBufferManager = nullptr;
    bgfx::TextBufferHandle   mStaticText;
    bgfx::TextBufferHandle   mTransientText;

    std::string mCurrentFontName;
    uint16_t    mCurrentFontSize = 20;

public:
    /**
     * @brief Constructs an uninitialized TextManager.
     */
    TextManager();

    /**
     * @brief Destructor. Destroys the managed static and transient text buffers
     * and frees the underlying bgfx::TextBufferManager.
     */
    ~TextManager();

    /**
     * @brief Initializes the internal bgfx::TextBufferManager and allocates
     * static and transient text buffers using distance field fonts.
     *
     * Sets the initial default font to Droid Sans at size 20.
     */
    void init();

    /**
     * @brief Loads a TrueType font file from disk and registers it under the
     * given font name.
     *
     * @param[in] filePath: Path to the font file (.ttf).
     * @param[in] fontName: Identifier to associate with the loaded font.
     */
    void loadFont(const std::string& filePath, const std::string& fontName);

    /**
     * @brief Clears both the static and transient text buffers.
     */
    void clear();

    /**
     * @brief Sets the active font using a predefined embedded font enum and
     * sets its size.
     *
     * @param[in] font: The embedded font identifier.
     * @param[in] fontSize: The font size in pixels/points.
     */
    void setCurrentFont(VclFont::Enum font, uint16_t fontSize);

    /**
     * @brief Sets the active font by its registered name and sets its size.
     *
     * @param[in] fontName: The name of the previously loaded or registered
     * font.
     * @param[in] fontSize: The font size in pixels/points.
     */
    void setCurrentFont(const std::string& fontName, uint16_t fontSize);

    /**
     * @brief Appends single-frame (transient) text to be drawn at the given 2D
     * position.
     *
     * Transient text is drawn on the next call to @ref submit and is
     * automatically cleared afterwards.
     *
     * @param[in] pos: 2D position (pen origin) in screen coordinates.
     * @param[in] text: The text string to render.
     * @param[in] color: Text color (defaults to black).
     */
    void appendTransientText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    /**
     * @brief Appends persistent (static) text to be drawn at the given 2D
     * position.
     *
     * Static text remains in the buffer across frames until @ref clear is
     * called.
     *
     * @param[in] pos: 2D position (pen origin) in screen coordinates.
     * @param[in] text: The text string to render.
     * @param[in] color: Text color (defaults to black).
     */
    void appendStaticText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    /**
     * @brief Submits both transient and static text buffers to the specified
     * bgfx view for rendering, and clears the transient text buffer.
     *
     * @param[in] viewId: The bgfx view ID to which the text draw calls are
     * submitted.
     */
    void submit(uint viewId);
};

} // namespace vcl

#endif // VCL_BGFX_TEXT_TEXT_MANAGER_H
