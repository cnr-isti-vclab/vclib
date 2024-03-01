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

#ifndef VCL_EXT_BGFX_CONTEXT_H
#define VCL_EXT_BGFX_CONTEXT_H

#include <stack>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "callback.h"
#include "text/font_map.h"

namespace vcl::bgf {

class Context
{
    void* windowHandle  = nullptr;
    void* displayHandle = nullptr;

    std::stack<bgfx::ViewId> viewStack;

    Callback cb;
    FontMap* fm = nullptr;

public:
    inline static bgfx::RendererType::Enum renderType =
        bgfx::RendererType::Count;

    static bgfx::ViewId requestViewId();

    static void releaseViewId(bgfx::ViewId viewId);

    static FontMap& fontManager();

private:
    Context();

    ~Context();

    static Context& instance();

public:
    Context(const Context&)            = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&)                 = delete;
    Context& operator=(Context&&)      = delete;
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_CONTEXT_H
