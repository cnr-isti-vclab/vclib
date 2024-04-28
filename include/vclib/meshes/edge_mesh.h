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

template<typename ScalarType>
class EdgeMeshT;

} // namespace vcl

namespace vcl::edgemesh {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Edge;

template<typename Scalar>
class Vertex :
        public vcl::Vertex<
            EdgeMeshT<Scalar>,
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vert::OptionalAdjacentEdges<Edge<Scalar>, Vertex<Scalar>>,
            vert::OptionalAdjacentVertices<Vertex<Scalar>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vert::OptionalMark<Vertex<Scalar>>,
            vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Edge :
        public vcl::Edge<
            EdgeMeshT<Scalar>,
            edge::BitFlags,
            edge::VertexPointers<Vertex<Scalar>>,
            edge::OptionalColor<Edge<Scalar>>,
            edge::OptionalQuality<Scalar, Edge<Scalar>>,
            edge::OptionalAdjacentEdges<Edge<Scalar>>,
            edge::OptionalMark<Edge<Scalar>>,
            edge::CustomComponents<Edge<Scalar>>>
{
};

} // namespace vcl::edgemesh

namespace vcl {

template<typename Scalar = double>
class EdgeMeshT :
        public vcl::Mesh<
            mesh::VertexContainer<edgemesh::Vertex<Scalar>>,
            mesh::EdgeContainer<edgemesh::Edge<Scalar>>,
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

using EdgeMeshf = EdgeMeshT<float>;
using EdgeMesh  = EdgeMeshT<double>;

} // namespace vcl

#endif // VCL_MESHES_EDGE_MESH_H
