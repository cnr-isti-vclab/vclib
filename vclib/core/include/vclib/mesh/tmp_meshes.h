// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_TMP_MESHES_H
#define VCL_MESH_TMP_MESHES_H

#include <vclib/mesh/mesh.h>

namespace vcl::detail {

namespace tmpMesh {

/**
 * @brief The VertexComponents type used by the TMPSimple*MeshT classes.
 *
 * @tparam Scalar: The scalar type used for vertex positions (default: double).
 * @tparam Index: The unsigned integral type used for indexing elements
 * (default: uint).
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using TmpVertexComponents =
    VertexComponents<vert::BitFlags, vert::Position3<Scalar>>;

/**
 * @brief The FaceComponents type used by the TMPSimpleTriMeshT class.
 *
 * @tparam Index: The unsigned integral type used for indexing elements
 * (default: uint).
 */
template<std::unsigned_integral Index = uint>
using TriFaceComponents = FaceComponents<
    face::TriangleBitFlags,
    face::TriangleVertexReferences<Index>>;

/**
 * @brief The FaceComponents type used by the TMPSimplePolyMeshT class.
 *
 * @tparam Index: The unsigned integral type used for indexing elements
 * (default: uint).
 */
template<std::unsigned_integral Index = uint>
using PolyFaceComponents =
    FaceComponents<face::PolygonBitFlags, face::PolygonVertexReferences<Index>>;

} // namespace tmpMesh

/**
 * @brief A minimal triangle mesh template used for temporary computations.
 *
 * @tparam Scalar: The scalar type used for vertex positions (default: double).
 * @tparam Index: The unsigned integral type used for indexing elements
 * (default: uint).
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using TMPSimpleTriMeshT = Mesh<
    mesh::VertexContainer<tmpMesh::TmpVertexComponents<Scalar, Index>, Index>,
    mesh::FaceContainer<tmpMesh::TriFaceComponents<Index>, Index>>;

/**
 * @brief A minimal polygon mesh template used for temporary computations.
 *
 * @tparam Scalar: The scalar type used for vertex positions (default: double).
 * @tparam Index: The unsigned integral type used for indexing elements
 * (default: uint).
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using TMPSimplePolyMeshT = Mesh<
    mesh::VertexContainer<tmpMesh::TmpVertexComponents<Scalar, Index>, Index>,
    mesh::FaceContainer<tmpMesh::PolyFaceComponents<Index>, Index>>;

/**
 * @brief A minimal triangle mesh using double as scalar and uint for indexing.
 */
using TMPSimpleTriMesh = TMPSimpleTriMeshT<double, uint>;

/**
 * @brief A minimal polygon mesh using double as scalar and uint for indexing.
 */
using TMPSimplePolyMesh = TMPSimplePolyMeshT<double, uint>;

} // namespace vcl::detail

#endif // VCL_MESH_TMP_MESHES_H
