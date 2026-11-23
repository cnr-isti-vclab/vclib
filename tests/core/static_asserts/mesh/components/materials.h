// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_MATERIALS_H
#define COMP_MATERIALS_H

#include <vclib/meshes.h>

void materialsComponentStaticAsserts()
{
    using namespace vcl;

    class NoMaterialsMesh :
            public vcl::Mesh<
                mesh::VertexContainer<
                    trimesh::VertexComponents<float, uint>,
                    uint>,
                mesh::FaceContainer<trimesh::FaceComponents<float, uint>, uint>>
    {
    public:
        using ScalarType = double;
    };

    // plain components
    static_assert(
        HasMaterials<mesh::Materials>,
        "mesh::Materials does not satisfy the HasMaterials concept");
    static_assert(
        HasMaterials<const mesh::Materials>,
        "const mesh::Materials does not satisfy the HasMaterials "
        "concept");
    static_assert(
        HasMaterials<mesh::Materials&>,
        "mesh::Materials& does not satisfy the HasMaterials concept");
    static_assert(
        HasMaterials<const mesh::Materials&>,
        "const mesh::Materials& does not satisfy the HasMaterials "
        "concept");
    static_assert(
        HasMaterials<mesh::Materials&&>,
        "mesh::Materials&& does not satisfy the HasMaterials concept");

    // trimesh
    static_assert(
        HasMaterials<TriMesh>,
        "TriMesh does not satisfy the HasMaterials concept");
    static_assert(
        HasMaterials<const TriMesh>,
        "const TriMesh does not satisfy the HasMaterials concept");
    static_assert(
        HasMaterials<TriMesh&>,
        "TriMesh& does not satisfy the HasMaterials concept");
    static_assert(
        HasMaterials<const TriMesh&>,
        "const TriMesh& does not satisfy the HasMaterials concept");
    static_assert(
        HasMaterials<TriMesh&&>,
        "TriMesh&& does not satisfy the HasMaterials concept");

    // no materials mesh
    static_assert(
        !HasMaterials<NoMaterialsMesh>,
        "NoMaterialsMesh does satisfy the HasMaterials concept");
    static_assert(
        !HasMaterials<const NoMaterialsMesh>,
        "const NoMaterialsMesh does satisfy the HasMaterials concept");
    static_assert(
        !HasMaterials<NoMaterialsMesh&>,
        "NoMaterialsMesh& does satisfy the HasMaterials concept");
    static_assert(
        !HasMaterials<const NoMaterialsMesh&>,
        "const NoMaterialsMesh& does satisfy the HasMaterials concept");
    static_assert(
        !HasMaterials<NoMaterialsMesh&&>,
        "NoMaterialsMesh&& does satisfy the HasMaterials concept");
}

#endif // COMP_MATERIALS_H
