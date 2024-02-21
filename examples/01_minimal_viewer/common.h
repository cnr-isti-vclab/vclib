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

#ifndef COMMON_H
#define COMMON_H

#include <vclib/algorithms/update/color.h>
#include <vclib/algorithms/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes/tri_mesh.h>

#include <vclib/ext/bgfx/drawable_mesh.h>

vcl::bgf::DrawableMesh<vcl::TriMesh> getDrawableMesh()
{
    // load a mesh:
    vcl::TriMesh m = vcl::load<vcl::TriMesh>(VCLIB_ASSETS_PATH "/bimba.obj");
    vcl::updatePerVertexAndFaceNormals(m);

    // enable the vertex color of the mesh and set it to gray
    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);

    // create a MeshRenderSettings object, that allows to set the rendering
    // options of the mesh
    // default is what we want: color per vertex, smooth shading, no wireframe
    vcl::MeshRenderSettings settings(m);

    // create a DrawableMesh object from the mesh
    vcl::bgf::DrawableMesh<vcl::TriMesh> drawable(m);

    // set the settings to the drawable mesh
    drawable.setRenderSettings(settings);

    return drawable;
}


#endif // COMMON_H
