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

#include <vclib/ext/bgfx/text/text_manager.h>

#include <vclib/ext/bgfx/context.h>

namespace vcl::bgf {

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
    textBufferManager->destroyTextBuffer(transientText);
    textBufferManager->destroyTextBuffer(staticText);
    delete textBufferManager;
}

void TextManager::init()
{
    textBufferManager =
        new bgfx::TextBufferManager(&Context::fontMap().getFontManager());

    transientText = textBufferManager->createTextBuffer(
        FONT_TYPE_ALPHA, bgfx::BufferType::Transient);

    staticText = textBufferManager->createTextBuffer(
        FONT_TYPE_ALPHA, bgfx::BufferType::Static);
}

void TextManager::loadFont(
    const std::string& filePath,
    const std::string& fontName)
{
    Context::fontMap().loadFont(filePath, fontName);
}

void TextManager::clear()
{
    textBufferManager->clearTextBuffer(transientText);
    textBufferManager->clearTextBuffer(staticText);
}

void TextManager::setCurrentFont(const std::string& fontName, uint16_t fontSize)
{
    currentFont = Context::fontMap().getFontHandle(fontName, fontSize);
}

void TextManager::appendTransientText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    textBufferManager->setTextColor(transientText, color.rgba());

    textBufferManager->setPenPosition(transientText, pos.x(), pos.y());
    textBufferManager->appendText(transientText, currentFont, text.c_str());
}

void TextManager::appendStaticText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    textBufferManager->setTextColor(staticText, color.rgba());

    textBufferManager->setPenPosition(staticText, pos.x(), pos.y());
    textBufferManager->appendText(staticText, currentFont, text.c_str());
}

void TextManager::submit(uint viewId)
{
    textBufferManager->submitTextBuffer(transientText, viewId);
    textBufferManager->submitTextBuffer(staticText, viewId);
    textBufferManager->clearTextBuffer(transientText);
}

} // namespace vcl::bgf
