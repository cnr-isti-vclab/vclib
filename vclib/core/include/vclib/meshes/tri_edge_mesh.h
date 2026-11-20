// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_TRI_EDGE_MESH_H
#define VCL_MESHES_TRI_EDGE_MESH_H

#include <vclib/mesh.h>

namespace vcl {

namespace triedgemesh {

/**
 * @brief The VertexComponents type used by the TriEdgeMeshT class.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using VertexComponents = vcl::VertexComponents<
    vert::BitFlags,
    vert::Position3<Scalar>,
    vert::Normal3<Scalar>,
    vert::OptionalColor,
    vert::OptionalQuality<Scalar>,
    vert::OptionalAdjacentEdges<Index>,
    vert::OptionalAdjacentFaces<Index>,
    vert::OptionalAdjacentVertices<Index>,
    vert::OptionalPrincipalCurvature<Scalar>,
    vert::OptionalTexCoord<Scalar>,
    vert::OptionalMaterialIndex,
    vert::OptionalTangent3<Scalar>,
    vert::CustomComponents>;

/**
 * @brief The FaceComponents type used by the TriEdgeMeshT class.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using FaceComponents = vcl::FaceComponents<
    face::TriangleBitFlags,
    face::TriangleVertexReferences<Index>,
    face::Normal3<Scalar>,
    face::OptionalColor,
    face::OptionalQuality<Scalar>,
    face::OptionalTriangleAdjacentFaces<Index>,
    face::OptionalAdjacentEdges<Index>,
    face::OptionalTriangleWedgeTexCoords<Scalar>,
    face::OptionalMaterialIndex,
    face::CustomComponents>;

/**
 * @brief The EdgeComponents type used by the TriEdgeMeshT class.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using EdgeComponents = vcl::EdgeComponents<
    edge::BitFlags,
    edge::VertexReferences<Index>,
    edge::OptionalNormal3<Scalar>,
    edge::OptionalColor,
    edge::OptionalQuality<Scalar>,
    edge::OptionalAdjacentEdges<Index>,
    edge::OptionalAdjacentFaces<Index>,
    edge::CustomComponents>;

} // namespace triedgemesh

/**
 * @brief The TriEdgeMeshT class is a mesh class that represents a triangle
 * mesh with edges.
 *
 * It allows to store triedgemesh vertices, triedgemesh faces, and
 * triedgemesh edges.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using TriEdgeMeshT = Mesh<
    mesh::VertexContainer<triedgemesh::VertexComponents<Scalar, Index>, Index>,
    mesh::FaceContainer<triedgemesh::FaceComponents<Scalar, Index>, Index>,
    mesh::EdgeContainer<triedgemesh::EdgeComponents<Scalar, Index>, Index>,
    mesh::BoundingBox3<Scalar>,
    mesh::Color,
    mesh::Materials,
    mesh::Name,
    mesh::TransformMatrix<Scalar>,
    mesh::CustomComponents>;

/**
 * @brief The TriEdgeMesh class is a specialization of the TriEdgeMeshT class
 * that uses `double` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using TriEdgeMesh = TriEdgeMeshT<double, uint>;

/**
 * @brief The TriEdgeMeshf class is a specialization of the TriEdgeMeshT class
 * that uses `float` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using TriEdgeMeshf = TriEdgeMeshT<float, uint>;

/**
 * @brief The TriEdgeMesh64 class is a specialization of the TriEdgeMeshT class
 * that uses `double` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using TriEdgeMesh64 = TriEdgeMeshT<double, uint64_t>;

/**
 * @brief The TriEdgeMesh64f class is a specialization of the TriEdgeMeshT class
 * that uses `float` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using TriEdgeMesh64f = TriEdgeMeshT<float, uint64_t>;

} // namespace vcl

#endif // VCL_MESHES_TRI_EDGE_MESH_H
