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

class TextManager
{
    bgfx::TextBufferManager* mTextBufferManager = nullptr;
    bgfx::TextBufferHandle   mStaticText;
    bgfx::TextBufferHandle   mTransientText;

    bgfx::FontHandle mCurrentFont = BGFX_INVALID_HANDLE;

public:
    TextManager();
    ~TextManager();

    void init();

    void loadFont(const std::string& filePath, const std::string& fontName);

    void clear();

    void setCurrentFont(VclFont::Enum font, uint16_t fontSize);
    void setCurrentFont(const std::string& fontName, uint16_t fontSize);

    void appendTransientText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    void appendStaticText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    void submit(uint viewId);
};

} // namespace vcl

#endif // VCL_BGFX_TEXT_TEXT_MANAGER_H
