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

#ifndef VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H
#define VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H

#include "event_drawer.h"

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/drawers/event_drawer.h>
#include <vclib/render/read_buffer_types.h>
#include <vclib/render/viewer/desktop_trackball.h>

#include <memory>

namespace vcl {

/**
 * @brief The AbstractViewerDrawer class is a base class for all viewer drawers
 * implementations.
 *
 * It provides all the common functionalities of a viewer, but not the core
 * rendering functionalities. It is meant to be subclassed by a concrete viewer
 * drawer implementation.
 */
template<typename DerivedRenderApp>
class AbstractViewerDrawer :
        public DesktopTrackBall<float>,
        public EventDrawer<DerivedRenderApp>
{
    bool mReadRequested = false;

protected:
    // the list of drawable objects
    // it could be owned by the viewer, or it could be shared with other
    // objects (e.g. the window that contains the viewer along with other
    // widgets)
    std::shared_ptr<DrawableObjectVector> mDrawList =
        std::make_shared<DrawableObjectVector>();

    using DTB = vcl::DesktopTrackBall<float>;

public:
    AbstractViewerDrawer(uint width = 1024, uint height = 768) :
            DTB(width, height)
    {
    }

    ~AbstractViewerDrawer() = default;

    const DrawableObjectVector& drawableObjectVector() const
    {
        return *mDrawList;
    }

    void setDrawableObjectVector(const std::shared_ptr<DrawableObjectVector>& v)
    {
        mDrawList = v;

        for (auto obj : *mDrawList) {
            obj->init();
        }
        fitScene();
    }

    uint pushDrawableObject(const DrawableObject& obj)
    {
        mDrawList->pushBack(obj);
        mDrawList->back()->init();
        return mDrawList->size() - 1;
    }

    uint pushDrawableObject(DrawableObject&& obj)
    {
        mDrawList->pushBack(std::move(obj));
        mDrawList->back()->init();
        return mDrawList->size() - 1;
    }

    void fitScene()
    {
        Point3f sceneCenter;
        float   sceneRadius = 1;

        Box3d bb = mDrawList->boundingBox();

        if (!bb.isNull()) {
            sceneCenter = bb.center().cast<float>();
            sceneRadius = bb.diagonal();
        }

        DTB::setTrackBall(sceneCenter, sceneRadius);
    }

    virtual void toggleAxisVisibility() = 0;

    virtual void toggleTrackBallVisibility() = 0;

    // events
    void onInit(uint) override
    {
        DerivedRenderApp::DRW::setCanvasDefaultClearColor(
            derived(), Color::White);
    }

    void onResize(unsigned int width, unsigned int height) override
    {
        DTB::resizeViewer(width, height);
    }

    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        DTB::setKeyModifiers(modifiers);

        switch (key) {
        case Key::C:
            std::cout << "(" << DTB::camera().eye() << ") "
                      << "(" << DTB::camera().center() << ") "
                      << "(" << DTB::camera().up() << ")\n";
            std::cout << std::flush;
            break;

        case Key::A: toggleAxisVisibility(); break;

        case Key::S:
            if (modifiers[KeyModifier::CONTROL])
                DerivedRenderApp::DRW::screenshot(
                    derived(), "viewer_screenshot.png");
            break;

        case Key::T: toggleTrackBallVisibility(); break;

        default: break;
        }

        DTB::keyPress(key);
    }

    void onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) override
    {
        DTB::setKeyModifiers(modifiers);
        DTB::keyRelease(key);
    }

    void onMouseMove(double x, double y, const KeyModifiers& modifiers) override
    {
        DTB::setKeyModifiers(modifiers);
        DTB::moveMouse(x, y);
    }

    void onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        DTB::setKeyModifiers(modifiers);
        DTB::moveMouse(x, y);
        DTB::pressMouse(button);
    }

    void onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        DTB::setKeyModifiers(modifiers);
        DTB::moveMouse(x, y);
        DTB::releaseMouse(button);
    }

    void onMouseScroll(double dx, double dy, const KeyModifiers& modifiers)
        override
    {
        DTB::setKeyModifiers(modifiers);
        DTB::scroll(dx, dy);
    }

protected:
    void readRequest(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers,
        bool                homogeneousNDC = true)
    {
        using ReadData  = ReadBufferTypes::ReadData;
        using FloatData = ReadBufferTypes::FloatData;

        if (mReadRequested)
            return;

        // get point
        const Point2d p(x, y);

        // create the callback
        const auto proj = DTB::projectionMatrix();
        const auto view = DTB::viewMatrix();
        // viewport
        auto size = DerivedRenderApp::DRW::canvasSize(derived());

        const Point4f vp = {.0f, .0f, float(size.x()), float(size.y())};

        auto callback = [=, this](const ReadData& dt) {
            mReadRequested = false;

            const auto& data = std::get<FloatData>(dt);
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
            derived()->update();
        };

        mReadRequested = DerivedRenderApp::DRW::readDepth(
            derived(), Point2i(p.x(), p.y()), callback);
        if (mReadRequested)
            derived()->update();
    }

private:
    auto* derived() { return static_cast<DerivedRenderApp*>(this); }

    const auto* derived() const
    {
        return static_cast<const DerivedRenderApp*>(this);
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H
