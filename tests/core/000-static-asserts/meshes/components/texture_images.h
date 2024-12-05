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

#ifndef COMP_TEXTURE_IMAGES_H
#define COMP_TEXTURE_IMAGES_H

#include <vclib/meshes.h>

void textureImagesComponentStaticAsserts()
{
    using namespace vcl;

    // test only the texture images component
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
}

#endif // COMP_TEXTURE_IMAGES_H
