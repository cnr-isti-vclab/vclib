/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_ALGORITHMS_SHUFFLE_H
#define VCL_ALGORITHMS_SHUFFLE_H

#include <random>

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(MeshType& m, std::random_device& rd);

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(MeshType& m);

template<MeshConcept MeshType>
std::vector<const typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(const MeshType& m, std::random_device& rd);

template<MeshConcept MeshType>
std::vector<const typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(const MeshType& m);

template<MeshConcept MeshType>
std::vector<uint>
fillAndShuffleVertexIndexVector(const MeshType& m, std::random_device& rd);

template<MeshConcept MeshType>
std::vector<uint>
fillAndShuffleVertexIndexVector(const MeshType& m);

template<FaceMeshConcept MeshType>
std::vector<typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(MeshType& m, std::random_device& rd);

template<FaceMeshConcept MeshType>
std::vector<typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(MeshType& m);

template<FaceMeshConcept MeshType>
std::vector<const typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(const MeshType& m, std::random_device& rd);

template<FaceMeshConcept MeshType>
std::vector<const typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(const MeshType& m);

template<FaceMeshConcept MeshType>
std::vector<uint>
fillAndShuffleFaceIndexVector(const MeshType& m, std::random_device& rd);

template<FaceMeshConcept MeshType>
std::vector<uint>
fillAndShuffleFaceIndexVector(const MeshType& m);

} // namespace vcl

#include "shuffle.cpp"

#endif // VCL_ALGORITHMS_SHUFFLE_H
