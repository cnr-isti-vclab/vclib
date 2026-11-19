// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_MESH_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_MESH_H

#include "base.h"

namespace vcl {

/* =========================================================================
 * Mesh-Level Components
 * ========================================================================= */

template<typename M>
concept HasTransformMatrix = mesh::HasTransformMatrix<M>;

template<typename M>
concept HasBoundingBox = mesh::HasBoundingBox<M>;

template<typename M>
concept HasMaterials = mesh::HasMaterials<M>;

template<typename M>
concept HasName = mesh::HasName<M>;

template<typename M>
concept HasMeshColor = mesh::HasColor<M>;

template<typename M>
concept HasMeshCustomComponents = mesh::HasCustomComponents<M>;

/* =========================================================================
 * Mesh Ranges Utility Concepts
 * ========================================================================= */

/**
 * @brief A concept that checks whether a type is a range of Meshes.
 *
 * The concept is satisfied when `T` is a range and the value type of the
 * range satisfies the MeshConcept.
 *
 * @tparam T: The type to be tested for conformity to the RangeOfMeshes concept.
 *
 * @ingroup mesh
 * @ingroup mesh_concepts
 */
template<typename T>
concept RangeOfMeshes =
    std::ranges::range<T> && MeshConcept<std::ranges::range_value_t<T>>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_MESH_H

