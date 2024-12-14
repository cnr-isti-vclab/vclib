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

#ifndef PER_MESH_H
#define PER_MESH_H

#include <vclib/meshes.h>

void perMeshStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasBoundingBox<TriMesh>, "TriMesh does not satisfy the HasBoundingBox");
    static_assert(
        HasBoundingBox<const TriMesh>,
        "const TriMesh does not satisfy the HasBoundingBox");
    static_assert(
        HasBoundingBox<TriMesh&>,
        "TriMesh& does not satisfy the HasBoundingBox");
    static_assert(
        HasBoundingBox<const TriMesh&>,
        "const TriMesh& does not satisfy the HasBoundingBox");
    static_assert(
        HasBoundingBox<TriMesh&&>,
        "TriMesh&& does not satisfy the HasBoundingBox");

    static_assert(HasColor<TriMesh>, "TriMesh does not satisfy the HasColor");
    static_assert(
        HasColor<const TriMesh>, "const TriMesh does not satisfy the HasColor");
    static_assert(HasColor<TriMesh&>, "TriMesh& does not satisfy the HasColor");
    static_assert(
        HasColor<const TriMesh&>,
        "const TriMesh& does not satisfy the HasColor");
    static_assert(
        HasColor<TriMesh&&>, "TriMesh&& does not satisfy the HasColor");

    static_assert(
        HasCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasCustomComponents");
    static_assert(
        HasCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasCustomComponents");
    static_assert(
        HasCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasCustomComponents");
    static_assert(
        HasCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasCustomComponents");
    static_assert(
        HasCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasCustomComponents");

    static_assert(HasMark<TriMesh>, "TriMesh does not satisfy the HasMark");
    static_assert(
        HasMark<const TriMesh>, "const TriMesh does not satisfy the HasMark");
    static_assert(HasMark<TriMesh&>, "TriMesh& does not satisfy the HasMark");
    static_assert(
        HasMark<const TriMesh&>, "const TriMesh& does not satisfy the HasMark");
    static_assert(HasMark<TriMesh&&>, "TriMesh&& does not satisfy the HasMark");

    static_assert(HasName<TriMesh>, "TriMesh does not satisfy the HasName");
    static_assert(
        HasName<const TriMesh>, "const TriMesh does not satisfy the HasName");
    static_assert(HasName<TriMesh&>, "TriMesh& does not satisfy the HasName");
    static_assert(
        HasName<const TriMesh&>, "const TriMesh& does not satisfy the HasName");
    static_assert(HasName<TriMesh&&>, "TriMesh&& does not satisfy the HasName");

    static_assert(
        HasTextureImages<TriMesh>,
        "TriMesh does not satisfy the HasTextureImages");
    static_assert(
        HasTextureImages<const TriMesh>,
        "const TriMesh does not satisfy the HasTextureImages");
    static_assert(
        HasTextureImages<TriMesh&>,
        "TriMesh& does not satisfy the HasTextureImages");
    static_assert(
        HasTextureImages<const TriMesh&>,
        "const TriMesh& does not satisfy the HasTextureImages");
    static_assert(
        HasTextureImages<TriMesh&&>,
        "TriMesh&& does not satisfy the HasTextureImages");

    static_assert(
        HasTexturePaths<TriMesh>,
        "TriMesh does not satisfy the HasTexturePaths");
    static_assert(
        HasTexturePaths<const TriMesh>,
        "const TriMesh does not satisfy the HasTexturePaths");
    static_assert(
        HasTexturePaths<TriMesh&>,
        "TriMesh& does not satisfy the HasTexturePaths");
    static_assert(
        HasTexturePaths<const TriMesh&>,
        "const TriMesh& does not satisfy the HasTexturePaths");
    static_assert(
        HasTexturePaths<TriMesh&&>,
        "TriMesh&& does not satisfy the HasTexturePaths");

    static_assert(
        HasTransformMatrix<TriMesh>,
        "TriMesh does not satisfy the HasTransformMatrix");
    static_assert(
        HasTransformMatrix<const TriMesh>,
        "const TriMesh does not satisfy the HasTransformMatrix");
    static_assert(
        HasTransformMatrix<TriMesh&>,
        "TriMesh& does not satisfy the HasTransformMatrix");
    static_assert(
        HasTransformMatrix<const TriMesh&>,
        "const TriMesh& does not satisfy the HasTransformMatrix");
    static_assert(
        HasTransformMatrix<TriMesh&&>,
        "TriMesh&& does not satisfy the HasTransformMatrix");
}

#endif // PER_MESH_H
