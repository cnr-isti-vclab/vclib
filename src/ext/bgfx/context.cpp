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

#include <vclib/ext/bgfx/context.h>

#include <vclib/gui/native_window_handle.h>
#include <vclib/types/base.h>

namespace vcl::bgf {

Context& Context::instance()
{
    static Context ctx;
    return ctx;
}

bgfx::ViewId Context::requestViewId()
{
    bgfx::ViewId viewId = instance().viewStack.top();
    instance().viewStack.pop();
    return viewId;
}

void Context::releaseViewId(bgfx::ViewId viewId)
{
    instance().viewStack.push(viewId);
}

FontManager& Context::fontManager()
{
    return *instance().fm;
}

ProgramManager& Context::programManager()
{
    return *instance().pm;
}

Context::Context()
{
    windowHandle = vcl::createWindow("", 1, 1, displayHandle, true);
#ifdef __APPLE__
    bgfx::renderFrame(); // needed for macos
#endif                   // __APPLE__

    bgfx::Init init;
    init.platformData.nwh  = windowHandle;
    init.type              = renderType;
    init.platformData.ndt  = displayHandle;
    init.resolution.width  = 1;
    init.resolution.height = 1;
    init.resolution.reset  = BGFX_RESET_NONE;
    init.callback          = &cb;
    bgfx::init(init);

    vcl::closeWindow(windowHandle, displayHandle);

    uint mv = bgfx::getCaps()->limits.maxViews;

    while (mv != 0) {
        viewStack.push((bgfx::ViewId) mv--);
    }
    viewStack.push((bgfx::ViewId) 0);

    // font manager must be created after bgfx::init
    fm = new FontManager();
    pm = new ProgramManager();
}

Context::~Context()
{
    delete fm;
    delete pm;
    bgfx::shutdown();
}

bool isViewValid(bgfx::ViewId viewId)
{
    return viewId <= bgfx::getCaps()->limits.maxViews;
}

} // namespace vcl::bgf
