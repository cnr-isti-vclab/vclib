/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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
#include <vclib/bgfx/text/text_view.h>
#include <vclib/render/interfaces/event_manager_i.h>

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
class CanvasBGFX : public virtual vcl::EventManagerI
{
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

    vcl::Color mDefaultClearColor = vcl::Color::Black;

    // current frame
    uint32_t mCurrFrame = 0;

    // offscreen readback request
    std::optional<ReadFramebufferRequest> mReadRequest = std::nullopt;

    TextView mTextView;

    // flags
    bool mStatsEnabled = false;

public:
    CanvasBGFX(void* winId, uint width, uint height, void* displayId = nullptr);

    ~CanvasBGFX();

    Point2<uint> size() const { return mSize; }

    bgfx::ViewId viewId() const { return mViewId; }

    void setDefaultClearColor(const Color& color);

    // text
    void enableText(bool b = true);
    bool isTextEnabled() const;

    void setTextFont(VclFont::Enum font, uint fontSize);
    void setTextFont(const std::string& fontName, uint fontSize);

    void clearText();

    void appendStaticText(
        const Point2f&     pos,
        const std::string& text,
        const Color&       color = Color::Black);

    void appendTransientText(
        const Point2f&     pos,
        const std::string& text,
        const Color&       color = Color::Black);

    void onKeyPress(Key::Enum key) override;

    bool supportsReadback() const;

    [[nodiscard]] bool readDepth(
        const Point2i&     point,
        CallbackReadBuffer callback = nullptr);

    bool screenshot(
        const std::string& filename,
        uint               width  = 0,
        uint               height = 0);

protected:
    virtual void draw() { drawContent(); };

    virtual void drawContent() = 0;

    void onResize(uint width, uint height) override;

    virtual void frame();

private:
    // draw offscreen frame
    void offscreenFrame();
};

} // namespace vcl

#endif // VCL_BGFX_CANVAS_H
