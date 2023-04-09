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

#ifndef VCL_CONCEPTS_MESH_PER_FACE_H
#define VCL_CONCEPTS_MESH_PER_FACE_H

#include "containers.h"
#include "elements/face.h"

namespace vcl {

template<typename MeshType>
concept HasPerFaceAdjacentEdges =
	HasFaces<MeshType> && vcl::face::HasAdjacentEdges<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceAdjacentFaces =
	HasFaces<MeshType> && vcl::face::HasAdjacentFaces<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceColor =
	HasFaces<MeshType> && vcl::face::HasColor<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceHalfEdgeReference =
	HasFaces<MeshType> && vcl::face::HasHalfEdgeReference<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceMark =
	HasFaces<MeshType> && vcl::face::HasMark<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceNormal =
	HasFaces<MeshType> && vcl::face::HasNormal<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFacePrincipalCurvature =
	HasFaces<MeshType> && vcl::face::HasPrincipalCurvature<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceScalar =
	HasFaces<MeshType> && vcl::face::HasScalar<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceWedgeColors =
	HasFaces<MeshType> && vcl::face::HasWedgeColors<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceWedgeTexCoords =
	HasFaces<MeshType> && vcl::face::HasWedgeTexCoords<typename MeshType::FaceType>;

template<typename MeshType>
concept HasPerFaceCustomComponents =
	HasFaces<MeshType> && vcl::face::HasCustomComponents<typename MeshType::FaceType>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_PER_FACE_H
