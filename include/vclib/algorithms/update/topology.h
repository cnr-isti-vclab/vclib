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

#ifndef VCL_ALGORITHMS_TOPOLOGY_H
#define VCL_ALGORITHMS_TOPOLOGY_H

namespace vcl {

template<typename MeshType>
void clearPerVertexAdjacentFaces(MeshType& m);

template<typename MeshType>
void updatePerVertexAdjacentFaces(MeshType& m);

template<typename MeshType>
void clearPerVertexAdjacentVertices(MeshType& m);

template<typename MeshType>
void updatePerVertexAdjacentVertices(MeshType& m);

template<typename MeshType>
void clearPerFaceAdjacentFaces(MeshType& m);

template<typename MeshType>
void updatePerFaceAdjacentFaces(MeshType& m);

} // namespace vcl

#include "topology.cpp"

#endif // VCL_ALGORITHMS_TOPOLOGY_H
