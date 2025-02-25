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

#ifndef VCL_MESHES_POLY_EDGE_MESH_H
#define VCL_MESHES_POLY_EDGE_MESH_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename ScalarType, bool INDEXED>
class PolyEdgeMeshT;

} // namespace vcl

namespace vcl::polyedgemesh {

template<typename Scalar, bool INDEXED>
class Vertex;

template<typename Scalar, bool INDEXED>
class Face;

template<typename Scalar, bool INDEXED>
class Edge;

template<typename Scalar, bool I>
class Vertex :
        public vcl::Vertex<
            PolyEdgeMeshT<Scalar, I>,
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar, I>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar, I>>,
            vert::OptionalAdjacentEdges<I, Edge<Scalar, I>, Vertex<Scalar, I>>,
            vert::OptionalAdjacentFaces<I, Face<Scalar, I>, Vertex<Scalar, I>>,
            vert::OptionalAdjacentVertices<I, Vertex<Scalar, I>>,
            vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar, I>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar, I>>,
            vert::OptionalMark<Vertex<Scalar, I>>,
            vert::CustomComponents<Vertex<Scalar, I>>>
{
};

template<typename Scalar, bool I>
class Face :
        public vcl::Face<
            PolyEdgeMeshT<Scalar, I>,
            face::PolygonBitFlags,
            face::PolygonVertexRefs<I, Vertex<Scalar, I>, Face<Scalar, I>>,
            face::Normal3<Scalar>,
            face::OptionalColor<Face<Scalar, I>>,
            face::OptionalQuality<Scalar, Face<Scalar, I>>,
            face::OptionalAdjacentPolygons<I, Face<Scalar, I>>,
            face::OptionalAdjacentEdges<I, Edge<Scalar, I>, Face<Scalar, I>>,
            face::OptionalPolygonWedgeTexCoords<Scalar, Face<Scalar, I>>,
            face::OptionalMark<Face<Scalar, I>>,
            face::CustomComponents<Face<Scalar, I>>>
{
};

template<typename Scalar, bool I>
class Edge :
        public vcl::Edge<
            PolyEdgeMeshT<Scalar, I>,
            edge::BitFlags,
            edge::VertexReferences<I, Vertex<Scalar, I>, Edge<Scalar, I>>,
            edge::OptionalNormal3<Scalar, Edge<Scalar, I>>,
            edge::OptionalColor<Edge<Scalar, I>>,
            edge::OptionalQuality<Scalar, Edge<Scalar, I>>,
            edge::OptionalAdjacentEdges<I, Edge<Scalar, I>>,
            edge::OptionalAdjacentFaces<I, Face<Scalar, I>, Edge<Scalar, I>>,
            edge::OptionalMark<Edge<Scalar, I>>,
            edge::CustomComponents<Edge<Scalar, I>>>
{
};

} // namespace vcl::polyedgemesh

namespace vcl {

/**
 * @brief The PolyEdgeMeshT is a Polygonal Mesh type that stores Vertices, Faces
 * and Edges as elements.
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam INDEXED: A boolean flag that indicates whether the mesh uses indices
 * or pointers to store references.
 *
 * @ingroup meshes
 */
template<typename Scalar, bool INDEXED>
class PolyEdgeMeshT :
        public Mesh<
            mesh::VertexContainer<polyedgemesh::Vertex<Scalar, INDEXED>>,
            mesh::FaceContainer<polyedgemesh::Face<Scalar, INDEXED>>,
            mesh::EdgeContainer<polyedgemesh::Edge<Scalar, INDEXED>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Color,
            mesh::Mark,
            mesh::Name,
            mesh::TextureImages,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    using ScalarType = Scalar;
};

/**
 * @brief The PolyEdgeMeshf class is a specialization of PolyEdgeMeshT that uses
 * `float` as scalar and pointers to store vertices of faces/edges and adjacency
 * information.
 * @ingroup meshes
 */
using PolyEdgeMeshf = PolyEdgeMeshT<float, false>;

/**
 * @brief The PolyEdgeMesh class is a specialization of PolyEdgeMeshT that uses
 * `double` as scalar and pointers to store vertices of faces/edges and
 * adjacency information.
 * @ingroup meshes
 */
using PolyEdgeMesh = PolyEdgeMeshT<double, false>;

/**
 * @brief The PolyEdgeMeshIndexedf class is a specialization of PolyEdgeMeshT
 * that uses `float` as scalar and indices to store vertices of faces/edges and
 * adjacency information.
 * @ingroup meshes
 */
using PolyEdgeMeshIndexedf = PolyEdgeMeshT<float, true>;

/**
 * @brief The PolyEdgeMeshIndexed class is a specialization of PolyEdgeMeshT
 * that uses `double` as scalar and indices to store vertices of faces/edges and
 * adjacency information.
 * @ingroup meshes
 */
using PolyEdgeMeshIndexed = PolyEdgeMeshT<double, true>;

} // namespace vcl

#endif // VCL_MESHES_POLY_EDGE_MESH_H
