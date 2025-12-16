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

#ifndef COMP_MATERIALS_H
#define COMP_MATERIALS_H

#include <vclib/meshes.h>

void materialsComponentStaticAsserts()
{
    using namespace vcl;

    class NoMaterialsMesh :
            public vcl::Mesh<
                mesh::VertexContainer<trimesh::Vertex<double, false>>,
                mesh::FaceContainer<trimesh::Face<double, false>>>
    {
    public:
        using ScalarType = double;
    };

    // plain components
    static_assert(
        comp::HasMaterials<mesh::Materials>,
        "mesh::Materials does not satisfy the HasMaterials concept");
    static_assert(
        comp::HasMaterials<const mesh::Materials>,
        "const mesh::Materials does not satisfy the HasMaterials "
        "concept");
    static_assert(
        comp::HasMaterials<mesh::Materials&>,
        "mesh::Materials& does not satisfy the HasMaterials concept");
    static_assert(
        comp::HasMaterials<const mesh::Materials&>,
        "const mesh::Materials& does not satisfy the HasMaterials "
        "concept");
    static_assert(
        comp::HasMaterials<mesh::Materials&&>,
        "mesh::Materials&& does not satisfy the HasMaterials concept");

    // trimesh
    static_assert(
        comp::HasMaterials<TriMesh>,
        "TriMesh does not satisfy the HasMaterials concept");
    static_assert(
        comp::HasMaterials<const TriMesh>,
        "const TriMesh does not satisfy the HasMaterials concept");
    static_assert(
        comp::HasMaterials<TriMesh&>,
        "TriMesh& does not satisfy the HasMaterials concept");
    static_assert(
        comp::HasMaterials<const TriMesh&>,
        "const TriMesh& does not satisfy the HasMaterials concept");
    static_assert(
        comp::HasMaterials<TriMesh&&>,
        "TriMesh&& does not satisfy the HasMaterials concept");

    // no materials mesh
    static_assert(
        !comp::HasMaterials<NoMaterialsMesh>,
        "NoMaterialsMesh does satisfy the HasMaterials concept");
    static_assert(
        !comp::HasMaterials<const NoMaterialsMesh>,
        "const NoMaterialsMesh does satisfy the HasMaterials concept");
    static_assert(
        !comp::HasMaterials<NoMaterialsMesh&>,
        "NoMaterialsMesh& does satisfy the HasMaterials concept");
    static_assert(
        !comp::HasMaterials<const NoMaterialsMesh&>,
        "const NoMaterialsMesh& does satisfy the HasMaterials concept");
    static_assert(
        !comp::HasMaterials<NoMaterialsMesh&&>,
        "NoMaterialsMesh&& does satisfy the HasMaterials concept");
}

#endif // COMP_MATERIALS_H
