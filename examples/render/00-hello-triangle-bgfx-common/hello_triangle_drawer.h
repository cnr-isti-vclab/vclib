// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HELLO_TRIANGLE_DRAWER_H
#define HELLO_TRIANGLE_DRAWER_H

#include "common.h"

#include <vclib/render/drawers/plain_drawer.h>

template<typename DerivedRenderApp>
class HelloTriangleDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{
public:
    HelloTriangleDrawer(vcl::uint width = 1024, vcl::uint height = 768) {}

    ~HelloTriangleDrawer()
    {
        if (bgfx::isValid(vbh))
            bgfx::destroy(vbh);
        if (bgfx::isValid(program))
            bgfx::destroy(program);
    }

    void onInit(vcl::uint viewId) override { setUpBGFX(viewId, vbh, program); }

    void onResize(vcl::uint width, vcl::uint height) override
    {
        std::cout << "Resize: " << width << "; " << height
                  << ". Nothing to do\n";
    }

    void onDrawContent(vcl::uint viewId) override
    {
        drawOnView(viewId, vbh, program);
    }

    void onDraw(vcl::uint viewId) override { onDrawContent(viewId); }

private:
    bgfx::VertexBufferHandle vbh;

    bgfx::ProgramHandle program;
};

#endif // HELLO_TRIANGLE_DRAWER_H
