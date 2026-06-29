// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/text/text_manager.h>

#include <vclib/bgfx/context.h>

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
        &Context::instance().fontManager().getBGFXFontManager());

    mTransientText = mTextBufferManager->createTextBuffer(
        FONT_TYPE_ALPHA, bgfx::BufferType::Transient);

    mStaticText = mTextBufferManager->createTextBuffer(
        FONT_TYPE_ALPHA, bgfx::BufferType::Static);
}

void TextManager::loadFont(
    const std::string& filePath,
    const std::string& fontName)
{
    Context::instance().fontManager().loadFont(filePath, fontName);
}

void TextManager::clear()
{
    mTextBufferManager->clearTextBuffer(mTransientText);
    mTextBufferManager->clearTextBuffer(mStaticText);
}

void TextManager::setCurrentFont(VclFont::Enum font, uint16_t fontSize)
{
    mCurrentFont =
        Context::instance().fontManager().getFontHandle(font, fontSize);
}

void TextManager::setCurrentFont(const std::string& fontName, uint16_t fontSize)
{
    mCurrentFont =
        Context::instance().fontManager().getFontHandle(fontName, fontSize);
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
