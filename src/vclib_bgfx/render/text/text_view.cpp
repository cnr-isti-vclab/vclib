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

#include <vclib_bgfx/render/text/text_view.h>

#include <vclib_bgfx/render/context.h>

#include <bx/math.h>

namespace vcl {

TextView::TextView()
{
}

TextView::~TextView()
{
    if (isTextEnabled())
        Context::releaseViewId(mView);
}

void TextView::init(uint width, uint height)
{
    mWidth  = width;
    mHeight = height;

    const bx::Vec3 at  = {0.0f, 0.0f, 0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, -1.0f};

    bx::mtxLookAt(mTextViewMatrix, eye, at);

    mTextManager.init();

    mTextManager.setCurrentFont(VclFont::DROID_SANS, 20);
}

void TextView::enableText(bool b)
{
    if (isViewValid(mView)) {
        if (!b) {
            Context::releaseViewId(mView);
            mView = BGFX_INVALID_HANDLE;
        }
    }
    else {
        if (b) {
            mView = Context::requestViewId();
            updateProjMatrix();
            bgfx::setViewRect(mView, 0, 0, mWidth, mHeight);
            bgfx::touch(mView);
        }
    }
}

bool TextView::isTextEnabled() const
{
    return isViewValid(mView);
}

void TextView::setTextFont(VclFont::Enum font, uint fontSize)
{
    mTextManager.setCurrentFont(font, fontSize);
}

void TextView::setTextFont(const std::string& fontName, uint fontSize)
{
    mTextManager.setCurrentFont(fontName, fontSize);
}

void TextView::clearText()
{
    mTextManager.clear();
}

void TextView::appendStaticText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    mTextManager.appendStaticText(pos, text, color);
}

void TextView::appendTransientText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    mTextManager.appendTransientText(pos, text, color);
}

void TextView::frame(bgfx::FrameBufferHandle fbh)
{
    static uint cnt = 0;
    if (isTextEnabled()) {
        bgfx::setViewFrameBuffer(mView, fbh);
        bgfx::touch(mView);

        bgfx::setViewTransform(mView, mTextViewMatrix, mTextProjMatrix);

        mTextManager.submit(mView);
    }
}

void TextView::resize(uint width, uint height)
{
    mWidth  = width;
    mHeight = height;

    if (isTextEnabled()) {
        updateProjMatrix();
        bgfx::setViewRect(mView, 0, 0, mWidth, mHeight);
        bgfx::touch(mView);
    }
}

void TextView::updateProjMatrix()
{
    const bgfx::Caps* caps = bgfx::getCaps();
    bx::mtxOrtho(
        mTextProjMatrix,
        0.0f,
        float(mWidth),
        float(mHeight),
        0.0f,
        0.0f,
        100.0f,
        0.0f,
        caps->homogeneousDepth);
}

} // namespace vcl
