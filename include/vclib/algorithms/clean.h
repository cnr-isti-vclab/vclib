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

#ifndef VCL_ALGORITHMS_CLEAN_H
#define VCL_ALGORITHMS_CLEAN_H

#include <set>
#include <vector>

#include <vclib/mesh/requirements.h>

/**
 * @defgroup clean Cleaning and Repairing Algorithms
 *
 * @ingroup algorithms
 *
 * @brief List of Mesh Cleaning and repairing algorithms.
 *
 * You can access these algorithms by including `#include <vclib/algorithms/clean.h>`
 */

namespace vcl {

template <MeshConcept MeshType>
uint numberUnreferencedVertices(const MeshType& m);

template <MeshConcept MeshType>
uint removeUnreferencedVertices(MeshType& m);

template <MeshConcept MeshType>
uint removeDuplicatedVertices(MeshType& m);

template <FaceMeshConcept MeshType>
uint removeDuplicatedFaces(MeshType& m);

template <MeshConcept MeshType>
uint removeDegeneratedVertices(MeshType& m,  bool deleteAlsoFaces = true);

template <FaceMeshConcept MeshType>
uint removeDegenerateFaces(MeshType& m);

template <FaceMeshConcept MeshType>
uint numberNonManifoldVertices(const MeshType& m);

template <FaceMeshConcept MeshType>
bool isWaterTight(const MeshType& m);

template <FaceMeshConcept MeshType>
uint numberHoles(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>;

template <FaceMeshConcept MeshType>
std::vector<std::set<uint>> connectedComponents(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>;

template <FaceMeshConcept MeshType>
uint numberConnectedComponents(const MeshType& m);

}

#include "clean.cpp"

#endif // VCL_ALGORITHMS_CLEAN_H
