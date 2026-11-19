// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_EDGE_MESH_H
#define VCL_MESHES_EDGE_MESH_H

#include <vclib/mesh.h>

namespace vcl {

namespace edgemesh {

/**
 * @brief The VertexComponents type used by the EdgeMeshT class.
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
    vert::OptionalAdjacentVertices<Index>,
    vert::OptionalTexCoord<Scalar>,
    vert::OptionalMaterialIndex,
    vert::CustomComponents>;

/**
 * @brief The EdgeComponents type used by the EdgeMeshT class.
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
    edge::CustomComponents>;

} // namespace edgemesh

/**
 * @brief The EdgeMeshT class is a mesh class that represents a mesh that stores
 * only vertices and edges (no faces).
 *
 * It allows to store edgemesh vertices and edgemesh edges.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using EdgeMeshT = Mesh<
    mesh::VertexContainer<edgemesh::VertexComponents<Scalar, Index>, Index>,
    mesh::EdgeContainer<edgemesh::EdgeComponents<Scalar, Index>, Index>,
    mesh::BoundingBox3<Scalar>,
    mesh::Color,
    mesh::Materials,
    mesh::Name,
    mesh::TransformMatrix<Scalar>,
    mesh::CustomComponents>;

/**
 * @brief The EdgeMesh class is a specialization of the EdgeMeshT class
 * that uses `double` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using EdgeMesh = EdgeMeshT<double, uint>;

/**
 * @brief The EdgeMeshf class is a specialization of the EdgeMeshT class
 * that uses `float` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using EdgeMeshf = EdgeMeshT<float, uint>;

/**
 * @brief The EdgeMesh64 class is a specialization of the EdgeMeshT class
 * that uses `double` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using EdgeMesh64 = EdgeMeshT<double, uint64_t>;

/**
 * @brief The EdgeMesh64f class is a specialization of the EdgeMeshT class
 * that uses `float` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using EdgeMesh64f = EdgeMeshT<float, uint64_t>;

} // namespace vcl

#endif // VCL_MESHES_EDGE_MESH_H
