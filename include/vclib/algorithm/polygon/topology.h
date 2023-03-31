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

#ifndef VCL_ALGORITHM_POLYGON_TOPOLOGY_H
#define VCL_ALGORITHM_POLYGON_TOPOLOGY_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template <FaceConcept FaceType>
bool isFaceManifoldOnEdge(const FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>;

template<FaceConcept FaceType>
bool isFaceEdgeOnBorder(const FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>;

template<FaceConcept FaceType>
bool checkFlipEdge(const FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>;

template<FaceConcept FaceType>
uint edgeAdjacentFacesNumber(const FaceType& f, uint edge)
	requires comp::HasAdjacentFaces<FaceType>;

template <FaceConcept FaceType>
uint faceEdgesOnBorderNumber(const FaceType& f) requires comp::HasAdjacentFaces<FaceType>;

template <FaceConcept FaceType>
void detachAdjacentFacesOnEdge(FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>;

template <FaceConcept FaceType>
void detachFace(FaceType& f) requires comp::HasAdjacentFaces<FaceType>;

template<FaceConcept Face>
std::vector<uint> earCut(const Face& polygon);

template<FaceMeshConcept MeshType, FaceConcept FaceType>
void addTriangleFacesFromPolygon(MeshType& m, FaceType& f, const std::vector<uint>& polygon);

template<FaceMeshConcept MeshType>
uint addTriangleFacesFromPolygon(MeshType& m, const std::vector<uint>& polygon);

} // namespace vcl

#include "topology.cpp"

#endif // VCL_ALGORITHM_POLYGON_TOPOLOGY_H
