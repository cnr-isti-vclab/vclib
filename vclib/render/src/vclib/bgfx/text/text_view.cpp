// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/text/text_view.h>

#include <vclib/bgfx/context.h>

#include <bx/math.h>

namespace vcl {

TextView::TextView()
{
}

TextView::~TextView()
{
    if (isTextEnabled())
        Context::instance().releaseViewId(mView);
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
    auto& ctx = Context::instance();
    if (ctx.isValidViewId(mView)) {
        if (!b) {
            ctx.releaseViewId(mView);
            mView = BGFX_INVALID_VIEW;
        }
    }
    else {
        if (b) {
            mView = ctx.requestViewId();
            updateProjMatrix();
            bgfx::setViewRect(mView, 0, 0, mWidth, mHeight);
            bgfx::touch(mView);
        }
    }
}

bool TextView::isTextEnabled() const
{
    return Context::instance().isValidViewId(mView);
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
