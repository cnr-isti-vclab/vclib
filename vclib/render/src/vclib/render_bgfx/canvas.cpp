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

// TODO: make at least the clear color configurable
static const uint32_t kDefaultClearColor   = 0xffffffff;
static const float    kDefaultClearDepth   = 1.0f;
static const uint8_t  kDefaultClearStencil = 0;

static const uint64_t kBlitFormat = 0
        | BGFX_TEXTURE_BLIT_DST
        | BGFX_TEXTURE_READ_BACK
        | BGFX_SAMPLER_MIN_POINT
        | BGFX_SAMPLER_MAG_POINT
        | BGFX_SAMPLER_MIP_POINT
        | BGFX_SAMPLER_U_CLAMP
        | BGFX_SAMPLER_V_CLAMP;

Canvas::Canvas(void* winId, uint width, uint height, void* displayId)
{
    // save window id
    mWinId = winId;

    // on screen framebuffer
    mViewId          = Context::requestViewId(mWinId, displayId);

    mTextView.init(width, height);

    // (re)create the framebuffers
    Canvas::onResize(width, height);
}

Canvas::~Canvas()
{
    // deallocate the framebuffers
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);
}

// TODO: remove bgfx screenshot alternative
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
    mSize = {width, height};

    // create window backbuffer
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    mFbh = createFrameBufferAndInitView(
        mWinId, mViewId, width, height, true);
    assert(!bgfx::isValid(mFbh)); // the canvas backbuffer is always non valid

    // resize the text view
    mTextView.resize(width, height);
}

void Canvas::frame()
{
    bgfx::setViewFrameBuffer(mViewId, mFbh);
    bgfx::touch(mViewId);
    draw();
    mTextView.frame(mFbh);

    const bool newReadRequested =
        (mReadRequest != std::nullopt && !mReadRequest->isSubmitted());

    if (newReadRequested) {
        // draw offscreen frame
        offscreenFrame();
        mCurrFrame = bgfx::frame();
        // submit the calls for blitting the offscreen depth buffer
        if (mReadRequest->submit()) {
            // solicit new frame
            this->update();
        }
    }
    else {
        mCurrFrame = bgfx::frame();
    }

    if (mReadRequest != std::nullopt && mReadRequest->isAvailable(mCurrFrame)) {
        // read depth data if available
        auto res = mReadRequest->readDepth();
        if (res.has_value()) {
            mReadRequest->callback({res.value()});
            mReadRequest = std::nullopt;
        }
        // solicit new frame
        this->update();
    }
}

[[nodiscard]]
bool Canvas::readDepth(
    const Point2i& point,
    CallbackReadBuffer callback)
{
    if (!supportsReadback()               // feature unsupported
        || mReadRequest != std::nullopt   // read already requested
        || point.x() < 0 || point.y() < 0 // point out of bounds
        || point.x() >= mSize.x() || point.y() >= mSize.y()) {
        return false;
    }

    mReadRequest.emplace(point, mSize, callback);
    return true;
}

bool Canvas::supportsReadback() const
{
    return (Context::capabilites().supported  &
        (BGFX_CAPS_TEXTURE_BLIT | BGFX_CAPS_TEXTURE_READ_BACK)) == 
        (BGFX_CAPS_TEXTURE_BLIT | BGFX_CAPS_TEXTURE_READ_BACK);
}

void Canvas::offscreenFrame()
{
    assert(mReadRequest != std::nullopt && !mReadRequest->isSubmitted());

    // render offscren
    bgfx::setViewFrameBuffer(
        mReadRequest->viewOffscreenId,
        mReadRequest->offscreenFbh);
    bgfx::touch(mReadRequest->viewOffscreenId);

    // render changing the view
    auto tmpId = mViewId;
    mViewId    = mReadRequest->viewOffscreenId;
    drawContent();
    mViewId = tmpId;
}

bgfx::FrameBufferHandle Canvas::createOffscreenFrameBufferAndInitView(
    bgfx::ViewId view,
    uint         width,
    uint         height,
    bool         clear)
{
    return createFrameBufferAndInitView(
        nullptr, view, width, height, clear);
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
        assert(bgfx::isValid(fbh));

        bgfx::setViewFrameBuffer(view, fbh);
    }
    else
    {
        bgfx::reset(width, height, BGFX_RESET_VSYNC, kDefaultColorFormat);
    }

    if (clear) {
        bgfx::setViewClear(
            view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            kDefaultClearColor, kDefaultClearDepth, kDefaultClearStencil);
    }
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);

    return fbh;
}

Point2<uint16_t> Canvas::getBlitDepthSize(Point2<uint> fbSize)
{
    assert(!fbSize.isZero());

    // get read depth size
    const auto renderType = Context::renderType();
    switch (renderType) {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        return {uint16_t(fbSize.x()), uint16_t(fbSize.y())};
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::Metal:
        return {1, 1};
    default:
        assert(false && "blit depth for untested render type");
        break;
    }

    return {0,0};
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
