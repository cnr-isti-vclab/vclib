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

#ifndef VCL_BENCHMARK_VIEWER_DRAWER
#define VCL_BENCHMARK_VIEWER_DRAWER

#include <vclib/render/drawers/benchmark_drawer.h>
#include <vclib/render/drawers/viewer_drawer.h>

namespace vcl {

template<typename DerivedRenderApp>
class BenchmarkViewerDrawer :
        public ViewerDrawer<BenchmarkDrawer<DerivedRenderApp>>
{
private:
    using Parent = ViewerDrawer<BenchmarkDrawer<DerivedRenderApp>>;

public:
    using Parent::onDraw;
    using Parent::onKeyPress;
    using Parent::onMouseDoubleClick;
    using Parent::Parent;

    void onInit(uint viewId) override { Parent::onInit(viewId); }

    void onDraw(uint viewId) override { Parent::onDraw(viewId); }

    void onDrawContent(uint viewId) override
    {
        Parent::onDrawContent(viewId);
        BenchmarkDrawer<DerivedRenderApp>::onDrawContent(viewId);
    }
};

} // namespace vcl

#endif
