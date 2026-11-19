// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_ELEMENT_CONCEPTS_H
#define VCL_MESH_REQUIREMENTS_ELEMENT_CONCEPTS_H

#include <vclib/mesh/components.h>
#include <vclib/mesh/elements.h>

/**
 * @defgroup element_concepts Element Concepts
 * @ingroup mesh_requirements
 *
 * @brief Concepts for Element types (Vertex, Face, Edge) and their components.
 */

namespace vcl {

/* =========================================================================
 * Base Element Identity Concepts
 *
 * ElementConcept, VertexConcept, FaceConcept, EdgeConcept, and WedgeConcept
 * are the primary identity concepts defined alongside their respective
 * element and proxy definitions:
 * - ElementConcept<T>: defined in <vclib/mesh/elements/base/element_proxy.h>
 * - VertexConcept<T>:  defined in <vclib/mesh/elements/vertex.h>
 * - FaceConcept<T>:    defined in <vclib/mesh/elements/face.h>
 * - EdgeConcept<T>:    defined in <vclib/mesh/elements/edge.h>
 * - WedgeConcept<T>:   defined in <vclib/mesh/components/wedges/wedge_proxy.h>
 * ========================================================================= */

/* =========================================================================
 * Topological Face Specializations
 * ========================================================================= */

/**
 * @brief Concept that checks whether a type T is a triangular Face element.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept TriangleFaceConcept = FaceConcept<T> && requires {
    requires std::remove_cvref_t<T>::VERTEX_COUNT == 3;
};

/**
 * @brief Concept that checks whether a type T is a quadrilateral Face element.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept QuadFaceConcept = FaceConcept<T> && requires {
    requires std::remove_cvref_t<T>::VERTEX_COUNT == 4;
};

/**
 * @brief Concept that checks whether a type T is a general polygon (dynamic size) Face element.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept PolygonFaceConcept = FaceConcept<T> && requires {
    requires std::remove_cvref_t<T>::VERTEX_COUNT < 0;
};

/* =========================================================================
 * Component Presence on Elements
 * ========================================================================= */

/**
 * @brief Checks whether an Element or component list has the Position component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasPosition = comp::HasPosition<E>;

template<typename E>
concept HasOptionalPosition = comp::HasOptionalPosition<E>;

/**
 * @brief Checks whether an Element or component list has the Normal component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasNormal =
    comp::HasNormal<E> || (WedgeConcept<E> && requires (E& e) { e.normal(); });

template<typename E>
concept HasOptionalNormal = comp::HasOptionalNormal<E>;

/**
 * @brief Checks whether an Element or component list has the Color component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasColor =
    comp::HasColor<E> || (WedgeConcept<E> && requires (E& e) { e.color(); });

template<typename E>
concept HasOptionalColor = comp::HasOptionalColor<E>;

/**
 * @brief Checks whether an Element or component list has the TexCoord component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasTexCoord =
    comp::HasTexCoord<E> || (WedgeConcept<E> && requires (E& e) { e.texCoord(); });

template<typename E>
concept HasOptionalTexCoord = comp::HasOptionalTexCoord<E>;

/**
 * @brief Checks whether an Element or component list has the AdjacentFaces component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasAdjacentFaces = comp::HasAdjacentFaces<E>;

template<typename E>
concept HasOptionalAdjacentFaces = comp::HasOptionalAdjacentFaces<E>;

/**
 * @brief Checks whether an Element or component list has the AdjacentEdges component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasAdjacentEdges = comp::HasAdjacentEdges<E>;

template<typename E>
concept HasOptionalAdjacentEdges = comp::HasOptionalAdjacentEdges<E>;

/**
 * @brief Checks whether an Element or component list has the AdjacentVertices component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasAdjacentVertices = comp::HasAdjacentVertices<E>;

template<typename E>
concept HasOptionalAdjacentVertices = comp::HasOptionalAdjacentVertices<E>;

/**
 * @brief Checks whether an Element or component list has the BitFlags component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasBitFlags = comp::HasBitFlags<E>;

/**
 * @brief Checks whether an Element or component list has the Quality component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasQuality = comp::HasQuality<E>;

template<typename E>
concept HasOptionalQuality = comp::HasOptionalQuality<E>;

/**
 * @brief Checks whether an Element or component list has the PrincipalCurvature component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasPrincipalCurvature = comp::HasPrincipalCurvature<E>;

template<typename E>
concept HasOptionalPrincipalCurvature = comp::HasOptionalPrincipalCurvature<E>;

/**
 * @brief Checks whether an Element or component list has the Tangent component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasTangent = comp::HasTangent<E>;

template<typename E>
concept HasOptionalTangent = comp::HasOptionalTangent<E>;

/**
 * @brief Checks whether an Element or component list has the MaterialIndex component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasMaterialIndex = comp::HasMaterialIndex<E>;

template<typename E>
concept HasOptionalMaterialIndex = comp::HasOptionalMaterialIndex<E>;

/**
 * @brief Checks whether an Element or component list has the VertexReferences component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasVertexReferences = comp::HasVertexReferences<E>;

/**
 * @brief Checks whether an Element or component list has the CustomComponents component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasCustomComponents = comp::HasCustomComponents<E>;

/**
 * @brief Checks whether an Element or component list has the Wedges component.
 * @tparam E: The element or component list type.
 * @ingroup element_concepts
 */
template<typename E>
concept HasWedges = comp::HasWedges<E>;

template<typename E>
concept HasWedgeColors = comp::HasWedgeColors<E>;

template<typename E>
concept HasOptionalWedgeColors = comp::HasOptionalWedgeColors<E>;

template<typename E>
concept HasWedgeTexCoords = comp::HasWedgeTexCoords<E>;

template<typename E>
concept HasOptionalWedgeTexCoords = comp::HasOptionalWedgeTexCoords<E>;

template<typename E>
concept HasWedgeNormals = comp::HasWedgeNormals<E>;

template<typename E>
concept HasOptionalWedgeNormals = comp::HasOptionalWedgeNormals<E>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_ELEMENT_CONCEPTS_H
