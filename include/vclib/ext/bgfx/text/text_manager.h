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

#ifndef VCL_EXT_BGFX_TEXT_TEXT_MANAGER_H
#define VCL_EXT_BGFX_TEXT_TEXT_MANAGER_H

#include <string>

#include <vclib/space/color.h>

#include "font/text_buffer_manager.h"

namespace vcl::bgf {

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

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_TEXT_TEXT_MANAGER_H
