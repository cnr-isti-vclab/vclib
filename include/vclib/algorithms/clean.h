/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_ALGORITHMS_CLEAN_H
#define VCL_ALGORITHMS_CLEAN_H

#include <vclib/misc/vcl_types.h>

namespace vcl {

template <typename MeshType>
uint numberUnreferencedVertices(const MeshType& m);

template <typename MeshType>
uint removeUnreferencedVertices(MeshType& m);

template <typename MeshType>
uint removeDuplicatedVertices(MeshType& m);

template <typename MeshType>
uint removeDuplicatedFaces(MeshType& m);

template <typename MeshType>
uint removeDegeneratedVertices(MeshType& m,  bool deleteAlsoFaces = true);

}

#include "clean.cpp"

#endif // VCL_ALGORITHMS_CLEAN_H
