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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
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
            vcl::vert::BitFlags,
            vcl::vert::Coordinate3<Scalar>,
            vcl::vert::Normal3<Scalar>,
            vcl::vert::OptionalColor<Vertex<Scalar>>,
            vcl::vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalAdjacentEdges<Edge<Scalar>, Vertex<Scalar>>,
            vcl::vert::OptionalAdjacentVertices<Vertex<Scalar>>,
            vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalMark<Vertex<Scalar>>,
            vcl::vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Edge :
        public vcl::Edge<
            EdgeMeshT<Scalar>,
            vcl::edge::BitFlags,
            vcl::edge::VertexPointers<Vertex<Scalar>>,
            vcl::edge::OptionalQuality<Scalar, Edge<Scalar>>,
            vcl::edge::OptionalColor<Edge<Scalar>>,
            vcl::edge::OptionalAdjacentEdges<Edge<Scalar>>,
            vcl::edge::OptionalMark<Edge<Scalar>>,
            vcl::edge::CustomComponents<Edge<Scalar>>>
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
