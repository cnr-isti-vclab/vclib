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

#ifndef VCL_RENDER_VIEWER_ABSTRACT_VIEWER_H
#define VCL_RENDER_VIEWER_ABSTRACT_VIEWER_H

#include "desktop_trackball.h"

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/drawers/event_drawer.h>

#include <memory>

namespace vcl {

/**
 * @brief The AbstractViewer class is a base class for all viewer
 * implementations.
 *
 * It provides all the common functionalities of a viewer, but not the core
 * rendering functionalities. It is meant to be subclassed by a concrete viewer
 * implementation.
 */
class AbstractViewer :
        public EventDrawer,
        public DesktopTrackBall<float> //, public virtual vcl::EventManagerI
{
protected:
    // the list of drawable objects
    // it could be owned by the viewer, or it could be shared with other
    // objects (e.g. the window that contains the viewer along with other
    // widgets)
    std::shared_ptr<DrawableObjectVector> mDrawList;

    using DTB = vcl::DesktopTrackBall<float>;

public:
    AbstractViewer(uint width = 1024, uint height = 768) : DTB(width, height) {}

    ~AbstractViewer() = default;

    const DrawableObjectVector& drawableObjectVector() const;

    void setDrawableObjectVector(
        const std::shared_ptr<DrawableObjectVector>& v);

    uint pushDrawableObject(const DrawableObject& obj);

    void fitScene();

    virtual void toggleAxisVisibility() = 0;

    virtual void toggleTrackBallVisibility() = 0;

protected:
    // events
    void onResize(unsigned int width, unsigned int height) override;

    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override;

    void onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) override;

    void onMouseMove(double x, double y, const KeyModifiers& modifiers)
        override;

    void onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override;

    void onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override;

    void onMouseScroll(double dx, double dy, const KeyModifiers& modifiers)
        override;
};

} // namespace vcl

#endif // VCL_RENDER_VIEWER_ABSTRACT_VIEWER_H
