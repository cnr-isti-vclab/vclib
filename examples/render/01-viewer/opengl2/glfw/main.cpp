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

#include "common.h"

#include <vclib/opengl2/drawers/viewer_drawer.h>
#include <vclib/render/canvas.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/renderer.h>

int main(int argc, char** argv)
{
    using ViewerWindow =
        vcl::Renderer<vcl::glfw::WindowManager, vcl::Canvas, vcl::ViewerDrawerOpenGL2>;

    ViewerWindow tw("Viewer GLFW");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh();

    auto mrs = drawable.renderSettings();
    mrs.setSurfaceColorPerMesh();
    mrs.setSurfaceShadingFlat();
    drawable.setRenderSettings(mrs);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    // vcl::glfw::ViewerWindow tw2("Viewer GLFW");

    // // load and set up a drawable mesh
    // vcl::DrawableMesh<vcl::TriMesh> drawable2 =
    //     getDrawableMesh("greek_helmet.obj");

    // // add the drawable mesh to the scene
    // // the viewer will own **a copy** of the drawable mesh
    // tw2.pushDrawableObject(drawable2);

    // tw2.fitScene();

    // tw2.show();

    return 0;
}
