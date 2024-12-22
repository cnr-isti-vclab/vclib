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

#ifndef VCL_OPENGL2_VIEWER_DRAWER_H
#define VCL_OPENGL2_VIEWER_DRAWER_H

#include <vclib/render/viewer/abstract_viewer.h>

#include <memory>

namespace vcl {

class ViewerDrawerOpenGL2 : public AbstractViewer
{
    bool mReadRequested = false;

public:
    ViewerDrawerOpenGL2(
        uint  width     = 1024,
        uint  height    = 768);

    ViewerDrawerOpenGL2(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width     = 1024,
        uint                                         height    = 768);

    void onInit() override;

    void toggleAxisVisibility() override
    {
        // todo
    }

    void toggleTrackBallVisibility() override
    {
        // todo
    }

    void onDrawContent(uint) override;

    // events
    void onResize(unsigned int width, unsigned int height) override;

   // void onMouseDoubleClick(
   //     MouseButton::Enum   button,
   //     double              x,
   //     double              y,
   //     const KeyModifiers& modifiers) override;

};

} // namespace vcl

#endif // VCL_OPENGL2_VIEWER_DRAWER_H
