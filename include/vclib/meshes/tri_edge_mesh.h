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
            vcl::vert::BitFlags,
            vcl::vert::Coordinate3<Scalar>,
            vcl::vert::Normal3<Scalar>,
            vcl::vert::OptionalColor<Vertex<Scalar>>,
            vcl::vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalAdjacentFaces<Face<Scalar>, Vertex<Scalar>>,
            vcl::vert::OptionalAdjacentVertices<Vertex<Scalar>>,
            vcl::vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalMark<Vertex<Scalar>>,
            vcl::vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
        public vcl::Face<
            TriEdgeMeshT<Scalar>,
            vcl::face::TriangleBitFlags,
            vcl::face::TriangleVertexPtrs<Vertex<Scalar>>,
            vcl::face::Normal3<Scalar>,
            vcl::face::OptionalQuality<Scalar, Face<Scalar>>,
            vcl::face::OptionalColor<Face<Scalar>>,
            vcl::face::OptionalAdjacentTriangles<Face<Scalar>>,
            vcl::face::OptionalTriangleWedgeTexCoords<Scalar, Face<Scalar>>,
            vcl::face::OptionalMark<Face<Scalar>>,
            vcl::face::CustomComponents<Face<Scalar>>>
{
};

template<typename Scalar>
class Edge :
        public vcl::Edge<
            TriEdgeMeshT<Scalar>,
            vcl::edge::BitFlags,
            vcl::edge::VertexPointers<Vertex<Scalar>>,
            vcl::edge::OptionalQuality<Scalar, Edge<Scalar>>,
            vcl::edge::OptionalColor<Edge<Scalar>>,
            vcl::edge::OptionalAdjacentEdges<Edge<Scalar>>,
            vcl::edge::OptionalMark<Edge<Scalar>>,
            vcl::edge::CustomComponents<Edge<Scalar>>>
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
