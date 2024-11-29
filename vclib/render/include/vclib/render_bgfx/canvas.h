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
#include <variant>

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
    void*                   mWinId  = nullptr;
    // frame buffer for drawing the canvas
    // BGFX_INVALID_HANDLE represents the default frame buffer of the window
    bgfx::FrameBufferHandle mFbh          = BGFX_INVALID_HANDLE;
    bgfx::ViewId            mViewId = 0;

    // size of the canvas
    Point2<uint> mSize = {0, 0};

    // current frame
    uint32_t               mCurrFrame      = 0;

    // depth readback
    class ReadBufferRequest
    {
    public:
        enum Type {
            COLOR = 0, // entire color buffer
            DEPTH = 1, // single pixel depth
            COUNT = 2
        };

        using FloatData = std::vector<float>;
        using ByteData  = std::vector<uint8_t>;
        using ReadData  = std::variant<FloatData, ByteData>;
        using CallbackReadBuffer = std::function<void(const ReadData &)>;

    private:
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
        bgfx::TextureHandle blitTexture  = BGFX_INVALID_HANDLE;
        Point2<uint16_t>    blitSize     = {0, 0};
        // data read from the blit texture
        ReadData            readData     = {};
        // callback called when the data is available
        CallbackReadBuffer  readCallback = nullptr;
        // submitted flag
        bool submitted = false;

    public:
        // Read depth constructor
        ReadBufferRequest(
            Point2i queryDepthPoint,
            Point2<uint> framebufferSize,
            CallbackReadBuffer callback)
            : type(DEPTH), point(queryDepthPoint), readCallback(callback) {
                
                blitSize = getBlitDepthSize(framebufferSize);

                // request a new view id (hopefully with no parameters)
                viewOffscreenId = Context::requestViewId();
                
                offscreenFbh = createOffscreenFrameBufferAndInitView(
                    viewOffscreenId,
                    framebufferSize.x(),
                    framebufferSize.y(),
                    true);
                assert(bgfx::isValid(offscreenFbh));

                // create the blit depth texture
                blitTexture = bgfx::createTexture2D(
                    uint16_t(blitSize.x()),
                    uint16_t(blitSize.y()),
                    false,
                    1,
                    getOffscreenDepthFormat(),
                    kBlitFormat);
                assert(bgfx::isValid(blitTexture));
            }
        // read color constructor
        ReadBufferRequest(
            Point2<uint> framebufferSize,
            CallbackReadBuffer callback)
            : type(COLOR), point(0,0), readCallback(callback) {
                
                blitSize = framebufferSize.cast<uint16_t>();

                // request a new view id (hopefully with no parameters)
                viewOffscreenId = Context::requestViewId();
                
                offscreenFbh = createOffscreenFrameBufferAndInitView(
                    viewOffscreenId,
                    framebufferSize.x(),
                    framebufferSize.y(),
                    true);
                assert(bgfx::isValid(offscreenFbh));

                // create the blit depth texture
                blitTexture = bgfx::createTexture2D(
                    uint16_t(blitSize.x()),
                    uint16_t(blitSize.y()),
                    false,
                    1,
                    getOffscreenColorFormat(),
                    kBlitFormat);
                assert(bgfx::isValid(blitTexture));
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

        bgfx::ViewId viewId() const {
            return viewOffscreenId;
        }

        bgfx::FrameBufferHandle frameBuffer() const {
            return offscreenFbh;
        }

        bool isSubmitted() const {
            return submitted;
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
            if (submitted)
                return false;
            
            // pixel size
            const auto readPixelSize = blitSize.x() * blitSize.y();

            // source buffer
            const auto srcBuffer = bgfx::getTexture(
                offscreenFbh,
                uint8_t(type));

            switch (type) {
            case DEPTH: {
                // allocate memory for blit depth data
                readData = FloatData(readPixelSize);
                if (readPixelSize == 1) {
                    // read a single fragment
                    bgfx::blit(viewOffscreenId, blitTexture, 0, 0,
                        srcBuffer, uint16_t(point.x()), uint16_t(point.y()),
                        1, 1);
                } else {
                    // read the entire depth buffer
                    bgfx::blit(viewOffscreenId, blitTexture, 0, 0, srcBuffer);
                }
                // submit read from blit CPU texture
                frameAvailable = bgfx::readTexture(
                    blitTexture,
                    std::get<FloatData>(readData).data());
            }
            break;
            case COLOR: {
                // allocate memory for blit color data
                readData = ByteData(readPixelSize * 4);

                // read the entire depth buffer
                bgfx::blit(viewOffscreenId, blitTexture, 0, 0, srcBuffer);

                frameAvailable = bgfx::readTexture(
                    blitTexture,
                    std::get<ByteData>(readData).data());
            }
            break;
            default:
                assert(false && "unsupported readback type");
                return false;
            }

            submitted = true;
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

        bool performRead(uint32_t currFrame) const {
            if (!isAvailable(currFrame))
                return false;

            switch (type) {
            case DEPTH: {
                assert(std::holds_alternative<FloatData>(readData));
                const auto & data = std::get<FloatData>(readData);
                if (data.size() == 1)
                    this->readCallback(readData);
                else {
                    this->readCallback(FloatData(
                        {data[point.y() * blitSize.x() + point.x()]}));
                }
            }
            break;
            case COLOR: {
                assert(std::holds_alternative<ByteData>(readData));
                this->readCallback(readData);
            }
            break;
                default:
                assert(false && "unsupported readback type");
            }
            return true;
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

    using CallbackReadBuffer = ReadBufferRequest::CallbackReadBuffer;
    using ReadData = ReadBufferRequest::ReadData;

    [[nodiscard]]
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
    static bgfx::TextureFormat::Enum getOffscreenColorFormat();
    static bgfx::TextureFormat::Enum getOffscreenDepthFormat();
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_CANVAS_H
