// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_POLY_EDGE_MESH_H
#define VCL_MESHES_POLY_EDGE_MESH_H

#include <vclib/mesh.h>

namespace vcl {

namespace polyedgemesh {

/**
 * @brief The VertexComponents type used by the PolyEdgeMeshT class.
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
 * @brief The FaceComponents type used by the PolyEdgeMeshT class.
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
    face::OptionalPolygonAdjacentEdges<Index>,
    face::OptionalPolygonWedgeTexCoords<Scalar>,
    face::OptionalMaterialIndex,
    face::CustomComponents>;

/**
 * @brief The EdgeComponents type used by the PolyEdgeMeshT class.
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

} // namespace polyedgemesh

/**
 * @brief The PolyEdgeMeshT class is a mesh class that represents a polygonal
 * mesh with edges.
 *
 * It allows to store polyedgemesh vertices, polyedgemesh faces, and
 * polyedgemesh edges.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using PolyEdgeMeshT = Mesh<
    mesh::VertexContainer<polyedgemesh::VertexComponents<Scalar, Index>, Index>,
    mesh::FaceContainer<polyedgemesh::FaceComponents<Scalar, Index>, Index>,
    mesh::EdgeContainer<polyedgemesh::EdgeComponents<Scalar, Index>, Index>,
    mesh::BoundingBox3<Scalar>,
    mesh::Color,
    mesh::Materials,
    mesh::Name,
    mesh::TransformMatrix<Scalar>,
    mesh::CustomComponents>;

/**
 * @brief The PolyEdgeMesh class is a specialization of the PolyEdgeMeshT class
 * that uses `double` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using PolyEdgeMesh = PolyEdgeMeshT<double, uint>;

/**
 * @brief The PolyEdgeMeshf class is a specialization of the PolyEdgeMeshT class
 * that uses `float` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using PolyEdgeMeshf = PolyEdgeMeshT<float, uint>;

/**
 * @brief The PolyEdgeMesh64 class is a specialization of the PolyEdgeMeshT class
 * that uses `double` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using PolyEdgeMesh64 = PolyEdgeMeshT<double, uint64_t>;

/**
 * @brief The PolyEdgeMesh64f class is a specialization of the PolyEdgeMeshT class
 * that uses `float` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using PolyEdgeMesh64f = PolyEdgeMeshT<float, uint64_t>;

} // namespace vcl

#endif // VCL_MESHES_POLY_EDGE_MESH_H
