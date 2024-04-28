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

template<typename ScalarType>
class TriEdgeMeshT;

} // namespace vcl

namespace vcl::triedgemesh {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Face;

template<typename Scalar>
class Edge;

template<typename Scalar>
class Vertex :
        public vcl::Vertex<
            TriEdgeMeshT<Scalar>,
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vert::OptionalAdjacentEdgePointers<Edge<Scalar>, Vertex<Scalar>>,
            vert::OptionalAdjacentFacePointers<Face<Scalar>, Vertex<Scalar>>,
            vert::OptionalAdjacentVertexPointers<Vertex<Scalar>>,
            vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vert::OptionalMark<Vertex<Scalar>>,
            vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
        public vcl::Face<
            TriEdgeMeshT<Scalar>,
            face::TriangleBitFlags,
            face::TriangleVertexPtrs<Vertex<Scalar>, Face<Scalar>>,
            face::Normal3<Scalar>,
            face::OptionalQuality<Scalar, Face<Scalar>>,
            face::OptionalColor<Face<Scalar>>,
            face::OptionalAdjacentTrianglePointers<Face<Scalar>>,
            face::OptionalTriangleWedgeTexCoords<Scalar, Face<Scalar>>,
            face::OptionalMark<Face<Scalar>>,
            face::CustomComponents<Face<Scalar>>>
{
};

template<typename Scalar>
class Edge :
        public vcl::Edge<
            TriEdgeMeshT<Scalar>,
            edge::BitFlags,
            edge::VertexPointers<Vertex<Scalar>, Edge<Scalar>>,
            edge::OptionalQuality<Scalar, Edge<Scalar>>,
            edge::OptionalColor<Edge<Scalar>>,
            edge::OptionalAdjacentEdgePointers<Edge<Scalar>>,
            edge::OptionalMark<Edge<Scalar>>,
            edge::CustomComponents<Edge<Scalar>>>
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
template<typename Scalar = double>
class TriEdgeMeshT :
        public vcl::Mesh<
            mesh::VertexContainer<triedgemesh::Vertex<Scalar>>,
            mesh::FaceContainer<triedgemesh::Face<Scalar>>,
            mesh::EdgeContainer<triedgemesh::Edge<Scalar>>,
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
 * `float` as scalar.
 * @ingroup meshes
 */
using TriEdgeMeshf = TriEdgeMeshT<float>;

/**
 * @brief The TriEdgeMesh class is a specialization of TriEdgeMeshT that uses
 * `double` as scalar.
 * @ingroup meshes
 */
using TriEdgeMesh = TriEdgeMeshT<double>;

} // namespace vcl

#endif // VCL_MESHES_TRI_EDGE_MESH_H
