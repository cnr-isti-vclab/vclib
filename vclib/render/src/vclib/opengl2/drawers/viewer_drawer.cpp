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

#include <vclib/opengl2/drawers/viewer_drawer.h>

#include <vclib/math/min_max.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/space/core/box.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

namespace vcl {

ViewerDrawerOpenGL2::ViewerDrawerOpenGL2(
    uint  width,
    uint  height) :
        AbstractViewer(width, height)
{
    init(width, height);
}

ViewerDrawerOpenGL2::ViewerDrawerOpenGL2(
    const std::shared_ptr<DrawableObjectVector>& v,
    uint                                         width,
    uint height) : ViewerDrawerOpenGL2(width, height)
{
    setDrawableObjectVector(v);
}

void ViewerDrawerOpenGL2::init(uint width, uint height)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    float lightAmbient[] = {.2f, .2f, .2f, 1.0f};
    float lightDiffuse[] = {.5f, .5f, .5f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    if (mDrawList) {
        for (auto& obj : *mDrawList) {
            obj->init();
        }
    }
}

void ViewerDrawerOpenGL2::onDrawContent(uint)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto         tmp = light().direction();
    vcl::Point4f lPos(tmp.x(), tmp.y(), tmp.z(), 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(projectionMatrix().data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lPos.data());
    glMultMatrixf(viewMatrix().data());

    for (auto& obj : *mDrawList)
        obj->draw();
}

void ViewerDrawerOpenGL2::onResize(unsigned int width, unsigned int height)
{
    DTB::resizeViewer(width, height);
    // update(); TODO
}

// void ViewerCanvasOpenGL2::onMouseDoubleClick(
//     MouseButton::Enum   button,
//     double              x,
//     double              y,
//     const KeyModifiers& modifiers)
// {
//     // FIXME: code duplication for both OpenGL2 and BGFX
//     if (mReadRequested)
//         return;

//     // get point
//     const Point2d p(x, y);

//     // get the homogeneous NDC flag
//     const bool homogeneousNDC = true;

//     // create the callback
//     const auto    proj     = projectionMatrix();
//     const auto    view     = viewMatrix();
//     const Point4f vp       = {.0f, .0f, float(size().x()), float(size().y())};
//     auto          callback = [=, this](std::vector<float> data) {
//         mReadRequested = false;

//         assert(data.size() == 1);
//         const float depth = data[0];
//         // if the depth is 1.0, the point is not in the scene
//         if (depth == 1.0f) {
//             return;
//         }

//         // unproject the point
//         const Point3f p2d(p.x(), vp[3] - p.y(), depth);
//         auto          unproj = unproject(
//             p2d, Matrix44<ScalarType>(proj * view), vp, homogeneousNDC);

//         this->focus(unproj);
//         this->update();
//     };

//     mReadRequested = this->readDepth(Point2i(p.x(), p.y()), callback);
//     if (mReadRequested)
//         update();
// }

} // namespace vcl
