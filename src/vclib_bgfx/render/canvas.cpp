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

#include <vclib_bgfx/render/canvas.h>

#include <vclib/render/system/native_window_handle.h>

namespace vcl {

Canvas::Canvas(void* winId, uint width, uint height)
{
    mWinId = winId;

    mViewId = Context::requestViewId();

    mFbh = createFrameBufferAndInitView(winId, mViewId, width, height, true);

    mTextView.init(width, height);
}

Canvas::~Canvas()
{
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    Context::releaseViewId(mViewId);
}

void Canvas::screenShot(const std::string& filename, uint width, uint height)
{
    if (width == 0 || height == 0) {
        draw();
        bgfx::requestScreenShot(mFbh, filename.c_str());
        bgfx::frame();
    }
    else {
        void* d;
        void* w = vcl::createWindow("", width, height, d, true);

        // setup view and frame buffer
        bgfx::ViewId            v = Context::requestViewId();
        bgfx::FrameBufferHandle fbh =
            createFrameBufferAndInitView(w, v, width, height, true);

        // replace the current view with the new one
        bgfx::ViewId tmpView = mViewId;
        mViewId              = v;
        draw();
        mTextView.frame(fbh);
        bgfx::requestScreenShot(fbh, filename.c_str());
        bgfx::frame();

        // restore the previous view and release the resources
        mViewId = tmpView;
        bgfx::destroy(fbh);
        Context::releaseViewId(v);
        vcl::closeWindow(w, d);
    }
}

void Canvas::enableText(bool b)
{
    mTextView.enableText(b);
}

bool Canvas::isTextEnabled() const
{
    return mTextView.isTextEnabled();
}

void Canvas::setTextFont(VclFont::Enum font, uint fontSize)
{
    mTextView.setTextFont(font, fontSize);
}

void Canvas::setTextFont(const std::string& fontName, uint fontSize)
{
    mTextView.setTextFont(fontName, fontSize);
}

void Canvas::clearText()
{
    mTextView.clearText();
}

void Canvas::appendStaticText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    mTextView.appendStaticText(pos, text, color);
}

void Canvas::appendTransientText(
    const Point2f&     pos,
    const std::string& text,
    const Color&       color)
{
    mTextView.appendTransientText(pos, text, color);
}

void Canvas::onResize(uint width, uint height)
{
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    mFbh = createFrameBufferAndInitView(mWinId, mViewId, width, height);

    mTextView.resize(width, height);
}

void Canvas::frame()
{
    bgfx::setViewFrameBuffer(mViewId, mFbh);
    bgfx::touch(mViewId);
    draw();
    mTextView.frame(mFbh);

    bgfx::frame();
#ifdef __APPLE__ // workaround for forcing bgfx refresh buffer on MacOS
    bgfx::frame();
#endif // __APPLE__
}

bgfx::FrameBufferHandle Canvas::createFrameBufferAndInitView(
    void*        winId,
    bgfx::ViewId view,
    uint         width,
    uint         height,
    bool         clear,
    bool         depth32bit)
{
    bgfx::TextureFormat::Enum colorFormat = bgfx::TextureFormat::RGBA8;
    bgfx::TextureFormat::Enum depthFormat = bgfx::TextureFormat::Count;

    if (depth32bit) {
        depthFormat = bgfx::TextureFormat::D32;
    }

    bgfx::FrameBufferHandle fbh =
        bgfx::createFrameBuffer(winId, width, height, colorFormat, depthFormat);
    bgfx::setViewFrameBuffer(view, fbh);
    if (clear) {
        bgfx::setViewClear(
            view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
    }
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);
    return fbh;
}

} // namespace vcl
