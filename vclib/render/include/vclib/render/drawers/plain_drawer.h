/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_RENDER_DRAWERS_PLAIN_DRAWER_H
#define VCL_RENDER_DRAWERS_PLAIN_DRAWER_H

#include <vclib/types.h>

namespace vcl {

template<typename DerivedRenderApp>
class PlainDrawer
{
protected:
    using DRA = DerivedRenderApp;

public:
    PlainDrawer() = default;

    PlainDrawer(uint, uint) {}

    virtual void onInit(uint viewId) {}

    virtual void onResize(uint width, uint height) {}

    virtual void onDraw(uint viewId) { onDrawContent(viewId); }

    virtual void onDrawId(uint viewId) {}

    virtual void onDrawContent(uint viewId) {}

    virtual void onPostDraw() {}

protected:
    auto* derived() { return static_cast<DRA*>(this); }

    const auto* derived() const { return static_cast<const DRA*>(this); }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_PLAIN_DRAWER_H
