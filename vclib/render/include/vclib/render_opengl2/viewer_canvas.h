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

#include <vclib/render/interfaces/viewer_i.h>

#include <vclib/render_opengl2/canvas.h>

namespace vcl {

class ViewerCanvas : public vcl::Canvas, public ViewerI
{
public:
    ViewerCanvas(
        void* winId,
        uint  width     = 1024,
        uint  height    = 768,
        void* displayId = nullptr);

    ViewerCanvas(
        void*                                        winId,
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width     = 1024,
        uint                                         height    = 768,
        void*                                        displayId = nullptr);

    void init(uint width, uint height);

    void toggleAxisVisibility() override
    {
        // todo
    }

    void toggleTrackBallVisibility() override
    {
        // todo
    }

protected:
    void draw() override;

    // events
    void onResize(unsigned int width, unsigned int height) override;
};

} // namespace vcl

#endif // VCL_OPENGL2_RENDER_VIEWER_CANVAS_H
