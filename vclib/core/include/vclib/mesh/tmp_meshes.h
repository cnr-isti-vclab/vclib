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

#ifndef VCL_MESH_TMP_MESHES_H
#define VCL_MESH_TMP_MESHES_H

#include <vclib/mesh/mesh.h>

namespace vcl::detail {

class TMPSimpleTriMesh;
class TMPSimplePolyMesh;

namespace tmpMesh {

class TriVertex :
        public Vertex<TMPSimpleTriMesh, vert::BitFlags, vert::Coordinate3d>
{
};

class PolyVertex :
        public Vertex<TMPSimplePolyMesh, vert::BitFlags, vert::Coordinate3d>
{
};

class TriFace;
class PolyFace;

class TriFace :
        public Face<
            TMPSimpleTriMesh,
            face::TriangleBitFlags,
            face::TriangleVertexPtrs<TriVertex, TriFace>>
{
};

class PolyFace :
        public Face<
            TMPSimplePolyMesh,
            face::PolygonBitFlags,
            face::PolygonVertexPtrs<PolyVertex, PolyFace>>
{
};

} // namespace tmpMesh

class TMPSimpleTriMesh :
        public Mesh<
            mesh::VertexContainer<tmpMesh::TriVertex>,
            mesh::FaceContainer<tmpMesh::TriFace>>
{
};

class TMPSimplePolyMesh :
        public Mesh<
            mesh::VertexContainer<tmpMesh::PolyVertex>,
            mesh::FaceContainer<tmpMesh::PolyFace>>
{
};

} // namespace vcl::detail

#endif // VCL_MESH_TMP_MESHES_H
