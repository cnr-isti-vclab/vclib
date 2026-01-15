/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCLIB_RENDER_EXAMPLES_COMMON_GET_DRAWABLE_MESH_H
#define VCLIB_RENDER_EXAMPLES_COMMON_GET_DRAWABLE_MESH_H

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <vclib/render/drawable/drawable_mesh.h>

template<vcl::MeshConcept MeshType>
inline vcl::DrawableMesh<MeshType> getDrawableMesh(
    std::string filename              = "bimba.obj",
    bool        fromVCLibExamplesPath = true)
{
    if (fromVCLibExamplesPath) {
        filename = VCLIB_EXAMPLE_MESHES_PATH "/" + filename;
    }

    MeshType m = vcl::loadMesh<MeshType>(filename);
    vcl::updatePerVertexAndFaceNormals(m);

    // create a DrawableMesh object from the mesh
    vcl::DrawableMesh<MeshType> drawable(m);

    return drawable;
}

#endif // VCLIB_RENDER_EXAMPLES_COMMON_GET_DRAWABLE_MESH_H
