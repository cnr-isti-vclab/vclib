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

#ifndef VCL_MESHES_EDGE_MESH_H
#define VCL_MESHES_EDGE_MESH_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename ScalarType, bool INDEXED>
class EdgeMeshT;

} // namespace vcl

namespace vcl::edgemesh {

template<typename Scalar, bool INDEXED>
class Vertex;

template<typename Scalar, bool INDEXED>
class Edge;

template<typename Scalar, bool I>
class Vertex :
        public vcl::Vertex<
            EdgeMeshT<Scalar, I>,
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar, I>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar, I>>,
            vert::OptionalAdjacentEdges<I, Edge<Scalar, I>, Vertex<Scalar, I>>,
            vert::OptionalAdjacentVertices<Vertex<Scalar, I>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar, I>>,
            vert::OptionalMark<Vertex<Scalar, I>>,
            vert::CustomComponents<Vertex<Scalar, I>>>
{
};

template<typename Scalar, bool I>
class Edge :
        public vcl::Edge<
            EdgeMeshT<Scalar, I>,
            edge::BitFlags,
            edge::VertexReferences<I, Vertex<Scalar, I>, Edge<Scalar, I>>,
            edge::OptionalColor<Edge<Scalar, I>>,
            edge::OptionalQuality<Scalar, Edge<Scalar, I>>,
            edge::OptionalAdjacentEdges<I, Edge<Scalar, I>>,
            edge::OptionalMark<Edge<Scalar, I>>,
            edge::CustomComponents<Edge<Scalar, I>>>
{
};

} // namespace vcl::edgemesh

namespace vcl {

template<typename Scalar, bool INDEXED>
class EdgeMeshT :
        public vcl::Mesh<
            mesh::VertexContainer<edgemesh::Vertex<Scalar, INDEXED>>,
            mesh::EdgeContainer<edgemesh::Edge<Scalar, INDEXED>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Mark,
            mesh::Name,
            mesh::TexturePaths,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    using ScalarType = Scalar;
};

using EdgeMeshf        = EdgeMeshT<float, false>;
using EdgeMesh         = EdgeMeshT<double, false>;
using EdgeMeshIndexedf = EdgeMeshT<float, true>;
using EdgeMeshIndexed  = EdgeMeshT<double, true>;

} // namespace vcl

#endif // VCL_MESHES_EDGE_MESH_H
