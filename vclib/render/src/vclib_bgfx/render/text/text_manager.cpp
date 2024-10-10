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

#include <vclib_bgfx/render/text/text_manager.h>

#include <vclib_bgfx/render/context.h>

namespace vcl {

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
    mTextBufferManager->destroyTextBuffer(mTransientText);
    mTextBufferManager->destroyTextBuffer(mStaticText);
    delete mTextBufferManager;
}

void TextManager::init()
{
    mTextBufferManager = new bgfx::TextBufferManager(
        &Context::fontManager().getBGFXFontManager());

    mTransientText = mTextBufferManager->createTextBuffer(
        FONT_TYPE_ALPHA, bgfx::BufferType::Transient);

    mStaticText = mTextBufferManager->createTextBuffer(
        FONT_TYPE_ALPHA, bgfx::BufferType::Static);
}

void TextManager::loadFont(
    const std::string& filePath,
    const std::string& fontName)
{
    Context::fontManager().loadFont(filePath, fontName);
}

void TextManager::clear()
{
    mTextBufferManager->clearTextBuffer(mTransientText);
    mTextBufferManager->clearTextBuffer(mStaticText);
}

void TextManager::setCurrentFont(VclFont::Enum font, uint16_t fontSize)
{
    mCurrentFont = Context::fontManager().getFontHandle(font, fontSize);
}

void TextManager::setCurrentFont(const std::string& fontName, uint16_t fontSize)
{
    mCurrentFont = Context::fontManager().getFontHandle(fontName, fontSize);
}

void TextManager::appendTransientText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    mTextBufferManager->setTextColor(mTransientText, color.rgba());

    mTextBufferManager->setPenPosition(mTransientText, pos.x(), pos.y());
    mTextBufferManager->appendText(mTransientText, mCurrentFont, text.c_str());
}

void TextManager::appendStaticText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    mTextBufferManager->setTextColor(mStaticText, color.rgba());

    mTextBufferManager->setPenPosition(mStaticText, pos.x(), pos.y());
    mTextBufferManager->appendText(mStaticText, mCurrentFont, text.c_str());
}

void TextManager::submit(uint viewId)
{
    mTextBufferManager->submitTextBuffer(mTransientText, viewId);
    mTextBufferManager->submitTextBuffer(mStaticText, viewId);
    mTextBufferManager->clearTextBuffer(mTransientText);
}

} // namespace vcl
