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

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>

#include <vclib/types.h>

namespace vcl::bgf {

/**
 * @brief The Canvas class describes a canvas on which bgfx can draw.
 *
 * Its usage requires to have a window object that can be used to initialize
 * the canvas. The window object is not managed by the Canvas class, and can be
 * managed by any of the windowing systems or libraries that allow to retrieve
 * the native window handle.
 *
 * You can create your custom bgfx canvas class by inheriting from this class
 * and passing to the constructor the native window handle and the window size.
 * Alternatively to the constructor, you can also use the init() method to
 * initialize the canvas (if the required parameters are not available at
 * construction time).
 *
 * The class provides two important member functions:
 * - frame(): this function must be called at the end of each frame, after all
 * the bgfx rendering commands have been issued;
 * - resize(width, height): this function must be called whenever the window is
 * resized.
 */
class Canvas
{
    const bgfx::ViewId viewID = 0;

public:
    Canvas() {}

    Canvas(
        void*                    winID,
        uint                     width,
        uint                     height,
        void*                    displayID  = nullptr,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count)
    {
        init(winID, width, height, displayID, renderType);
    }

    Canvas(
        void*                    winID,
        uint                     width,
        uint                     height,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count)
    {
        init(winID, width, height, nullptr, renderType);
    }

    ~Canvas() { bgfx::shutdown(); }

    void init(
        void*                    winID,
        uint                     width,
        uint                     height,
        void*                    displayID  = nullptr,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count)
    {
#ifdef __APPLE__
        bgfx::renderFrame(); // needed for macos
#endif
        bgfx::Init init;
        init.platformData.nwh = winID;
        init.type             = renderType;

#ifdef __linux__
        init.platformData.ndt = displayID;
#endif

        init.resolution.width  = width;
        init.resolution.height = height;
        init.resolution.reset  = BGFX_RESET_VSYNC;
        bgfx::init(init);

        bgfx::setViewClear(
            0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(viewID, 0, 0, bgfx::BackbufferRatio::Equal);
        bgfx::touch(viewID);
    }

    void init(
        void*                    winID,
        uint                     width,
        uint                     height,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count)
    {
        init(winID, width, height, nullptr, renderType);
    }

    void frame() const
    {
        bgfx::touch(viewID);
        bgfx::frame();
    }

    void resize(uint width, uint height)
    {
        bgfx::reset(width, height, BGFX_RESET_VSYNC);
        bgfx::setViewRect(viewID, 0, 0, bgfx::BackbufferRatio::Equal);
        bgfx::touch(viewID);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_CANVAS_H
