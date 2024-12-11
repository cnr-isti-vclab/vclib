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

#ifndef COMP_TEXTURES_H
#define COMP_TEXTURES_H

#include <vclib/meshes.h>

void texturesComponentStaticAsserts()
{
    using namespace vcl;

    class NoTextureImagesMesh :
            public vcl::Mesh<
                mesh::VertexContainer<trimesh::Vertex<double, false>>,
                mesh::FaceContainer<trimesh::Face<double, false>>,
                mesh::TexturePaths>
    {
    public:
        using ScalarType = double;
    };

    // plain components
    static_assert(
        comp::HasTextureImages<mesh::TextureImages>,
        "mesh::TextureImages does not satisfy the HasTextureImages concept");
    static_assert(
        comp::HasTextureImages<const mesh::TextureImages>,
        "const mesh::TextureImages does not satisfy the HasTextureImages "
        "concept");
    static_assert(
        comp::HasTextureImages<mesh::TextureImages&>,
        "mesh::TextureImages& does not satisfy the HasTextureImages concept");
    static_assert(
        comp::HasTextureImages<const mesh::TextureImages&>,
        "const mesh::TextureImages& does not satisfy the HasTextureImages "
        "concept");
    static_assert(
        comp::HasTextureImages<mesh::TextureImages&&>,
        "mesh::TextureImages&& does not satisfy the HasTextureImages concept");

    static_assert(
        comp::HasTexturePaths<mesh::TexturePaths>,
        "mesh::TexturePaths does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<const mesh::TexturePaths>,
        "const mesh::TexturePaths does not satisfy the HasTexturePaths "
        "concept");
    static_assert(
        comp::HasTexturePaths<mesh::TexturePaths&>,
        "mesh::TexturePaths& does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<const mesh::TexturePaths&>,
        "const mesh::TexturePaths& does not satisfy the HasTexturePaths "
        "concept");
    static_assert(
        comp::HasTexturePaths<mesh::TexturePaths&&>,
        "mesh::TexturePaths&& does not satisfy the HasTexturePaths concept");

    // trimesh
    static_assert(
        comp::HasTextureImages<TriMesh>,
        "TriMesh does not satisfy the HasTextureImages concept");
    static_assert(
        comp::HasTextureImages<const TriMesh>,
        "const TriMesh does not satisfy the HasTextureImages concept");
    static_assert(
        comp::HasTextureImages<TriMesh&>,
        "TriMesh& does not satisfy the HasTextureImages concept");
    static_assert(
        comp::HasTextureImages<const TriMesh&>,
        "const TriMesh& does not satisfy the HasTextureImages concept");
    static_assert(
        comp::HasTextureImages<TriMesh&&>,
        "TriMesh&& does not satisfy the HasTextureImages concept");

    static_assert(
        comp::HasTexturePaths<TriMesh>,
        "TriMesh does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<const TriMesh>,
        "const TriMesh does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<TriMesh&>,
        "TriMesh& does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<const TriMesh&>,
        "const TriMesh& does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<TriMesh&&>,
        "TriMesh&& does not satisfy the HasTexturePaths concept");

    // no texture images mesh
    static_assert(
        !comp::HasTextureImages<NoTextureImagesMesh>,
        "NoTextureImagesMesh does satisfy the HasTextureImages concept");
    static_assert(
        !comp::HasTextureImages<const NoTextureImagesMesh>,
        "const NoTextureImagesMesh does satisfy the HasTextureImages concept");
    static_assert(
        !comp::HasTextureImages<NoTextureImagesMesh&>,
        "NoTextureImagesMesh& does satisfy the HasTextureImages concept");
    static_assert(
        !comp::HasTextureImages<const NoTextureImagesMesh&>,
        "const NoTextureImagesMesh& does satisfy the HasTextureImages concept");
    static_assert(
        !comp::HasTextureImages<NoTextureImagesMesh&&>,
        "NoTextureImagesMesh&& does satisfy the HasTextureImages concept");

    static_assert(
        comp::HasTexturePaths<NoTextureImagesMesh>,
        "NoTextureImagesMesh does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<const NoTextureImagesMesh>,
        "const NoTextureImagesMesh does not satisfy the HasTexturePaths "
        "concept");
    static_assert(
        comp::HasTexturePaths<NoTextureImagesMesh&>,
        "NoTextureImagesMesh& does not satisfy the HasTexturePaths concept");
    static_assert(
        comp::HasTexturePaths<const NoTextureImagesMesh&>,
        "const NoTextureImagesMesh& does not satisfy the HasTexturePaths "
        "concept");
    static_assert(
        comp::HasTexturePaths<NoTextureImagesMesh&&>,
        "NoTextureImagesMesh&& does not satisfy the HasTexturePaths concept");
}

#endif // COMP_TEXTURES_H
