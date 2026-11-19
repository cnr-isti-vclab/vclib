// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_FACE_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_FACE_H

#include "base.h"

namespace vcl {

/* =========================================================================
 * Face Topology Specializations
 * ========================================================================= */

/**
 * @brief Concept that checks whether a Mesh has triangular faces.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTriangles = HasFaces<MeshType> && requires {
    typename std::remove_cvref_t<MeshType>::Face;
    requires TriangleFaceConcept<typename std::remove_cvref_t<MeshType>::Face>;
};

/**
 * @brief Concept that checks whether a Mesh has quadrilateral faces.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasQuads = HasFaces<MeshType> && requires {
    typename std::remove_cvref_t<MeshType>::Face;
    requires QuadFaceConcept<typename std::remove_cvref_t<MeshType>::Face>;
};

/**
 * @brief Concept that checks whether a Mesh has general polygonal faces.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasPolygons = HasFaces<MeshType> && requires {
    typename std::remove_cvref_t<MeshType>::Face;
    requires PolygonFaceConcept<typename std::remove_cvref_t<MeshType>::Face>;
};

/**
 * @brief Concept that checks whether a type is a triangle Mesh.
 * @ingroup mesh_concepts
 */
template<typename M>
concept TriangleMeshConcept = FaceMeshConcept<M> && HasTriangles<M>;

/**
 * @brief Concept that checks whether a type is a quadrilateral Mesh.
 * @ingroup mesh_concepts
 */
template<typename M>
concept QuadMeshConcept = FaceMeshConcept<M> && HasQuads<M>;

/**
 * @brief Concept that checks whether a type is a general polygon Mesh.
 * @ingroup mesh_concepts
 */
template<typename M>
concept PolygonMeshConcept = FaceMeshConcept<M> && HasPolygons<M>;

/* =========================================================================
 * Face Component Concepts
 * ========================================================================= */

/* Normals */
template<typename M>
concept HasFaceNormal =
    HasElementComponent<M, ElemId::FACE, CompId::NORMAL>;

template<typename M>
concept HasFaceOptionalNormal =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::NORMAL>;

/* Colors */
template<typename M>
concept HasFaceColor =
    HasElementComponent<M, ElemId::FACE, CompId::COLOR>;

template<typename M>
concept HasFaceOptionalColor =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::COLOR>;

/* Wedges */
template<typename M>
concept HasFaceWedgeColors =
    HasElementComponent<M, ElemId::FACE, CompId::WEDGE_COLORS>;

template<typename M>
concept HasFaceOptionalWedgeColors =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::WEDGE_COLORS>;

template<typename M>
concept HasFaceWedgeTexCoords =
    HasElementComponent<M, ElemId::FACE, CompId::WEDGE_TEX_COORDS>;

template<typename M>
concept HasFaceOptionalWedgeTexCoords =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::WEDGE_TEX_COORDS>;

template<typename M>
concept HasFaceWedgeNormals =
    HasElementComponent<M, ElemId::FACE, CompId::WEDGE_NORMALS>;

template<typename M>
concept HasFaceOptionalWedgeNormals =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::WEDGE_NORMALS>;

template<typename M>
concept HasFaceWedges =
    HasFaceWedgeColors<M> || HasFaceWedgeTexCoords<M> || HasFaceWedgeNormals<M>;

/* Adjacencies */
template<typename M>
concept HasFaceAdjacentFaces =
    HasElementAdjacentFaces<M, ElemId::FACE>;

template<typename M>
concept HasFaceOptionalAdjacentFaces =
    HasElementOptionalAdjacentFaces<M, ElemId::FACE>;

template<typename M>
concept HasFaceAdjacentEdges =
    HasElementAdjacentEdges<M, ElemId::FACE>;

template<typename M>
concept HasFaceOptionalAdjacentEdges =
    HasElementOptionalAdjacentEdges<M, ElemId::FACE>;

/* Quality */
template<typename M>
concept HasFaceQuality =
    HasElementComponent<M, ElemId::FACE, CompId::QUALITY>;

template<typename M>
concept HasFaceOptionalQuality =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::QUALITY>;

/* MaterialIndex */
template<typename M>
concept HasFaceMaterialIndex =
    HasElementComponent<M, ElemId::FACE, CompId::MATERIAL_INDEX>;

template<typename M>
concept HasFaceOptionalMaterialIndex =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::MATERIAL_INDEX>;

/* Custom Components */
template<typename M>
concept HasFaceCustomComponents =
    HasElementCustomComponents<M, ElemId::FACE>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_FACE_H
