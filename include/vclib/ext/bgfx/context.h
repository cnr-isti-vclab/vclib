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

#ifndef VCL_EXT_BGFX_CONTEXT_H
#define VCL_EXT_BGFX_CONTEXT_H

#include <stack>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <vclib/gui/native_window_handle.h>

namespace vcl::bgf {

class Context
{
    void* windowHandle  = nullptr;
    void* displayHandle = nullptr;

    std::stack<bgfx::ViewId> viewStack;

public:
    static Context& instance()
    {
        static Context ctx;
        return ctx;
    }

    static bgfx::ViewId requestViewId()
    {
        bgfx::ViewId viewId = instance().viewStack.top();
        instance().viewStack.pop();
        return viewId;
    }

    static void releaseViewId(bgfx::ViewId viewId)
    {
        instance().viewStack.push(viewId);
    }

private:
    Context()
    {
        windowHandle = vcl::createWindow("", 1, 1, displayHandle, true);
#ifdef __APPLE__
        bgfx::renderFrame(); // needed for macos
#endif                       // __APPLE__

        bgfx::Init init;
        init.platformData.nwh  = windowHandle;
        init.type              = bgfx::RendererType::Count;
        init.platformData.ndt  = displayHandle;
        init.resolution.width  = 1;
        init.resolution.height = 1;
        init.resolution.reset  = BGFX_RESET_NONE;
        bgfx::init(init);

        uint mv = bgfx::getCaps()->limits.maxViews;

        while (mv != 0) {
            viewStack.push((bgfx::ViewId) mv--);
        }
    };

    ~Context() { bgfx::shutdown(); };

public:
    Context(const Context&)            = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&)                 = delete;
    Context& operator=(Context&&)      = delete;
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_CONTEXT_H
