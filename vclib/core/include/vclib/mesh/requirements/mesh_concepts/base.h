// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_BASE_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_BASE_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements/element_concepts.h>

/**
 * @defgroup mesh_concepts Mesh Concepts
 * @ingroup mesh_requirements
 *
 * @brief Concepts that check the structure, topology, and component availability
 * of Mesh classes.
 */

namespace vcl {

/* =========================================================================
 * Base Mesh Structure & Container Presence
 *
 * MeshConcept<M> is defined in <vclib/mesh/mesh.h> as the primary identity
 * concept for any Mesh class.
 * ========================================================================= */

/**
 * @brief Concept that checks whether a Mesh has an ElementContainer of type ELEM_ID.
 * @tparam MeshType: The mesh type to check.
 * @tparam ELEM_ID: The element ID.
 * @ingroup mesh_concepts
 */
template<typename MeshType, ElemId ELEM_ID>
concept HasContainerOf = requires {
    requires std::remove_cvref_t<MeshType>::template hasContainerOf<ELEM_ID>();
};

/**
 * @brief Concept that checks whether a Mesh has a FaceContainer.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasFaces = HasContainerOf<MeshType, ElemId::FACE>;

/**
 * @brief Concept that checks whether a Mesh has an EdgeContainer.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasEdges = HasContainerOf<MeshType, ElemId::EDGE>;

/**
 * @brief Concept that checks whether a type is a Mesh with faces.
 * @ingroup mesh_concepts
 */
template<typename M>
concept FaceMeshConcept = MeshConcept<M> && HasFaces<M>;

/**
 * @brief Concept that checks whether a type is a Mesh with edges.
 * @ingroup mesh_concepts
 */
template<typename M>
concept EdgeMeshConcept = MeshConcept<M> && HasEdges<M>;

/* =========================================================================
 * Element Component Presence on Mesh (Generic)
 * ========================================================================= */

/**
 * @brief Concept that checks whether a Mesh has a container for the given
 * ELEM_ID with the given component COMP_ID.
 * @ingroup mesh_concepts
 */
template<typename MeshType, ElemId ELEM_ID, CompId COMP_ID>
concept HasElementComponent =
    HasContainerOf<MeshType, ELEM_ID> &&
    comp::HasComponent<
        COMP_ID,
        typename std::remove_cvref_t<MeshType>::template ContainerType<
            ELEM_ID>>;

/**
 * @brief Concept that checks whether a Mesh has a container for the given
 * ELEM_ID with the given optional component COMP_ID.
 * @ingroup mesh_concepts
 */
template<typename MeshType, ElemId ELEM_ID, CompId COMP_ID>
concept HasElementOptionalComponent =
    HasContainerOf<MeshType, ELEM_ID> &&
    comp::HasOptionalComponent<
        COMP_ID,
        typename std::remove_cvref_t<MeshType>::template ContainerType<
            ELEM_ID>>;

/* Adjacencies (Generic per Element) */
template<typename M, ElemId ELEM_ID>
concept HasElementAdjacentFaces =
    HasElementComponent<M, ELEM_ID, CompId::ADJACENT_FACES>;

template<typename M, ElemId ELEM_ID>
concept HasElementOptionalAdjacentFaces =
    HasElementOptionalComponent<M, ELEM_ID, CompId::ADJACENT_FACES>;

template<typename M, ElemId ELEM_ID>
concept HasElementAdjacentEdges =
    HasElementComponent<M, ELEM_ID, CompId::ADJACENT_EDGES>;

template<typename M, ElemId ELEM_ID>
concept HasElementOptionalAdjacentEdges =
    HasElementOptionalComponent<M, ELEM_ID, CompId::ADJACENT_EDGES>;

template<typename M, ElemId ELEM_ID>
concept HasElementAdjacentVertices =
    HasElementComponent<M, ELEM_ID, CompId::ADJACENT_VERTICES>;

template<typename M, ElemId ELEM_ID>
concept HasElementOptionalAdjacentVertices =
    HasElementOptionalComponent<M, ELEM_ID, CompId::ADJACENT_VERTICES>;

/* Custom Components on Elements (Generic) */
template<typename M, ElemId ELEM_ID>
concept HasElementCustomComponents =
    HasElementComponent<M, ELEM_ID, CompId::CUSTOM_COMPONENTS>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_BASE_H

