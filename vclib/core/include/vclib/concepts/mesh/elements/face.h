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

#ifndef VCL_CONCEPTS_MESH_ELEMENTS_FACE_H
#define VCL_CONCEPTS_MESH_ELEMENTS_FACE_H

#include "element.h"

#include <vclib/concepts/mesh/components.h>

/**
 * @defgroup face_concepts Face Concepts
 * @ingroup element_concepts
 *
 * @brief List of concepts for types related to the Face Element.
 */

namespace vcl {

namespace face {

/**
 * @ingroup face_concepts face_components
 * @{
 */

/* Port concepts into the face namespace */
template<typename T>
concept HasAdjacentEdges = comp::HasAdjacentEdges<T>;
template<typename T>
concept HasOptionalAdjacentEdges = comp::HasOptionalAdjacentEdges<T>;
template<typename T>
concept HasAdjacentFaces = comp::HasAdjacentFaces<T>;
template<typename T>
concept HasOptionalAdjacentFaces = comp::HasOptionalAdjacentFaces<T>;
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasOptionalColor = comp::HasOptionalColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasFaceBitFlags = comp::HasFaceBitFlags<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasOptionalMark = comp::HasOptionalMark<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasOptionalNormal = comp::HasOptionalNormal<T>;
template<typename T>
concept HasPolygonBitFlags = comp::HasPolygonBitFlags<T>;
template<typename T>
concept HasPrincipalCurvature = comp::HasPrincipalCurvature<T>;
template<typename T>
concept HasOptionalPrincipalCurvature = comp::HasOptionalPrincipalCurvature<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasTriangleBitFlags = comp::HasTriangleBitFlags<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;
template<typename T>
concept HasWedgeColors = comp::HasWedgeColors<T>;
template<typename T>
concept HasOptionalWedgeColors = comp::HasOptionalWedgeColors<T>;
template<typename T>
concept HasWedgeTexCoords = comp::HasWedgeTexCoords<T>;
template<typename T>
concept HasOptionalWedgeTexCoords = comp::HasOptionalWedgeTexCoords<T>;

/** @} */ // end of group

} // namespace face

/**
 * @brief The FaceConcept describes how a Face element that can be
 * used for a FaceContainer should be organized.
 *
 * The Face concept is satisfied for a class F if ALL the following sentences
 * are true:
 * - The class F has the BitFlags component (or a derivate);
 * - The class F has either VertexPointers or VertexIndices components;
 * - The number of vertices of the VertexPointers/VertexIndices is -1 (dynamic
 * size) or at least 3 (static size)
 * - If the class F has the TriangleBitFlags component (or a derivate), the
 * number of vertices must be 3 (static)
 * - If the class F has the AdjacentEdges component (or a derivate), its size
 * must be the same of the vertices;
 * - If the class F has the AdjacentFaces component (or a derivate), its size
 * must be the same of the vertices;
 * - If the class F has the WedgeColors component (or a derivate), its size must
 * be the same of the vertices;
 * - If the class F has the WedgeTexCoords component (or a derivate), its size
 * must be the same of the vertices;
 *
 * @ingroup face_concepts
 */
template<typename T>
concept FaceConcept =
    ElementConcept<T> && RemoveRef<T>::ELEMENT_ID == ElemId::FACE &&
    face::HasBitFlags<T> && face::HasVertexReferences<T> &&
    (RemoveRef<T>::VERTEX_NUMBER < 0 || RemoveRef<T>::VERTEX_NUMBER >= 3) &&
    (!face::HasTriangleBitFlags<T> || RemoveRef<T>::VERTEX_NUMBER == 3) &&
    comp::SanityCheckAdjacentEdges<T> && comp::SanityCheckAdjacentFaces<T> &&
    comp::SanityCheckWedgeColors<T> && comp::SanityCheckWedgeTexCoords<T>;

template<typename T>
concept TriangleFaceConcept =
    RemoveRef<T>::VERTEX_NUMBER == 3 && FaceConcept<T>;

/**
 * @brief The PolygonFaceConcept describes how a Face element class should be
 * organized to be a polygonal face with dynamic size.
 *
 * The PolygonFace concept is satisfied for a class F if ALL the following
 * sentences are true:
 * - It satisfies the FaceConcept;
 * - The number of vertices of the VertexPointers component is -1 (dynamic
 * size);
 *
 * @ingroup face_concepts
 */
template<typename T>
concept PolygonFaceConcept = RemoveRef<T>::VERTEX_NUMBER < 0 && FaceConcept<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_ELEMENTS_FACE_H
