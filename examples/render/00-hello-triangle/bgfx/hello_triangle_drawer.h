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

#ifndef HELLO_TRIANGLE_DRAWER_H
#define HELLO_TRIANGLE_DRAWER_H

#include "common.h"

#include <vclib/render/drawers/plain_drawer.h>

template<typename DerivedDrawer>
class HelloTriangleDrawer : public vcl::PlainDrawer<DerivedDrawer>
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
