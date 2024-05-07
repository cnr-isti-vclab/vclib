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

#ifndef VCL_MESHES_TRI_EDGE_MESH_H
#define VCL_MESHES_TRI_EDGE_MESH_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename ScalarType, bool INDEXED>
class TriEdgeMeshT;

} // namespace vcl

namespace vcl::triedgemesh {

template<typename Scalar, bool INDEXED>
class Vertex;

template<typename Scalar, bool INDEXED>
class Face;

template<typename Scalar, bool INDEXED>
class Edge;

template<typename Scalar, bool I>
class Vertex :
        public vcl::Vertex<
            TriEdgeMeshT<Scalar, I>,
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar, I>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar, I>>,
            vert::OptionalAdjacentEdgePointers<
                Edge<Scalar, I>,
                Vertex<Scalar, I>>,
            vert::OptionalAdjacentFaces<
                Face<Scalar, I>,
                Vertex<Scalar, I>>,
            vert::OptionalAdjacentVertices<Vertex<Scalar, I>>,
            vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar, I>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar, I>>,
            vert::OptionalMark<Vertex<Scalar, I>>,
            vert::CustomComponents<Vertex<Scalar, I>>>
{
};

template<typename Scalar, bool I>
class Face :
        public vcl::Face<
            TriEdgeMeshT<Scalar, I>,
            face::TriangleBitFlags,
            face::TriangleVertexRefs<I, Vertex<Scalar, I>, Face<Scalar, I>>,
            face::Normal3<Scalar>,
            face::OptionalColor<Face<Scalar, I>>,
            face::OptionalQuality<Scalar, Face<Scalar, I>>,
            face::OptionalAdjacentTriangles<Face<Scalar, I>>,
            face::
                OptionalAdjacentEdgePointers<Edge<Scalar, I>, Face<Scalar, I>>,
            face::OptionalTriangleWedgeTexCoords<Scalar, Face<Scalar, I>>,
            face::OptionalMark<Face<Scalar, I>>,
            face::CustomComponents<Face<Scalar, I>>>
{
};

template<typename Scalar, bool I>
class Edge :
        public vcl::Edge<
            TriEdgeMeshT<Scalar, I>,
            edge::BitFlags,
            edge::VertexReferences<I, Vertex<Scalar, I>, Edge<Scalar, I>>,
            edge::OptionalColor<Edge<Scalar, I>>,
            edge::OptionalQuality<Scalar, Edge<Scalar, I>>,
            edge::OptionalAdjacentEdgePointers<Edge<Scalar, I>>,
            edge::
                OptionalAdjacentFaces<Face<Scalar, I>, Edge<Scalar, I>>,
            edge::OptionalMark<Edge<Scalar, I>>,
            edge::CustomComponents<Edge<Scalar, I>>>
{
};

} // namespace vcl::triedgemesh

namespace vcl {

/**
 * @brief The TriEdgeMeshT is a Mesh type that stores Vertices, Faces and Edges
 * as elements.
 * @tparam Scalar: The scalar type used for the mesh.
 * @ingroup meshes
 */
template<typename Scalar, bool INDEXED>
class TriEdgeMeshT :
        public vcl::Mesh<
            mesh::VertexContainer<triedgemesh::Vertex<Scalar, INDEXED>>,
            mesh::FaceContainer<triedgemesh::Face<Scalar, INDEXED>>,
            mesh::EdgeContainer<triedgemesh::Edge<Scalar, INDEXED>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Color,
            mesh::Mark,
            mesh::Name,
            mesh::TexturePaths,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    using ScalarType = Scalar;
};

/**
 * @brief The TriEdgeMeshf class is a specialization of TriEdgeMeshT that uses
 * `float` as scalar and pointers to store vertices of faces/edges and adjacency
 * information.
 * @ingroup meshes
 */
using TriEdgeMeshf = TriEdgeMeshT<float, false>;

/**
 * @brief The TriEdgeMesh class is a specialization of TriEdgeMeshT that uses
 * `double` as scalar and pointers to store vertices of faces/edges and
 * adjacency information.
 * @ingroup meshes
 */
using TriEdgeMesh = TriEdgeMeshT<double, false>;

/**
 * @brief The TriEdgeMeshIndexedf class is a specialization of TriEdgeMeshT that
 * uses `float` as scalar and indices to store vertices of faces/edges and
 * adjacency information.
 * @ingroup meshes
 */
using TriEdgeMeshIndexedf = TriEdgeMeshT<float, true>;

/**
 * @brief The TriEdgeMeshIndexed class is a specialization of TriEdgeMeshT that
 * uses `double` as scalar and indices to store vertices of faces/edges and
 * adjacency information.
 * @ingroup meshes
 */
using TriEdgeMeshIndexed = TriEdgeMeshT<double, true>;

} // namespace vcl

#endif // VCL_MESHES_TRI_EDGE_MESH_H
