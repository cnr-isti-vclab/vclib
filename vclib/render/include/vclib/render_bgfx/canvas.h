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

#ifndef VCL_BGFX_RENDER_CANVAS_H
#define VCL_BGFX_RENDER_CANVAS_H

#include <vclib/types.h>

#include <vclib/render/interfaces/event_manager_i.h>
#include <vclib/render_bgfx/text/text_view.h>

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
 * - onResize(width, height): this function must be called by the derived classes
 * whenever the window is resized.
 */
class Canvas : public virtual vcl::EventManagerI
{
public:
    using CallbackReadBuffer = std::function<void(std::vector<float>)>;

private:

    void*                   mWinId  = nullptr;
    // frame buffer for drawing the canvas
    // BGFX_INVALID_HANDLE represents the default frame buffer of the window
    bgfx::FrameBufferHandle mFbh          = BGFX_INVALID_HANDLE;
    bgfx::ViewId            mViewId = 0;

    // size of the canvas
    Point2<uint> mSize = {0, 0};

    // blit textures
    // bgfx::TextureHandle mBlitColor        = BGFX_INVALID_HANDLE;
    // blit data
    // std::vector<uint32_t> mColorData      = {};

    // current frame
    uint32_t               mCurrFrame      = 0;

    // depth readback
    struct ReadBufferRequest
    {
        enum Type {
            COLOR = 0,
            DEPTH = 1,
            COUNT = 2
        };

        // Read depth constructor
        ReadBufferRequest(
            Point2i queryDepthPoint,
            Point2<uint> framebufferSize,
            CallbackReadBuffer callback)
            : type(DEPTH), point(queryDepthPoint), callback(callback) {
                
                blitSize = getBlitDepthSize(framebufferSize);

                // request a new view id (hopefully with no parameters)
                viewOffscreenId = Context::requestViewId();

                // create offscreen framebuffer
                if (bgfx::isValid(offscreenFbh))
                    bgfx::destroy(offscreenFbh);
                
                offscreenFbh = createFrameBufferAndInitView(
                    nullptr,
                    viewOffscreenId,
                    framebufferSize.x(),
                    framebufferSize.y(),
                    true);
                assert(bgfx::isValid(offscreenFbh));
            }

        ~ReadBufferRequest() {
            if (bgfx::isValid(blitTexture))
                bgfx::destroy(blitTexture);

            if (bgfx::isValid(offscreenFbh))
                bgfx::destroy(offscreenFbh);
            
            if (viewOffscreenId != 0)
                Context::releaseViewId(viewOffscreenId);
        }

        ReadBufferRequest& operator=(ReadBufferRequest&& right) = default;

        using FloatData = std::vector<float>;
        using ByteData  = std::vector<uint8_t>;
        using ReadData  = std::variant<FloatData, ByteData>;

        // read back type
        Type                  type           = COUNT;

        // frame # when data will be available for reading
        uint32_t              frameAvailable = 0;
        // point to read from
        Point2i               point          = {-1, -1};

        // frame buffer for offscreen drawing and reading back
        bgfx::FrameBufferHandle offscreenFbh    = BGFX_INVALID_HANDLE;
        // view id for offscreen drawing
        bgfx::ViewId            viewOffscreenId = 0;

        // blit texture
        bgfx::TextureHandle blitTexture = BGFX_INVALID_HANDLE;
        Point2<uint16_t>    blitSize    = {0, 0};
        // data read from the blit texture
        ReadData            readData    = {};
        // callback called when the data is available
        CallbackReadBuffer  callback    = nullptr;

        bool isSubmitted() const {
            return bgfx::isValid(blitTexture);
        }

        static const uint64_t kBlitFormat = 0
        | BGFX_TEXTURE_BLIT_DST
        | BGFX_TEXTURE_READ_BACK
        | BGFX_SAMPLER_MIN_POINT
        | BGFX_SAMPLER_MAG_POINT
        | BGFX_SAMPLER_MIP_POINT
        | BGFX_SAMPLER_U_CLAMP
        | BGFX_SAMPLER_V_CLAMP;

        bool submit() {
            if (isSubmitted())
                return false;
            
            switch (type) {
            case DEPTH: {
                // TODO: move to the constructor?
                // create the blit depth texture
                assert(!bgfx::isValid(blitTexture));
                blitTexture = bgfx::createTexture2D(
                    uint16_t(blitSize.x()),
                    uint16_t(blitSize.y()),
                    false,
                    1,
                    getOffscreenDepthFormat(),
                    kBlitFormat);
                assert(bgfx::isValid(blitTexture));

                // allocate memory for blit depth data
                const auto readSize = blitSize.x() * blitSize.y();
                readData = std::vector<float>(readSize);

                // blit depth buffer
                const auto depthBuffer = bgfx::getTexture(
                    offscreenFbh,
                    uint8_t(type));
                if (readSize == 1) {
                    // read a single fragment
                    bgfx::blit(viewOffscreenId, blitTexture, 0, 0,
                        depthBuffer, uint16_t(point.x()), uint16_t(point.y()),
                        1, 1);
                } else {
                    // read the entire depth buffer
                    bgfx::blit(viewOffscreenId, blitTexture, 0, 0, depthBuffer);
                }
            }
            break;
            case COLOR: {
                // TODO: implement color readback
                // blit the color buffer
                const auto colorBuffer = bgfx::getTexture(offscreenFbh, 0);
                bgfx::blit(viewOffscreenId, blitTexture, 0, 0, colorBuffer);
            }
            break;
            default:
                assert(false && "unsupported readback type");
                return false;
            }

            // submit read from blit CPU texture
            frameAvailable = bgfx::readTexture(
                blitTexture,
                std::get<FloatData>(readData).data());
            return true;
        }

        bool isAvailable(uint32_t currentFrame) const {
            return frameAvailable != 0 
                && currentFrame >= frameAvailable;
        }

        std::optional<float> readDepth() const {
            if (type != DEPTH)
                return std::nullopt;

            assert(std::holds_alternative<FloatData>(readData));
            const auto & data = std::get<FloatData>(readData);
            if (data.size() == 1)
                return data[0];
            else
                return data[point.y() * blitSize.x() + point.x()];
        }
    };
    std::optional<ReadBufferRequest> mReadRequest = std::nullopt;

    TextView mTextView;

    // flags
    bool mStatsEnabled = false;

public:
    Canvas(void* winId, uint width, uint height, void* displayId = nullptr);

    ~Canvas();

    Point2<uint> size() const { return mSize; }

    bgfx::ViewId viewId() const { return mViewId; }

    void screenShot(
        const std::string& filename,
        uint               width  = 0,
        uint               height = 0);

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

    bool readDepth(
        const Point2i& point,
        CallbackReadBuffer callback = nullptr);

protected:
    virtual void draw() { drawContent(); };

    virtual void drawContent() = 0;

    void onResize(uint width, uint height) override;

    void frame();

private:
    // draw offscreen frame
    void offscreenFrame();

    // submit the calls for blitting the offscreen depth buffer
    // and reading it back
    // void submitReadDepth();

    static bgfx::FrameBufferHandle createOffscreenFrameBufferAndInitView(
        bgfx::ViewId view,
        uint         width,
        uint         height,
        bool         clear = false);

    static bgfx::FrameBufferHandle createFrameBufferAndInitView(
        void*        winId,
        bgfx::ViewId view,
        uint         width,
        uint         height,
        bool         clear = false);

    static Point2<uint16_t> getBlitDepthSize(Point2<uint> fbSize);
    static bgfx::TextureFormat::Enum getOffscreenDepthFormat();
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_CANVAS_H
