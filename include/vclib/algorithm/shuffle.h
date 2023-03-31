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

#ifndef VCL_ALGORITHM_SHUFFLE_H
#define VCL_ALGORITHM_SHUFFLE_H

#include <random>

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(MeshType& m, bool deterministic = false);

template<MeshConcept MeshType>
std::vector<const typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(const MeshType& m, bool deterministic = false);

template<MeshConcept MeshType>
std::vector<uint>
fillAndShuffleVertexIndexVector(const MeshType& m, bool deterministic = false);

template<FaceMeshConcept MeshType>
std::vector<typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(MeshType& m, bool deterministic = false);

template<FaceMeshConcept MeshType>
std::vector<const typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(const MeshType& m, bool deterministic = false);

template<FaceMeshConcept MeshType>
std::vector<uint> fillAndShuffleFaceIndexVector(const MeshType& m, bool deterministic = false);

} // namespace vcl

#include "shuffle.cpp"

#endif // VCL_ALGORITHM_SHUFFLE_H
