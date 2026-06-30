// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_DRAWERS_VIEWER_DRAWER_OPENGL2_H
#define VCL_OPENGL2_DRAWERS_VIEWER_DRAWER_OPENGL2_H

#include <vclib/render/drawers/abstract_viewer_drawer.h>
#include <vclib/render/read_buffer_types.h>

#include <memory>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

namespace vcl {

template<typename DerivedRenderApp>
class ViewerDrawerOpenGL2 : public AbstractViewerDrawer<DerivedRenderApp>
{
    using ParentViewer = AbstractViewerDrawer<DerivedRenderApp>;

public:
    ViewerDrawerOpenGL2(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
    }

    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);

        float lightAmbient[] = {.2f, .2f, .2f, 1.0f};
        float lightDiffuse[] = {.5f, .5f, .5f, 1.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

        if (ParentViewer::mDrawList) {
            for (auto& obj : *(ParentViewer::mDrawList)) {
                obj->init();
            }
        }
    }

    void onDraw(uint viewId) override { onDrawContent(viewId); }

    void onDrawContent(uint) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto         tmp = ParentViewer::light().direction();
        vcl::Point4f lPos(tmp.x(), tmp.y(), tmp.z(), 0.0f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMultMatrixf(ParentViewer::projectionMatrix().data());
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLightfv(GL_LIGHT0, GL_POSITION, lPos.data());
        glMultMatrixf(ParentViewer::viewMatrix().data());

        for (auto& obj : *(ParentViewer::mDrawList))
            obj->draw();
    }

    // events
    bool onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        bool block = ParentViewer::onMouseDoubleClick(button, x, y, modifiers);
        if (!block && button == MouseButton::LEFT) {
            ParentViewer::readDepthRequest(x, y);
        }
        return block;
    }

    // drawable trackball
    bool isTrackBallVisible() const override { return false; }

    void toggleTrackBallVisibility() override {}

    void setShortcutToggleTrackballCallback(
        std::function<void(void)> callback) override
    {
    }
};

} // namespace vcl

#endif // VCL_OPENGL2_DRAWERS_VIEWER_DRAWER_OPENGL2_H
