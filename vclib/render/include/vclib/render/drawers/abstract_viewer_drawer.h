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

#ifndef VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H
#define VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/drawers/event_drawer.h>
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
template<typename DerivedRenderer>
class AbstractViewerDrawer :
        public DesktopTrackBall<float>
{
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

    void setDrawableObjectVector(
        const std::shared_ptr<DrawableObjectVector>& v)
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
    void onResize(unsigned int width, unsigned int height)
    {
        DTB::resizeViewer(width, height);
    }

    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers)
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

        case Key::T: toggleTrackBallVisibility(); break;

        default: break;
        }

        DTB::keyPress(key);
    }

    void onKeyRelease(Key::Enum key, const KeyModifiers& modifiers)
    {
        DTB::setKeyModifiers(modifiers);
        DTB::keyRelease(key);
    }

    void onMouseMove(double x, double y, const KeyModifiers& modifiers)
    {
        DTB::setKeyModifiers(modifiers);
        DTB::moveMouse(x, y);
    }

    void onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        DTB::setKeyModifiers(modifiers);
        DTB::moveMouse(x, y);
        DTB::pressMouse(button);
    }

    void onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        DTB::setKeyModifiers(modifiers);
        DTB::moveMouse(x, y);
        DTB::releaseMouse(button);
    }

    void onMouseScroll(double dx, double dy, const KeyModifiers& modifiers)
    {
        DTB::setKeyModifiers(modifiers);
        DTB::scroll(dx, dy);
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H
