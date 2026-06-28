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

    ~HelloTriangleDrawer() {}

    void onResize(vcl::uint width, vcl::uint height) override
    {
        std::cout << "Resize: " << width << "; " << height
                  << ". Nothing to do\n";
    }

    void onDrawContent(vcl::uint) override
    {
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor4ubv((GLubyte*) &(vertices[0].abgr));
        glVertex2fv((GLfloat*) &(vertices[0].pos));
        glColor4ubv((GLubyte*) &(vertices[1].abgr));
        glVertex2fv((GLfloat*) &(vertices[1].pos));
        glColor4ubv((GLubyte*) &(vertices[2].abgr));
        glVertex2fv((GLfloat*) &(vertices[2].pos));
        glEnd();
    }
};

#endif // HELLO_TRIANGLE_DRAWER_H
