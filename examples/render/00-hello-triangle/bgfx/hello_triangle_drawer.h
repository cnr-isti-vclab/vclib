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

#ifndef HELLO_TRIANGLE_DRAWER_H
#define HELLO_TRIANGLE_DRAWER_H

#include "common.h"

template<typename DerivedDrawer>
class HelloTriangleDrawer
{
public:
    HelloTriangleDrawer(uint width = 1024, uint height = 768) {}

    ~HelloTriangleDrawer()
    {
        if (bgfx::isValid(vbh))
            bgfx::destroy(vbh);
        if (bgfx::isValid(program))
            bgfx::destroy(program);
    }

    void onInit(vcl::uint viewId)
    {
        setUpBGFX(viewId, vbh, program);
    }

    void onResize(vcl::uint width, vcl::uint height)
    {
        std::cout << "Resize: " << width << "; " << height
                  << ". Nothing to do\n";
    }

    void onDrawContent(vcl::uint viewId)
    {
        drawOnView(viewId, vbh, program);
    }

    void onDraw(vcl::uint viewId)
    {
        onDrawContent(viewId);
    }

private:
    bgfx::VertexBufferHandle vbh;

    bgfx::ProgramHandle program;
};

#endif // HELLO_TRIANGLE_DRAWER_H
