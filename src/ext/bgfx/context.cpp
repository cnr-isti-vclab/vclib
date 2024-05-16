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
    bgfx::ViewId viewId = instance().mViewStack.top();
    instance().mViewStack.pop();
    return viewId;
}

void Context::releaseViewId(bgfx::ViewId viewId)
{
    instance().mViewStack.push(viewId);
}

FontManager& Context::fontManager()
{
    return *instance().mFontManager;
}

ProgramManager& Context::programManager()
{
    return *instance().mProgramManager;
}

void Context::setDebugVerbosity(bool verbose)
{
    instance().mCallBack.setDebugVerbosity(verbose);
}

Context::Context()
{
    mWindowHandle = vcl::createWindow("", 1, 1, mDisplayHandle, true);
#ifdef __APPLE__
    bgfx::renderFrame(); // needed for macos
#endif                   // __APPLE__

    bgfx::Init init;
    init.platformData.nwh  = mWindowHandle;
    init.type              = renderType;
    init.platformData.ndt  = mDisplayHandle;
    init.resolution.width  = 1;
    init.resolution.height = 1;
    init.resolution.reset  = BGFX_RESET_NONE;
    init.callback          = &mCallBack;
    bgfx::init(init);

    vcl::closeWindow(mWindowHandle, mDisplayHandle);

    uint mv = bgfx::getCaps()->limits.maxViews;

    while (mv != 0) {
        mViewStack.push((bgfx::ViewId) mv--);
    }
    mViewStack.push((bgfx::ViewId) 0);

    // font manager must be created after bgfx::init
    mFontManager    = new FontManager();
    mProgramManager = new ProgramManager(bgfx::getCaps()->rendererType);
}

Context::~Context()
{
    delete mFontManager;
    delete mProgramManager;
    bgfx::shutdown();
}

bool isViewValid(bgfx::ViewId viewId)
{
    return viewId <= bgfx::getCaps()->limits.maxViews;
}

} // namespace vcl::bgf
