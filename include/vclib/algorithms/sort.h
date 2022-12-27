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

#ifndef VCL_ALGORITHMS_SORT_H
#define VCL_ALGORITHMS_SORT_H

#include <vclib/mesh_utils/mesh_edge_util.h>

namespace vcl {

template<FaceMeshConcept MeshType>
std::vector<MeshEdgeUtil<MeshType>> fillAndSortMeshEdgeUtilVector(MeshType& m, bool includeFauxEdges = true);

template<FaceMeshConcept MeshType>
std::vector<ConstMeshEdgeUtil<MeshType>>
fillAndSortMeshEdgeUtilVector(const MeshType& m, bool includeFauxEdges = true);

} // namespace vcl

#include "sort.cpp"

#endif // VCL_ALGORITHMS_SORT_H
