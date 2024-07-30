/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_MESH_EDGE_MESH_CONCEPT_H
#define VCL_CONCEPTS_MESH_EDGE_MESH_CONCEPT_H

#include "mesh_concept.h"
#include "per_edge.h"

namespace vcl {

/**
 * @brief The EdgeMeshConcept is evaluated true if the type T is a Mesh (it
 * satisfies the @ref vcl::MeshConcept) and has an EdgeContainer.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept EdgeMeshConcept = MeshConcept<T> && mesh::HasEdgeContainer<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_EDGE_MESH_CONCEPT_H
