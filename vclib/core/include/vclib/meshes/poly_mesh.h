// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_POLY_MESH_H
#define VCL_MESHES_POLY_MESH_H

#include <vclib/mesh.h>

namespace vcl {

namespace polymesh {

/**
 * @brief The VertexComponents type used by the PolyMeshT class.
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
 * @brief The FaceComponents type used by the PolyMeshT class.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using FaceComponents = vcl::FaceComponents<
    face::PolygonBitFlags,
    face::PolygonVertexReferences<Index>,
    face::Normal3<Scalar>,
    face::OptionalColor,
    face::OptionalQuality<Scalar>,
    face::OptionalPolygonAdjacentFaces<Index>,
    face::OptionalPolygonWedgeTexCoords<Scalar>,
    face::OptionalMaterialIndex,
    face::CustomComponents>;

} // namespace polymesh

/**
 * @brief The PolyMeshT class is a mesh class that represents a polygonal mesh.
 *
 * It allows to store polymesh vertices and polymesh faces.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using PolyMeshT = Mesh<
    mesh::VertexContainer<polymesh::VertexComponents<Scalar, Index>, Index>,
    mesh::FaceContainer<polymesh::FaceComponents<Scalar, Index>, Index>,
    mesh::BoundingBox3<Scalar>,
    mesh::Color,
    mesh::Materials,
    mesh::Name,
    mesh::TransformMatrix<Scalar>,
    mesh::CustomComponents>;

/**
 * @brief The PolyMesh class is a specialization of the PolyMeshT class
 * that uses `double` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using PolyMesh = PolyMeshT<double, uint>;

/**
 * @brief The PolyMeshf class is a specialization of the PolyMeshT class
 * that uses `float` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using PolyMeshf = PolyMeshT<float, uint>;

/**
 * @brief The PolyMesh64 class is a specialization of the PolyMeshT class
 * that uses `double` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using PolyMesh64 = PolyMeshT<double, uint64_t>;

/**
 * @brief The PolyMesh64f class is a specialization of the PolyMeshT class
 * that uses `float` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using PolyMesh64f = PolyMeshT<float, uint64_t>;

} // namespace vcl

#endif // VCL_MESHES_POLY_MESH_H
