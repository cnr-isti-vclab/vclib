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

#ifndef VCL_OPENGL2_RENDER_VIEWER_CANVAS_H
#define VCL_OPENGL2_RENDER_VIEWER_CANVAS_H

#include <memory>

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/interfaces/event_manager_i.h>
#include <vclib/render/viewer/desktop_trackball.h>

#include <vclib_opengl2/render/canvas.h>

namespace vcl {

class ViewerCanvas : public vcl::Canvas, public vcl::DesktopTrackBall<float>
{
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> mDrawList;

protected:
    using DTB = vcl::DesktopTrackBall<float>;

public:
    ViewerCanvas(void* winId, uint width = 1024, uint height = 768);

    ViewerCanvas(
        void*                                 winId,
        std::shared_ptr<DrawableObjectVector> v,
        uint                                  width  = 1024,
        uint                                  height = 768);

    const DrawableObjectVector& drawableObjectVector() const;

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v);

    uint pushDrawableObject(const DrawableObjectI& obj);

    void fitScene();

protected:
    void draw() override;

    // events
    void onResize(unsigned int width, unsigned int height) override;

    void onKeyPress(Key::Enum key) override;

    void onKeyRelease(Key::Enum key) override;

    void onMouseMove(double x, double y) override;

    void onMousePress(MouseButton::Enum button) override;

    void onMouseRelease(MouseButton::Enum button) override;

    void onMouseScroll(double dx, double dy) override;

private:
    void initDrawableObject(DrawableObjectI& obj);
};

} // namespace vcl

#endif // VCL_OPENGL2_RENDER_VIEWER_CANVAS_H
