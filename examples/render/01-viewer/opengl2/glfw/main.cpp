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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "common.h"

#include <vclib/glfw/viewer_window.h>

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");

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
