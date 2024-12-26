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

#ifndef VCL_OPENGL2_CANVAS_H
#define VCL_OPENGL2_CANVAS_H

#include <vclib/io/image.h>
#include <vclib/render/concepts/renderer.h>
#include <vclib/render/read_buffer_types.h>
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
template<typename DerivedRenderer>
class CanvasOpenGL2
{
    using DRT = DerivedRenderer;

protected:
    using FloatData          = ReadBufferTypes::FloatData;
    using ByteData           = ReadBufferTypes::ByteData;
    using ReadData           = ReadBufferTypes::ReadData;

public:
    using CallbackReadBuffer = ReadBufferTypes::CallbackReadBuffer;

private:
    void* mWinId = nullptr;

    Point2<uint> mSize = {0, 0};

    CallbackReadBuffer mReadBufferCallback = nullptr;
    Point2i            mReadDepthPoint     = Point2i(-1, -1);

public:
    CanvasOpenGL2(
        void* winId,
        uint  width,
        uint  height,
        void* displayId = nullptr)
    {
        static_assert(
            RendererConcept<DRT>,
            "The DerivedRenderer must satisfy the RendererConcept.");

        mSize = {width, height};
    }

    ~CanvasOpenGL2() {}

    Point2<uint> size() const { return mSize; }

    uint viewId() const { return 0; }

    /**
     * @brief Automatically called by the DerivedRenderer when the window
     * initializes.
     * Initialization is requires in some backends+window manager combinations,
     * and therefore it must be implemented (also if empty) in every Canvas
     * class.
     */
    void onInit()
    {
        glViewport(0, 0, mSize.x(), mSize.y());
        glClearColor(1.f, 1.f, 1.f, 1.f);
    }

    /**
     * @brief Automatically called by the DerivedRenderer when the window
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
     * @brief Automatically called by the DerivedRenderer when the window asks
     * to repaint.
     */
    void onPaint()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // if depth requested, read it
        if (mReadBufferCallback) {
            DRT::CNV::drawContent(derived());
            readDepthData();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        DRT::CNV::draw(derived());
    }

    /**
     * @brief Automatically called by the DerivedRenderer when a drawer asks
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
     * @brief Automatically called by the DerivedRenderer when a drawer asks
     * for a screenshot.
     *
     * @param filename
     * @param width
     * @param height
     * @return
     */
    bool onScreenshot(
        const std::string& filename,
        uint               width  = 0,
        uint               height = 0)
    {
        (void) width;
        (void) height;

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

    auto* derived() { return static_cast<DRT*>(this); }

    const auto* derived() const { return static_cast<const DRT*>(this); }
};

} // namespace vcl

#endif // VCL_OPENGL2_CANVAS_H
