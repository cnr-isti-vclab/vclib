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

#ifndef VCL_MESHES_EDGE_MESH_H
#define VCL_MESHES_EDGE_MESH_H

#include <vclib/mesh.h>

namespace vcl {

template<typename ScalarType, bool INDEXED>
class EdgeMeshT;

} // namespace vcl

namespace vcl::edgemesh {

template<typename Scalar, bool INDEXED>
class Vertex;

template<typename Scalar, bool INDEXED>
class Edge;

/**
 * @brief The Vertex type used by the EdgeMeshT class.
 *
 * @extends vert::BitFlags
 * @extends vert::Position3
 * @extends vert::Normal3
 * @extends vert::OptionalColor
 * @extends vert::OptionalQuality
 * @extends vert::OptionalAdjacentEdges
 * @extends vert::OptionalAdjacentVertices
 * @extends vert::OptionalTexCoord
 * @extends vert::OptionalMark
 * @extends vert::CustomComponents
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam I: A boolean flag that indicates whether the mesh uses indices or
 * pointers to store vertices of edges and adjacency information.
 *
 * @ingroup meshes
 */
template<typename Scalar, bool I>
class Vertex :
        public vcl::Vertex<
            EdgeMeshT<Scalar, I>,
            vert::BitFlags,
            vert::Position3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar, I>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar, I>>,
            vert::OptionalAdjacentEdges<I, Edge<Scalar, I>, Vertex<Scalar, I>>,
            vert::OptionalAdjacentVertices<I, Vertex<Scalar, I>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar, I>>,
            vert::OptionalMark<Vertex<Scalar, I>>,
            vert::CustomComponents<Vertex<Scalar, I>>>
{
};

/**
 * @brief The Edge type used by the EdgeMeshT class.
 *
 * @extends edge::BitFlags
 * @extends edge::VertexReferences
 * @extends edge::OptionalNormal3
 * @extends edge::OptionalColor
 * @extends edge::OptionalQuality
 * @extends edge::OptionalAdjacentEdges
 * @extends edge::OptionalMark
 * @extends edge::CustomComponents
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam I: A boolean flag that indicates whether the mesh uses indices or
 * pointers to store vertices of edges and adjacency information.
 *
 * @ingroup meshes
 */
template<typename Scalar, bool I>
class Edge :
        public vcl::Edge<
            EdgeMeshT<Scalar, I>,
            edge::BitFlags,
            edge::VertexReferences<I, Vertex<Scalar, I>, Edge<Scalar, I>>,
            edge::OptionalNormal3<Scalar, Edge<Scalar, I>>,
            edge::OptionalColor<Edge<Scalar, I>>,
            edge::OptionalQuality<Scalar, Edge<Scalar, I>>,
            edge::OptionalAdjacentEdges<I, Edge<Scalar, I>>,
            edge::OptionalMark<Edge<Scalar, I>>,
            edge::CustomComponents<Edge<Scalar, I>>>
{
};

} // namespace vcl::edgemesh

namespace vcl {

/**
 * @brief The EdgeMeshT class is a mesh class that represents a mesh that stores
 * only vertices and edges (no faces).
 *
 * It allows to store edgemesh::Vertex and edgemesh::Edge elements.
 *
 * The mesh is templated over the scalar type and a boolean flag that indicates
 * whether the mesh uses indices to store vertices of edges and adjacency
 * information.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam INDEXED: A boolean flag that indicates whether the mesh uses indices
 * or pointers to store references.
 *
 * @extends mesh::VertexContainer
 * @extends mesh::EdgeContainer
 * @extends mesh::BoundingBox3
 * @extends mesh::Color
 * @extends mesh::Mark
 * @extends mesh::Name
 * @extends mesh::TextureImages
 * @extends mesh::TransformMatrix
 * @extends mesh::CustomComponents
 *
 * @ingroup meshes
 */
template<typename Scalar, bool INDEXED>
class EdgeMeshT :
        public Mesh<
            mesh::VertexContainer<edgemesh::Vertex<Scalar, INDEXED>>,
            mesh::EdgeContainer<edgemesh::Edge<Scalar, INDEXED>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Mark,
            mesh::Name,
            mesh::TextureImages,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    /** @brief The scalar used to store all the data of the Mesh. */
    using ScalarType = Scalar;
};

/**
 * @brief The EdgeMeshf class is a specialization of the EdgeMeshT class that
 * uses `float` as scalar and pointers to store vertices of edges and adjacency
 * information.
 * @ingroup meshes
 */
using EdgeMeshf = EdgeMeshT<float, false>;

/**
 * @brief The EdgeMesh class is a specialization of the EdgeMeshT class that
 * uses `double` as scalar and pointers to store vertices of edges and adjacency
 * information.
 * @ingroup meshes
 */
using EdgeMesh = EdgeMeshT<double, false>;

/**
 * @brief The EdgeMeshIndexedf class is a specialization of the EdgeMeshT class
 * that uses `float` as scalar and indices (`unsigned int`) to store vertices of
 * edges and adjacency information.
 * @ingroup meshes
 */
using EdgeMeshIndexedf = EdgeMeshT<float, true>;

/**
 * @brief The EdgeMeshIndexed class is a specialization of the EdgeMeshT class
 * that uses `double` as scalar and indices (`unsigned int`) to store vertices
 * of edges and adjacency information.
 * @ingroup meshes
 */
using EdgeMeshIndexed = EdgeMeshT<double, true>;

} // namespace vcl

#endif // VCL_MESHES_EDGE_MESH_H
