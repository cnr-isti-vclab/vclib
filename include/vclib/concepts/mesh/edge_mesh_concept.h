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

#ifndef VCL_CONCEPTS_MESH_EDGE_MESH_CONCEPT_H
#define VCL_CONCEPTS_MESH_EDGE_MESH_CONCEPT_H

#include "mesh_concept.h"
#include "per_edge.h"

namespace vcl {

template<typename T>
concept EdgeMeshConcept =
    MeshConcept<T> && mesh::HasEdgeContainer<T> &&
    requires(
        T o,
        const T& co,
        typename T::EdgeType e)
{
    { co.index(e) } -> std::same_as<uint>;
    { co.index(&e) } -> std::same_as<uint>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_EDGE_MESH_CONCEPT_H
