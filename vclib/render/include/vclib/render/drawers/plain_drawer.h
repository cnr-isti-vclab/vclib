// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWERS_PLAIN_DRAWER_H
#define VCL_RENDER_DRAWERS_PLAIN_DRAWER_H

#include <vclib/base.h>

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
