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

#include <vclib/io/image.h>

namespace vcl {

Canvas::Canvas(void* winId, uint width, uint height, void* displayId)
{
    // save window id
    mWinId = winId;

    // on screen framebuffer
    mViewId = Context::instance(mWinId, displayId).requestViewId();

    mTextView.init(width, height);

    // (re)create the framebuffers
    Canvas::onResize(width, height);
}

Canvas::~Canvas()
{
    // deallocate the framebuffers
    if (bgfx::isValid(mFbh))
        bgfx::destroy(mFbh);

    // release the view id
    auto& ctx = Context::instance();
    if (ctx.isValidViewId(mViewId))
        ctx.releaseViewId(mViewId);
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

    auto& ctx = Context::instance();
    mFbh =
        ctx.createFramebufferAndInitView(mWinId, mViewId, width, height, true);
    // the canvas framebuffer is non valid for the default window
    assert(ctx.isDefaultWindow(mWinId) == !bgfx::isValid(mFbh));

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

    if (mReadRequest != std::nullopt) {
        // read depth data if available
        const bool done = mReadRequest->performRead(mCurrFrame);
        if (done)
            mReadRequest = std::nullopt;
        // solicit new frame
        this->update();
    }
}

bool Canvas::readDepth(const Point2i& point, CallbackReadBuffer callback)
{
    if (!Context::instance().supportsReadback() // feature unsupported
        || mReadRequest != std::nullopt         // read already requested
        || point.x() < 0 || point.y() < 0       // point out of bounds
        || point.x() >= mSize.x() || point.y() >= mSize.y()) {
        return false;
    }

    mReadRequest.emplace(point, mSize, callback);
    return true;
}

bool Canvas::screenshot(const std::string& filename, uint width, uint height)
{
    if (!Context::instance().supportsReadback() // feature unsupported
        || mReadRequest != std::nullopt) {      // read already requested
        return false;
    }

    // get size
    auto size = mSize;
    if (width != 0 && height != 0)
        size = {width, height};

    // color data callback
    CallbackReadBuffer callback = [=](const ReadData& data) {
        assert(std::holds_alternative<ReadFramebufferRequest::ByteData>(data));
        const auto& d = std::get<ReadFramebufferRequest::ByteData>(data);

        // save rgb image data into file using stb depending on file
        try {
            vcl::saveImageData(filename, size.x(), size.y(), d.data());
        }
        catch (const std::exception& e) {
            std::cerr << "Error saving image: " << e.what() << std::endl;
        }
    };

    mReadRequest.emplace(size, callback);
    return true;
}

void Canvas::offscreenFrame()
{
    assert(mReadRequest != std::nullopt && !mReadRequest->isSubmitted());

    // render offscren
    bgfx::setViewFrameBuffer(
        mReadRequest->viewId(), mReadRequest->frameBuffer());
    bgfx::touch(mReadRequest->viewId());

    // render changing the view
    auto tmpId = mViewId;
    mViewId    = mReadRequest->viewId();
    drawContent();
    mViewId = tmpId;
}

} // namespace vcl
