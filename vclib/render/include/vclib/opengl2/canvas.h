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

#ifndef VCL_OPENGL2_CANVAS_H
#define VCL_OPENGL2_CANVAS_H

#include <vclib/io/image.h>
#include <vclib/render/concepts/render_app.h>
#include <vclib/render/read_buffer_types.h>
#include <vclib/space/core/color.h>
#include <vclib/space/core/point.h>
#include <vclib/types.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

namespace vcl {

/**
 * @brief The Canvas class describes a canvas on which opengl2 can draw.
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
 * The opengl2 rendering code should be implemented in the virtual member
 * function draw(), that must be reimplemented in the derived classes. This
 * method is called by the frame() method (that must be called in the render
 * loop).
 *
 * The class provides two important member functions:
 * - frame(): this function must be called by the derived classes at the end of
 * each frame, after all the opengl2 rendering commands have been issued;
 */
template<typename DerivedRenderApp>
class CanvasOpenGL2
{
protected:
    using FloatData = ReadBufferTypes::FloatData;
    using ByteData  = ReadBufferTypes::ByteData;
    using ReadData  = ReadBufferTypes::ReadData;

public:
    using CallbackReadBuffer = ReadBufferTypes::CallbackReadBuffer;

private:
    void* mWinId = nullptr;

    Point2<uint> mSize = {0, 0};

    vcl::Color mDefaultClearColor = vcl::Color::Black;

    CallbackReadBuffer mReadBufferCallback = nullptr;
    Point2i            mReadDepthPoint     = Point2i(-1, -1);

public:
    CanvasOpenGL2(
        void* winId,
        uint  width,
        uint  height,
        void* displayId = nullptr) : mWinId(winId), mSize(width, height)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");
    }

    ~CanvasOpenGL2() {}

    Point2<uint> size() const { return mSize; }

    uint viewId() const { return 0; }

    void setDefaultClearColor(const Color& color)
    {
        mDefaultClearColor = color;
        glClearColor(
            mDefaultClearColor.redF(),
            mDefaultClearColor.greenF(),
            mDefaultClearColor.blueF(),
            mDefaultClearColor.alphaF());
    }

    /**
     * @brief Request a screenshot of the canvas.
     *     The screenshot will be saved asynchronously.
     * @param filename The filename where the screenshot will be saved.
     * @param multiplier The multiplier applied to the canvas image.
     * @return true if the screenshot is requested, false otherwise.
     */
    bool screenshot(const std::string& filename, uint multiplier = 1)
    {
        return onScreenshot(filename, multiplier);
    }

    /**
     * @brief Automatically called by the DerivedRenderApp when the window
     * initializes.
     * Initialization is requires in some backends+window manager combinations,
     * and therefore it must be implemented (also if empty) in every Canvas
     * class.
     */
    void onInit()
    {
        glViewport(0, 0, mSize.x(), mSize.y());
        glClearColor(
            mDefaultClearColor.redF(),
            mDefaultClearColor.greenF(),
            mDefaultClearColor.blueF(),
            mDefaultClearColor.alphaF());
    }

    /**
     * @brief Automatically called by the DerivedRenderApp when the window
     * is resized.
     * @param width
     * @param height
     */
    void onResize(uint width, uint height)
    {
        mSize = {width, height};
        glViewport(0, 0, width, height);
    }

    /**
     * @brief Automatically called by the DerivedRenderApp when the window asks
     * to repaint.
     */
    void onPaint()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // if depth requested, read it
        if (mReadBufferCallback) {
            DerivedRenderApp::CNV::drawContent(derived());
            readDepthData();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        DerivedRenderApp::CNV::draw(derived());
        DerivedRenderApp::CNV::postDraw(derived());
    }

    /**
     * @brief Automatically called by the DerivedRenderApp when a drawer asks
     * to read the depth buffer at a specific point.
     *
     * @param point
     * @param callback
     * @return
     */
    [[nodiscard]] bool onReadDepth(
        const Point2i&     point,
        CallbackReadBuffer callback = nullptr)
    {
        if (point.x() < 0 || point.y() < 0 || // point out of bounds
            point.x() >= mSize.x() || point.y() >= mSize.y()) {
            return false;
        }

        mReadDepthPoint     = point;
        mReadBufferCallback = callback;
        return true;
    }

    /**
     * @brief Automatically called by the DerivedRenderApp when a drawer asks
     * for a screenshot.
     *
     * @param filename
     * @param width
     * @param height
     * @return
     */
    bool onScreenshot(const std::string& filename, uint multiplier = 1)
    {
        (void) multiplier; // not used

        std::vector<std::uint8_t> buffer(mSize.x() * mSize.y() * 4);
        // read pixels
        glReadPixels(
            0,
            0,
            GLsizei(mSize.x()),
            GLsizei(mSize.y()),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            buffer.data());

        // write image using stb
        bool ret = true;
        stbi_flip_vertically_on_write(1);
        try {
            saveImageData(filename, mSize.x(), mSize.y(), buffer.data());
        }
        catch (const std::exception& e) {
            ret = false;
        }
        stbi_flip_vertically_on_write(0);

        return ret;
    }

    /**
     * @brief Automatically called by the DerivedRenderApp when a drawer asks
     * to read the ID at a specific point.
     *
     * @param point The point where the ID must be read.
     * @param callback The callback function that will be called when the ID is
     * read.
     * @return always false
     * @note this function is not supported in opengl2
     */
    [[nodiscard]] bool onReadId(
        const Point2i&     point,
        CallbackReadBuffer callback = nullptr)
    {
        return false;
    }

private:
    void readDepthData()
    {
        // get depth range
        std::array<GLfloat, 2> depthRange = {0, 0};
        glGetFloatv(GL_DEPTH_RANGE, depthRange.data());

        // get viewport heigth only
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        // read depth
        GLfloat depth = depthRange[1];
        glReadPixels(
            GLint(mReadDepthPoint.x()),
            GLint(viewport[3] - mReadDepthPoint.y() - 1),
            1,
            1,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            &depth);

        // normalize depth into [0,1] interval
        depth = (depth - depthRange[0]) / (depthRange[1] - depthRange[0]);

        ReadData rd = FloatData({depth});

        // callback
        mReadBufferCallback(rd);

        // cleanup
        mReadDepthPoint     = {-1, -1};
        mReadBufferCallback = nullptr;
    }

    auto* derived() { return static_cast<DerivedRenderApp*>(this); }

    const auto* derived() const
    {
        return static_cast<const DerivedRenderApp*>(this);
    }
};

} // namespace vcl

#endif // VCL_OPENGL2_CANVAS_H
