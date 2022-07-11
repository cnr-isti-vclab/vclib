/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_H

#include "element_concepts.h"
#include "../mesh_concept.h"

namespace vcl {

template<typename MeshType>
concept HasTriangles =
	vcl::HasFaces<MeshType> && MeshType::FaceType::VERTEX_NUMBER == 3;

template<typename MeshType>
concept HasQuads =
	vcl::HasFaces<MeshType> && MeshType::FaceType::VERTEX_NUMBER == 4;

template<typename MeshType>
concept HasPolygons =
	vcl::HasFaces<MeshType> && MeshType::FaceType::VERTEX_NUMBER == -1;

template<typename MeshType>
concept HasBoundingBox =
	mesh::HasBoundingBox<MeshType>;

template<typename MeshType>
concept HasMark =
	mesh::HasMark<MeshType>;

template<typename MeshType>
concept HasTexFileNames =
	mesh::HasTexFileNames<MeshType>;

template<typename MeshType>
concept HasTransformMatrix =
	mesh::HasTransformMatrix<MeshType>;

template<typename T>
concept MeshConcept =
	(mesh::IsDerivedFromMesh<T>::value || mesh::IsAMesh<T>::value) &&
	mesh::HasVertexContainer<T>;

template<typename T>
concept FaceMeshConcept =
	MeshConcept<T> && mesh::HasFaceContainer<T>;

template<typename T>
concept TriangleMeshConcept =
	FaceMeshConcept<T> && HasTriangles<T>;

template<typename T>
concept QuadMeshConcept =
	FaceMeshConcept<T> && HasQuads<T>;

template<typename T>
concept PolygonMeshConcept =
	FaceMeshConcept<T> && HasPolygons<T>;

template<typename T>
concept EdgeMeshConcept =
	MeshConcept<T> && mesh::HasEdgeContainer<T>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_H
