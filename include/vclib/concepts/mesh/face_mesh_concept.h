/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_CONCEPTS_MESH_FACE_MESH_CONCEPT_H
#define VCL_CONCEPTS_MESH_FACE_MESH_CONCEPT_H

#include "mesh_concept.h"
#include "per_face.h"

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

template<typename T>
concept FaceMeshConcept = MeshConcept<T> && mesh::HasFaceContainer<T>;

template<typename T>
concept TriangleMeshConcept = FaceMeshConcept<T> && HasTriangles<T>;

template<typename T>
concept QuadMeshConcept = FaceMeshConcept<T> && HasQuads<T>;

template<typename T>
concept PolygonMeshConcept = FaceMeshConcept<T> && HasPolygons<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_FACE_MESH_CONCEPT_H
