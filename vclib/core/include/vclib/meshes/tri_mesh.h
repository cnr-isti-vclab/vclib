// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_TRI_MESH_H
#define VCL_MESHES_TRI_MESH_H

#include <vclib/mesh.h>

namespace vcl {

namespace trimesh {

/**
 * @brief The VertexComponents type used by the TriMeshT class.
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
    vert::OptionalAdjacentFaces<Index>,
    vert::OptionalAdjacentVertices<Index>,
    vert::OptionalPrincipalCurvature<Scalar>,
    vert::OptionalTexCoord<Scalar>,
    vert::OptionalMaterialIndex,
    vert::OptionalTangent3<Scalar>,
    vert::CustomComponents>;

/**
 * @brief The FaceComponents type used by the TriMeshT class.
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
    face::OptionalTriangleWedgeTexCoords<Scalar>,
    face::OptionalMaterialIndex,
    face::CustomComponents>;

} // namespace trimesh

/**
 * @brief The TriMeshT class is a mesh class that represents a triangle mesh.
 *
 * It allows to store trimesh vertices and trimesh faces.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using TriMeshT = Mesh<
    mesh::VertexContainer<trimesh::VertexComponents<Scalar, Index>, Index>,
    mesh::FaceContainer<trimesh::FaceComponents<Scalar, Index>, Index>,
    mesh::BoundingBox3<Scalar>,
    mesh::Color,
    mesh::Materials,
    mesh::Name,
    mesh::TransformMatrix<Scalar>,
    mesh::CustomComponents>;

/**
 * @brief The TriMesh class is a specialization of the TriMeshT class
 * that uses `double` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using TriMesh = TriMeshT<double, uint>;

/**
 * @brief The TriMeshf class is a specialization of the TriMeshT class
 * that uses `float` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using TriMeshf = TriMeshT<float, uint>;

/**
 * @brief The TriMesh64 class is a specialization of the TriMeshT class
 * that uses `double` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using TriMesh64 = TriMeshT<double, uint64_t>;

/**
 * @brief The TriMesh64f class is a specialization of the TriMeshT class
 * that uses `float` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using TriMesh64f = TriMeshT<float, uint64_t>;

} // namespace vcl

#endif // VCL_MESHES_TRI_MESH_H
