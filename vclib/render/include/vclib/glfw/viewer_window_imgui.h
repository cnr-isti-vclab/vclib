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

#ifndef VCL_EXT_GLFW_VIEWER_WINDOW_IMGUI_H
#define VCL_EXT_GLFW_VIEWER_WINDOW_IMGUI_H

#include <vclib/glfw/viewer_window.h>

namespace vcl::glfw {

class ViewerWindowImgui : public ViewerWindow
{
public:
    ViewerWindowImgui(
        const std::shared_ptr<DrawableObjectVector>& v,
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        void*              parent      = nullptr);

    ViewerWindowImgui(
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        void*              parent      = nullptr);

    ViewerWindowImgui(void* parent);

    ~ViewerWindowImgui() override = default;

    void show() override;

protected:
    void draw() override;
};

} // namespace vcl::glfw

#endif // VCL_EXT_GLFW_VIEWER_WINDOW_H
