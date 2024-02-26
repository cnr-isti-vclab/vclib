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

#ifndef VCL_CONCEPTS_MESH_PER_FACE_H
#define VCL_CONCEPTS_MESH_PER_FACE_H

#include "containers/face_container.h"
#include "elements/face.h"

/**
 * @defgroup face_mesh_concepts FaceMesh Concepts
 * @ingroup mesh_concepts
 *
 * @brief List of concepts for types related to Meshes having Faces.
 * They allow to check the presence of specific components for Faces.
 */

namespace vcl {

/**
 * @brief Concept that checks if a Mesh has the per Face AdjacentEdges
 * component.
 *
 * Evaluates to true if AdjacentEdges is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceAdjacentEdges =
    HasFaces<MeshType> &&
    vcl::face::HasAdjacentEdges<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face AdjacentFaces
 * component.
 *
 * Evaluates to true if AdjacentFaces is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceAdjacentFaces =
    HasFaces<MeshType> &&
    vcl::face::HasAdjacentFaces<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face BitFlags
 * component.
 *
 * Evaluates to true if BitFlags is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceBitFlags =
    HasFaces<MeshType> && vcl::face::HasBitFlags<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face Color
 * component.
 *
 * Evaluates to true if Color is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceColor =
    HasFaces<MeshType> && vcl::face::HasColor<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face CustomComponents
 * component.
 *
 * Evaluates to true if CustomComponents is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceCustomComponents =
    HasFaces<MeshType> &&
    vcl::face::HasCustomComponents<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face Mark
 * component.
 *
 * Evaluates to true if Mark is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceMark =
    HasFaces<MeshType> && vcl::face::HasMark<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face Normal
 * component.
 *
 * Evaluates to true if Normal is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceNormal =
    HasFaces<MeshType> && vcl::face::HasNormal<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face PolygonBitFlags
 * component.
 *
 * Evaluates to true if PolygonBitFlags is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFacePolygonBitFlags =
    HasFaces<MeshType> &&
    vcl::face::HasPolygonBitFlags<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face PrincipalCurvature
 * component.
 *
 * Evaluates to true if PrincipalCurvature is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFacePrincipalCurvature =
    HasFaces<MeshType> &&
    vcl::face::HasPrincipalCurvature<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face Quality
 * component.
 *
 * Evaluates to true if Quality is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceQuality =
    HasFaces<MeshType> && vcl::face::HasQuality<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face TriangleBitFlags
 * component.
 *
 * Evaluates to true if TriangleBitFlags is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceTriangleBitFlags =
    HasFaces<MeshType> &&
    vcl::face::HasTriangleBitFlags<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face VertexPointers
 * component.
 *
 * Evaluates to true if VertexPointers is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceVertexPointers =
    HasFaces<MeshType> &&
    vcl::face::HasVertexPointers<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face WedgeColors
 * component.
 *
 * Evaluates to true if WedgeColors is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceWedgeColors =
    HasFaces<MeshType> &&
    vcl::face::HasWedgeColors<typename MeshType::FaceType>;

/**
 * @brief Concept that checks if a Mesh has the per Face WedgeTexCoords
 * component.
 *
 * Evaluates to true if WedgeTexCoords is part of the Face element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup face_mesh_concepts
 */
template<typename MeshType>
concept HasPerFaceWedgeTexCoords =
    HasFaces<MeshType> &&
    vcl::face::HasWedgeTexCoords<typename MeshType::FaceType>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_PER_FACE_H
