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

#ifndef VCL_OPENGL2_RENDER_CANVAS_H
#define VCL_OPENGL2_RENDER_CANVAS_H

#include <vclib/types.h>

#include <vclib/render/interfaces/event_manager_i.h>

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
class Canvas : public virtual vcl::EventManagerI
{
    void* mWinId = nullptr;

public:
    Canvas(void* winId, uint width, uint height, void* displayId = nullptr);

    ~Canvas() {}

    void init(uint width, uint height);

    void screenShot(
        const std::string& filename,
        uint               width  = 0,
        uint               height = 0);

protected:
    virtual void draw() = 0;

    void onResize(uint width, uint height) override;

    void frame();
};

} // namespace vcl

#endif // VCL_OPENGL2_RENDER_CANVAS_H
