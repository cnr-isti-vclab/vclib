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

#ifndef VCL_QT_CUSTOM_MESH_VIEWER_RENDER_APP_H
#define VCL_QT_CUSTOM_MESH_VIEWER_RENDER_APP_H

 #include <vclib/qt/widget_manager.h>
 #include <vclib/render/canvas.h>
 #include <vclib/render/drawers/viewer_drawer.h>
 #include <vclib/render/render_app.h>

template<typename DerivedRenderApp>
class ViewerDrawerSelectQt : public vcl::ViewerDrawer<DerivedRenderApp>
{
    using Base = vcl::ViewerDrawer<DerivedRenderApp>;

    // a callback function called when an object is selected
    std::function<void(uint)> mOnObjectSelected = [](uint) {};

public:
    using Base::Base;

    void onMousePress(
        vcl::MouseButton::Enum   button,
        double              x,
        double              y,
        const vcl::KeyModifiers& modifiers) override
    {
        if (button == vcl::MouseButton::RIGHT) {
            this->readIdRequest(x, y, [&](uint id) {
                if (id == vcl::UINT_NULL)
                    return;
                
                std::cout << "selected ID: " << id << std::endl;
                if (mOnObjectSelected)
                    mOnObjectSelected(id);
            });
        }

        Base::onMousePress(button, x, y, modifiers);
    }

    // seeter fo the callback function called when an object is selected
    void setOnObjectSelected(const std::function<void(uint)>& f)
    {
        mOnObjectSelected = f;
    }
};

// definition of custom MeshViewerRenderApp for the qt MeshViewer
namespace vcl::qt {
 
     using MeshViewerRenderApp =
        vcl::RenderApp<vcl::qt::WidgetManager, vcl::Canvas, ViewerDrawerSelectQt>;

 } // namespace vcl::qt
 
#endif // VCL_QT_CUSTOM_MESH_VIEWER_RENDER_APP_H
