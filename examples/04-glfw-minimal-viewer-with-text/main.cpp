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

#include <vclib/ext/glfw/bgfx/minimal_viewer_window.h>

int main(int argc, char** argv)
{
    vcl::bglfwx::MinimalViewerWindow tw("Minimal Viewer GLFW");

    // load and set up a drawable mesh
    vcl::TriMesh                         m        = getMesh("greek_helmet.obj");
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh(m);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.enableText();

    tw.setTextFont(vcl::VclFont::DROID_SANS, 20);
    tw.appendStaticText(
        {5, 5}, "Vertices: " + std::to_string(m.vertexNumber()));
    tw.appendStaticText({5, 30}, "Faces: " + std::to_string(m.faceNumber()));

    tw.fitScene();

    tw.show();

    return 0;
}
