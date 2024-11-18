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

#include <vclib/render_bgfx/canvas.h>

#include <vclib/render_bgfx/system/native_window_handle.h>

namespace vcl {

Canvas::Canvas(void* winId, uint width, uint height, void* displayId)
{
    // save window id
    mWinId = winId;

    // get the passes:
    // - drawing on screen
    mViewId          = Context::requestViewId(mWinId, displayId);
    // - drawing offscreen
    mViewOffscreenId = Context::requestViewId(mWinId, displayId);

    mTextView.init(width, height);

    // (re)create the framebuffers
    this->onResize(width, height);

    // create the blith depth texture
    mBlitDepth =
        bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::D32, 0
            | BGFX_TEXTURE_BLIT_DST
            | BGFX_TEXTURE_READ_BACK
            | BGFX_SAMPLER_MIN_POINT
            | BGFX_SAMPLER_MAG_POINT
            | BGFX_SAMPLER_MIP_POINT
            | BGFX_SAMPLER_U_CLAMP
            | BGFX_SAMPLER_V_CLAMP
            );
    assert(bgfx::isValid(mBlitDepth));
    assert(bgfx::isValid(mOffscreenFbh));
}

Canvas::~Canvas()
{
    // deallocate the framebuffers
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    if (bgfx::isValid(mOffscreenFbh))
        bgfx::destroy(mOffscreenFbh);

    // deallocate the blit depth texture
    if (bgfx::isValid(mBlitDepth))
        bgfx::destroy(mBlitDepth);

    // release the passes
    Context::releaseViewId(mViewId);
    Context::releaseViewId(mViewOffscreenId);
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

void Canvas::onKeyPress(Key::Enum key)
{
    if (key == Key::F1) {
        if (mStatsEnabled) {
            mStatsEnabled = false;
            bgfx::setDebug(BGFX_DEBUG_NONE);
        }
        else {
            mStatsEnabled = true;
            bgfx::setDebug(BGFX_DEBUG_STATS);
        }
    }
}

void Canvas::onResize(uint width, uint height)
{
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    mFbh = createFrameBufferAndInitView(
        mWinId, mViewId, width, height, true, false);

    if (bgfx::isValid(mOffscreenFbh))
        bgfx::destroy(mOffscreenFbh);

    mOffscreenFbh = createFrameBufferAndInitView(
        nullptr, mViewOffscreenId, width, height, true, true);

    mTextView.resize(width, height);
}

void Canvas::frame()
{
    bgfx::setViewFrameBuffer(mViewId, mFbh);
    bgfx::touch(mViewId);
    draw();
    mTextView.frame(mFbh);

    if (mCurrFrame > 0 && mCurrFrame % 100 == 0) {
        offscreenFrame();
    }

    mCurrFrame = bgfx::frame();

    std::cerr << "frame: " << mCurrFrame << std::endl;
    if (mReadFrame != 0 && mReadFrame >= mCurrFrame) {
        mReadFrame = 0;
        std::cerr << "DEPTH: " << mDepthData << std::endl;
    }
}

void Canvas::offscreenFrame()
{
    // if already reading depth, do nothing
    if (mReadFrame != 0) {
        return;
    }

    // render offscren
    bgfx::setViewFrameBuffer(mViewOffscreenId, mOffscreenFbh);
    bgfx::touch(mViewOffscreenId);

    // render changing the view
    auto tmpId = mViewId;
    mViewId    = mViewOffscreenId;
    draw();
    mViewId = tmpId;

    // blit the depth buffer
    auto depthTexture = bgfx::getTexture(mOffscreenFbh, 1);
    bgfx::blit(mViewOffscreenId, mBlitDepth,
        0, 0, depthTexture,
        1022, 760, 1, 1);
    // read the depth
    mReadFrame = bgfx::readTexture(mBlitDepth, &mDepthData);
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
    bgfx::TextureFormat::Enum depthFormat = bgfx::TextureFormat::D24;

    if (depth32bit) {
        depthFormat = bgfx::TextureFormat::D32;
    }

    bgfx::FrameBufferHandle fbh = BGFX_INVALID_HANDLE;

    if (winId == nullptr) {
        // create offscreen framebuffer with explicit textures
        bgfx::TextureHandle fbtextures[2];
        fbtextures[0] = bgfx::createTexture2D(
                uint16_t(width)
            , uint16_t(height)
            , false
            , 1
            , colorFormat
            , BGFX_TEXTURE_RT
            | BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
            );

        fbtextures[1] = bgfx::createTexture2D(
                uint16_t(width)
            , uint16_t(height)
            , false
            , 1
            , depthFormat
            , BGFX_TEXTURE_RT
            | BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
            );
        fbh = bgfx::createFrameBuffer(2, fbtextures, true);

        bgfx::setViewFrameBuffer(view, fbh);
    }

    if (clear) {
        bgfx::setViewClear(
            view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
    }
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::reset(width, height,
        (winId == nullptr) ? BGFX_RESET_NONE : BGFX_RESET_VSYNC);
    bgfx::touch(view);

    return fbh;
}

} // namespace vcl
