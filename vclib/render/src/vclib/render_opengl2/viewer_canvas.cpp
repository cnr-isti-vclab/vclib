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

#include <vclib/render_opengl2/viewer_canvas.h>

#include <vclib/math/min_max.h>
#include <vclib/render/interfaces/drawable_mesh_i.h>
#include <vclib/space/core/box.h>

namespace vcl {

ViewerCanvasOpenGL2::ViewerCanvasOpenGL2(
    void* winId,
    uint  width,
    uint  height,
    void* displayId) :
        vcl::CanvasOpenGL2(winId, width, height, displayId), ViewerI(width, height)
{
    init(width, height);
}

ViewerCanvasOpenGL2::ViewerCanvasOpenGL2(
    void*                                        winId,
    const std::shared_ptr<DrawableObjectVector>& v,
    uint                                         width,
    uint                                         height,
    void* displayId) : ViewerCanvasOpenGL2(winId, width, height, displayId)
{
    setDrawableObjectVector(v);
}

void ViewerCanvasOpenGL2::init(uint width, uint height)
{
    CanvasOpenGL2::init(width, height);
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

void ViewerCanvasOpenGL2::drawContent()
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

void ViewerCanvasOpenGL2::onResize(unsigned int width, unsigned int height)
{
    CanvasOpenGL2::onResize(width, height);
    DTB::resizeViewer(width, height);
    update();
}

void ViewerCanvasOpenGL2::onMouseDoubleClick(
    MouseButton::Enum button,
    double            x,
    double            y)
{
    // FIXME: code duplication for both OpenGL2 and BGFX
    if (mReadRequested)
        return;

    // get point
    const Point2d p(x, y);

    // get the homogeneous NDC flag
    const bool homogeneousNDC = true;

    // create the callback
    const auto    proj     = projectionMatrix();
    const auto    view     = viewMatrix();
    const Point4f vp       = {.0f, .0f, float(size().x()), float(size().y())};
    auto          callback = [=, this](std::vector<float> data) {
        mReadRequested = false;

        assert(data.size() == 1);
        const float depth = data[0];
        // if the depth is 1.0, the point is not in the scene
        if (depth == 1.0f) {
            return;
        }

        // unproject the point
        const Point3f p2d(p.x(), vp[3] - p.y(), depth);
        auto          unproj = unproject(
            p2d, Matrix44<ScalarType>(proj * view), vp, homogeneousNDC);

        this->focus(unproj);
        this->update();
    };

    mReadRequested = this->readDepth(Point2i(p.x(), p.y()), callback);
    if (mReadRequested)
        update();
}

} // namespace vcl
