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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_CANVAS_H
#define VCL_EXT_BGFX_CANVAS_H

#include "context.h"

#include <vclib/types.h>

namespace vcl::bgf {

/**
 * @brief The Canvas class describes a canvas on which bgfx can draw.
 *
 * Its usage requires to have a window object that can be used to initialize
 * the canvas. The window object is not managed by the Canvas class, and can be
 * managed by any of the windowing systems or libraries that allow to retrieve
 * the native window handle (e.g. Qt, Imgui, GLFW...).
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
 * draw(uint viewID), that can be reimplemented in the derived classes. This
 * method is called by the frame() method (that must be called in the render
 * loop), and its argument is the bgfx view ID.
 *
 * The class provides two important member functions:
 * - frame(): this function must be called by the derived classes at the end of
 * each frame, after all the bgfx rendering commands have been issued;
 * - resize(width, height): this function must be called by the derived classes
 * whenever the window is resized.
 */
class Canvas
{
    void*                   winID  = nullptr;
    bgfx::ViewId            viewID = 0;
    bgfx::FrameBufferHandle fbh;

public:
    Canvas();

    Canvas(void* winID, uint width, uint height, void* displayID = nullptr);

    Canvas(void* winID, uint width, uint height);

    ~Canvas();

    void init(void* winID, uint width, uint height, void* displayID = nullptr);

    void init(void* winID, uint width, uint height);

    virtual void draw(uint viewID) = 0;

    void screenShot(const std::string& filename);

protected:
    void frame();

    void resize(uint width, uint height);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_CANVAS_H
