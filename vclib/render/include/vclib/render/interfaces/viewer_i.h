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

#ifndef VCL_RENDER_INTERFACES_VIEWER_I_H
#define VCL_RENDER_INTERFACES_VIEWER_I_H

#include "event_manager_i.h"

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/viewer/desktop_trackball.h>

#include <memory>

namespace vcl {

class ViewerI :
        public DesktopTrackBall<float>,
        public virtual vcl::EventManagerI
{
protected:
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> mDrawList;

    using DTB = vcl::DesktopTrackBall<float>;

public:
    ViewerI(uint width = 1024, uint height = 768) : DTB(width, height) {}

    ~ViewerI() = default;

    const DrawableObjectVector& drawableObjectVector() const;

    void setDrawableObjectVector(
        const std::shared_ptr<DrawableObjectVector>& v);

    uint pushDrawableObject(const DrawableObjectI& obj);

    void fitScene();

    virtual void toggleAxisVisibility() = 0;

    virtual void toggleTrackBallVisibility() = 0;

protected:
    // events
    void onKeyPress(Key::Enum key) override;

    void onKeyRelease(Key::Enum key) override;

    void onMouseMove(double x, double y) override;

    void onMousePress(MouseButton::Enum button) override;

    void onMouseRelease(MouseButton::Enum button) override;

    void onMouseScroll(double dx, double dy) override;
};

} // namespace vcl

#endif // VCL_RENDER_INTERFACES_VIEWER_I_H
