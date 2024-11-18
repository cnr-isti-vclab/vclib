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

#include <iostream>

#include <vclib/render_bgfx/context.h>
#include <vclib/render_bgfx/system/native_window_handle.h>
#include <vclib/types/base.h>

namespace vcl {

bgfx::ViewId Context::requestViewId(void* windowHandle, void* displayHandle)
{
    bgfx::ViewId viewId =
        instance(windowHandle, displayHandle).mViewStack.top();
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

Context::Context(void* windowHandle, void* displayHandle)
{
    if (windowHandle == nullptr) {
        std::cerr << "WARNING: The first window used to create the bgfx "
                     "context is a dummy window. This is not recommended."
                  << std::endl;
        std::cerr
            << "Be sure to pass a valid window handle when requesting the "
               "context instance for the first time."
            << std::endl;
        mWindowHandle = vcl::createWindow("", 1, 1, mDisplayHandle, true);
    }
    else {
#ifdef __linux__
        assert(displayHandle != nullptr);
#endif
        mWindowHandle  = windowHandle;
        mDisplayHandle = displayHandle;
    }
#ifdef __APPLE__
    bgfx::renderFrame(); // needed for macos
#endif                   // __APPLE__

    bgfx::Init init;
    init.platformData.nwh  = mWindowHandle;
    init.type              = renderType;
    init.platformData.ndt  = mDisplayHandle;
    init.resolution.width  = 1;
    init.resolution.height = 1;
    init.resolution.reset  = BGFX_RESET_VSYNC;
    init.callback          = &mCallBack;
    bgfx::init(init);

    std::cerr << "READ BACK SUPPORTED: " <<
    ((bgfx::getCaps()->supported &
    (BGFX_CAPS_TEXTURE_BLIT|BGFX_CAPS_TEXTURE_READ_BACK)) == 
    (BGFX_CAPS_TEXTURE_BLIT|BGFX_CAPS_TEXTURE_READ_BACK)) << std::endl;

    if (windowHandle == nullptr) {
        vcl::closeWindow(mWindowHandle, mDisplayHandle);
    }

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

Context& Context::instance(void* windowHandle, void* displayHandle)
{
    static Context ctx(windowHandle, displayHandle);
    return ctx;
}

bool isViewValid(bgfx::ViewId viewId)
{
    return viewId <= bgfx::getCaps()->limits.maxViews;
}

} // namespace vcl
