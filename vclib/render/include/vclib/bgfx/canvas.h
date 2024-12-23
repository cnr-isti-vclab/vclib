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

#ifndef VCL_BGFX_CANVAS_H
#define VCL_BGFX_CANVAS_H

#include <vclib/types.h>

#include <vclib/bgfx/read_framebuffer_request.h>
#include <vclib/bgfx/system/native_window_handle.h>
#include <vclib/bgfx/text/text_view.h>
#include <vclib/io/image.h>
#include <vclib/render/input.h>
#include <vclib/render/concepts/renderer.h>

#include <optional>

namespace vcl {

/**
 * @brief The Canvas class describes a canvas on which bgfx can draw.
 *
 * Its usage requires to have a window object that can be used to initialize
 * the canvas. The window object is not managed by the Canvas class, and can be
 * managed by any of the windowing systems or libraries that allow to retrieve
 * the native window handle (e.g. Qt, GLFW...).
 *
 * You can create your custom bgfx canvas class by inheriting from this class
 * and passing to the constructor the native window handle and the window size.
 * Alternatively to the constructor, you can also use the init() method to
 * initialize the canvas (if the required parameters are not available at
 * construction time).
 *
 * This class does not provide a render loop, that must be implemented in one of
 * the derived classes. The render loop must call the frame() member function at
 * the end of each frame, after all the bgfx rendering commands have been
 * issued.
 *
 * The bgfx rendering code should be implemented in the virtual member function
 * draw(uint viewId), that must be reimplemented in the derived classes. This
 * method is called by the frame() method (that must be called in the render
 * loop), and its argument is the bgfx view ID.
 *
 * The class provides two important member functions:
 * - frame(): this function must be called by the derived classes at the end of
 * each frame, after all the bgfx rendering commands have been issued;
 * - onResize(width, height): this function must be called by the derived
 * classes whenever the window is resized.
 */
template<typename DerivedRenderer>
class CanvasBGFX
{
    using DRT = DerivedRenderer;

public:
    using ReadFramebufferRequest = detail::ReadFramebufferRequest;
    using CallbackReadBuffer     = ReadFramebufferRequest::CallbackReadBuffer;
    using ReadData               = ReadFramebufferRequest::ReadData;

private:
    void* mWinId = nullptr;

    // frame buffer for drawing the canvas
    // BGFX_INVALID_HANDLE represents the default frame buffer of the window
    bgfx::ViewId            mViewId = BGFX_INVALID_VIEW;
    bgfx::FrameBufferHandle mFbh    = BGFX_INVALID_HANDLE;

    // size of the canvas
    Point2<uint> mSize = {0, 0};

    // TODO background color

    // current frame
    uint32_t mCurrFrame = 0;

    // offscreen readback request
    std::optional<ReadFramebufferRequest> mReadRequest = std::nullopt;

    TextView mTextView;

    // flags
    bool mStatsEnabled = false;

public:
    CanvasBGFX(void* winId, uint width, uint height, void* displayId = nullptr)
    {
        static_assert(
            RendererConcept<DRT>,
            "The DerivedRenderer must satisfy the RendererConcept.");

        // save window id
        mWinId = winId;

        // on screen framebuffer
        mViewId = Context::instance(mWinId, displayId).requestViewId();

        mTextView.init(width, height);

        // (re)create the framebuffers
        onResize(width, height);
    }

    ~CanvasBGFX()
    {
        // deallocate the framebuffers
        if (bgfx::isValid(mFbh))
            bgfx::destroy(mFbh);

        // release the view id
        auto& ctx = Context::instance();
        if (ctx.isValidViewId(mViewId))
            ctx.releaseViewId(mViewId);
    }

    void onInit() {}

    Point2<uint> size() const { return mSize; }

    bgfx::ViewId viewId() const { return mViewId; }

    // text
    void enableText(bool b = true) { mTextView.enableText(b); }

    bool isTextEnabled() const { return mTextView.isTextEnabled(); }

    void setTextFont(VclFont::Enum font, uint fontSize)
    {
        mTextView.setTextFont(font, fontSize);
    }

    void setTextFont(const std::string& fontName, uint fontSize)
    {
        mTextView.setTextFont(fontName, fontSize);
    }

    void clearText()
    {
        mTextView.clearText();
    }

    void appendStaticText(
        const Point2f&     pos,
        const std::string& text,
        const Color&       color = Color::Black)
    {
        mTextView.appendStaticText(pos, text, color);
    }

    void appendTransientText(
        const Point2f&     pos,
        const std::string& text,
        const Color&       color = Color::Black)
    {
        mTextView.appendTransientText(pos, text, color);
    }

    void onKeyPress(Key::Enum key)/* override*/
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

    [[nodiscard]] bool readDepth(
        const Point2i&     point,
        CallbackReadBuffer callback = nullptr)
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

    bool screenshot(
        const std::string& filename,
        uint               width  = 0,
        uint               height = 0)
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

    // using private inheritance on DerivedRenderer, no need to protect here
    /**
     * @brief Automatically called by the DerivedRenderer when the window
     * is resized.
     * @param width
     * @param height
     */
    void onResize(uint width, uint height)
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

    /**
     * @brief Automatically called by the DerivedRenderer when the window asks
     * to repaint.
     */
    void onPaint()
    {
        bgfx::setViewFrameBuffer(mViewId, mFbh);
        bgfx::touch(mViewId);
        // ask the derived frame to draw all the drawer objects:
        DRT::CNV::draw(derived());
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
                DRT::CNV::update(derived());
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
            DRT::CNV::update(derived());
        }

        // this is probably required only when using Qt
#if defined(__APPLE__) || defined(__linux__)
        bgfx::frame(); // needed on unix systems
#endif             // __APPLE__ || __linux__
    }

private:
    // draw offscreen frame
    void offscreenFrame()
    {
        assert(mReadRequest != std::nullopt && !mReadRequest->isSubmitted());

        // render offscren
        bgfx::setViewFrameBuffer(
            mReadRequest->viewId(), mReadRequest->frameBuffer());
        bgfx::touch(mReadRequest->viewId());

        // render changing the view
        auto tmpId = mViewId;
        mViewId    = mReadRequest->viewId();
        DRT::CNV::drawContent(derived());
        mViewId = tmpId;
    }

    auto* derived() { return static_cast<DRT*>(this); }

    const auto* derived() const { return static_cast<const DRT*>(this); }
};

} // namespace vcl

#endif // VCL_BGFX_CANVAS_H
