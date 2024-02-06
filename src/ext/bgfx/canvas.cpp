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

#include <vclib/ext/bgfx/canvas.h>

#include <vclib/gui/native_window_handle.h>

namespace vcl::bgf {

Canvas::Canvas()
{
}

Canvas::Canvas(void* winID, uint width, uint height, void* displayID)
{
    init(winID, width, height, displayID);
}

Canvas::Canvas(void* winID, uint width, uint height)
{
    init(winID, width, height, nullptr);
}

Canvas::~Canvas()
{
    if (bgfx::isValid(fbh))
        bgfx::destroy(fbh);
    Context::releaseViewId(viewID);
}

void Canvas::init(void* winID, uint width, uint height, void* displayID)
{
    this->winID = winID;

    viewID = Context::requestViewId();

    fbh = bgfx::createFrameBuffer(winID, width, height);

    bgfx::setViewFrameBuffer(viewID, fbh);
    bgfx::setViewClear(
        viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
    bgfx::setViewRect(viewID, 0, 0, width, height);
    bgfx::touch(viewID);
}

void Canvas::init(void* winID, uint width, uint height)
{
    init(winID, width, height, nullptr);
}


void Canvas::screenShot(const std::string& filename, uint width, uint height)
{
    if (width == 0 || height == 0) {
        draw(viewID);
        bgfx::requestScreenShot(fbh, filename.c_str());
        bgfx::frame();
    }
    else {
        void* d;
        void* w = vcl::createWindow("", width, height, d, true);
        bgfx::ViewId v = Context::requestViewId();
        bgfx::FrameBufferHandle fbh = bgfx::createFrameBuffer(w, width, height);
        bgfx::setViewFrameBuffer(v, fbh);
        bgfx::setViewClear(
            v, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
        bgfx::setViewRect(v, 0, 0, width, height);
        bgfx::touch(v);
        draw(v);
        bgfx::requestScreenShot(fbh, filename.c_str());
        bgfx::frame();

        bgfx::destroy(fbh);
        Context::releaseViewId(v);
        vcl::closeWindow(w, d);
    }
}

void Canvas::frame()
{
    bgfx::setViewFrameBuffer(viewID, fbh);
    // bgfx::touch(viewID);
    draw(viewID);
    bgfx::frame();
}

void Canvas::resize(uint width, uint height)
{
    if (bgfx::isValid(fbh))
        bgfx::destroy(fbh);

    fbh = bgfx::createFrameBuffer(winID, width, height);
    bgfx::setViewFrameBuffer(viewID, fbh);
    bgfx::setViewRect(viewID, 0, 0, width, height);
    bgfx::touch(viewID);
}

} // namespace vcl::bgf
