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

#include <vclib/glfw/window_manager.h>

#include <vclib/imgui/mesh_viewer_imgui_drawer.h>
#include <vclib/imgui/imgui_drawer.h>

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>

#include "get_drawable_mesh.h"

int main(void)
{
    using RA = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::MeshViewerDrawerImgui>;
    
    RA tw("Selection", 1024, 768);

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();
    tw.setDrawableObjectVector(vec);

    vcl::DrawableMesh<vcl::TriMesh> msh = getDrawableMesh<vcl::TriMesh>();

    tw.pushDrawableObject(std::move(msh));

    tw.fitScene();

    tw.show();

    return 0;
}
