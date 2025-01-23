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

#ifndef COMMON_H
#define COMMON_H

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes/tri_mesh.h>

#include <vclib/render/drawable/drawable_mesh.h>

template<vcl::MeshConcept MeshType>
inline vcl::DrawableMesh<MeshType> getDrawableMesh(
    const std::string& filename = "bimba.obj")
{
    // load a mesh:
    MeshType m = vcl::load<MeshType>(VCLIB_EXAMPLE_MESHES_PATH "/" + filename);
    vcl::updatePerVertexAndFaceNormals(m);

    // enable the vertex color of the mesh and set it to gray
    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);
    m.color() = vcl::Color::Blue;

    // create a MeshRenderSettings object, that allows to set the rendering
    // options of the mesh
    // default is what we want: color per vertex, smooth shading, no wireframe
    vcl::MeshRenderSettings settings(m);
    settings.setWireframeVisibility(true);
    settings.setSurfaceVisibility(false);
    settings.setWireframeWidth(5);
    // settings.setWireframeColorPerMesh();
    // settings.setWireframeColorUserDefined();
    // settings.setWireframeUserColor(0, 1, 1, 1);

    // create a DrawableMesh object from the mesh
    vcl::DrawableMesh<MeshType> drawable(m);

    // set the settings to the drawable mesh
    drawable.setRenderSettings(settings);

    return drawable;
}

#endif // COMMON_H
