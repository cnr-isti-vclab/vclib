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

static const bgfx::TextureFormat::Enum kDefaultColorFormat =
    bgfx::TextureFormat::RGBA8;

static const bgfx::TextureFormat::Enum kDefaultDepthFormat =
    bgfx::TextureFormat::D24S8;

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
    Canvas::onResize(width, height);

    assert(bgfx::isValid(mOffscreenFbh));
}

Canvas::~Canvas()
{
    // deallocate the framebuffers
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    if (bgfx::isValid(mOffscreenFbh))
        bgfx::destroy(mOffscreenFbh);

    // deallocate the blit textures
    if (bgfx::isValid(mBlitDepth))
        bgfx::destroy(mBlitDepth);

    if (bgfx::isValid(mBlitColor))
        bgfx::destroy(mBlitColor);

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
    // create window backbuffer
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    mFbh = createFrameBufferAndInitView(
        mWinId, mViewId, width, height, true);
    assert(!bgfx::isValid(mFbh)); // the backbuffer is always non valid

    // TODO this is inefficient for two reasons:
    // 1. we keep the offscreen framebuffer allocated even if we don't need it
    // 2. we keep the blit textures and data allocated if we don't need them

    // create offscreen framebuffer
    if (bgfx::isValid(mOffscreenFbh))
        bgfx::destroy(mOffscreenFbh);

    mOffscreenFbh = createFrameBufferAndInitView(
        nullptr, mViewOffscreenId, width, height, true);
    assert(bgfx::isValid(mOffscreenFbh));

    // create the blit textures
    static const uint64_t kBlitFormat = 0
        | BGFX_TEXTURE_BLIT_DST
        | BGFX_TEXTURE_READ_BACK
        | BGFX_SAMPLER_MIN_POINT
        | BGFX_SAMPLER_MAG_POINT
        | BGFX_SAMPLER_MIP_POINT
        | BGFX_SAMPLER_U_CLAMP
        | BGFX_SAMPLER_V_CLAMP;

    // get read depth size (depends on the render type)
    auto blitDepthSize = getBlitDepthSize(width, height);

    // create the blit depth texture
    if (bgfx::isValid(mBlitDepth))
        bgfx::destroy(mBlitDepth);

    mBlitDepth = bgfx::createTexture2D( 1, 1, false, 1,
        getOffscreenDepthFormat(), kBlitFormat);
    assert(bgfx::isValid(mBlitDepth));
    mDepthData.resize(blitDepthSize.x() * blitDepthSize.y());

    // create the blit color texture
    if (bgfx::isValid(mBlitColor))
        bgfx::destroy(mBlitColor);

    mBlitColor = bgfx::createTexture2D(width, height, false, 1,
        kDefaultColorFormat, kBlitFormat);
    assert(bgfx::isValid(mBlitColor));

    // allocate the color data
    mColorData.resize(width*height);

    mTextView.resize(width, height);
}

void Canvas::frame()
{
    bgfx::setViewFrameBuffer(mViewId, mFbh);
    bgfx::touch(mViewId);
    draw();
    // mTextView.frame(mFbh);

    bool read = false;
    if (mCurrFrame > 0 && mCurrFrame % 60 == 0) {
        offscreenFrame();
        read = true;
    }

    mCurrFrame = bgfx::frame();

    if (read)
        requestReadDepth();

    // read depth data
    if (mReadDepthFrame != 0 && mReadDepthFrame <= mCurrFrame) {
        mReadDepthFrame = 0;
        std::cerr << "frame: " << mCurrFrame << std::endl;
        std::cerr << "Depth: " << mDepthData[0] << std::endl;
    }
}

void Canvas::requestReadDepth()
{
    // blit the depth buffer
    assert(!mDepthData.empty());
    const auto depthTexture = bgfx::getTexture(mOffscreenFbh, 1);
    if (mDepthData.size() == 1) {
        // read a single fragment
        bgfx::blit(mViewOffscreenId, mBlitDepth,
        0, 0, depthTexture, 0, 0, 1, 1);
    } else {
        // read the entire depth buffer
        bgfx::blit(mViewOffscreenId, mBlitDepth,
            0, 0, depthTexture);
    }
    mReadDepthFrame = bgfx::readTexture(mBlitDepth, mDepthData.data());

    // blit the color buffer
    // auto colorTexture = bgfx::getTexture(mOffscreenFbh, 0);
    // bgfx::blit(mViewOffscreenId, mBlitColor,
    //     0, 0, colorTexture);
    // mReadFrame = bgfx::readTexture(mBlitColor, mColorData.data());
}

void Canvas::offscreenFrame()
{
    // if already reading depth, do nothing
    if (mReadDepthFrame != 0) {
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
}

bgfx::FrameBufferHandle Canvas::createFrameBufferAndInitView(
    void*        winId,
    bgfx::ViewId view,
    uint         width,
    uint         height,
    bool         clear)
{
    const bool offscreen = (winId == nullptr);

    bgfx::FrameBufferHandle fbh = BGFX_INVALID_HANDLE;

    if (offscreen) {
        // create offscreen framebuffer with explicit textures
        bgfx::TextureHandle fbtextures[2];
        fbtextures[0] = bgfx::createTexture2D(
                uint16_t(width)
            , uint16_t(height)
            , false
            , 1
            , kDefaultColorFormat
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
            , getOffscreenDepthFormat()
            , BGFX_TEXTURE_RT
            | BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
            );

        assert(bgfx::isValid(fbtextures[0]));
        assert(bgfx::isValid(fbtextures[1]));
        fbh = bgfx::createFrameBuffer(2, fbtextures, true);

        bgfx::setViewFrameBuffer(view, fbh);
    }
    else
    {
        bgfx::reset(width, height, BGFX_RESET_VSYNC, kDefaultColorFormat);
    }

    if (clear) {
        bgfx::setViewClear(
            view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
    }
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);

    return fbh;
}

Point<uint16_t,2> Canvas::getBlitDepthSize(uint fbWidth, uint fbHeight)
{
    // get read depth size
    Point<uint16_t,2> blitDepthSize;
    const auto renderType = Context::renderType();
    switch (renderType) {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        blitDepthSize = {uint16_t(fbWidth), uint16_t(fbWidth)};
        break;
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::Metal:
        blitDepthSize = {1, 1};
        break;
    default:
        assert(false && "blit depth for untested render type");
        break;
    }

    return blitDepthSize;
}

bgfx::TextureFormat::Enum Canvas::getOffscreenDepthFormat()
{
    const auto renderType = Context::renderType();
    switch (renderType) {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
    case bgfx::RendererType::Vulkan:
        return bgfx::TextureFormat::D32F;
    case bgfx::RendererType::Metal:
        return bgfx::TextureFormat::D32;
    default:
        assert(false && "offscreen depth untested for current render type");
    }
    return bgfx::TextureFormat::Count;
}


} // namespace vcl
