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

#ifndef VCL_CONCEPTS_MESH_PER_MESH_H
#define VCL_CONCEPTS_MESH_PER_MESH_H

#include "components.h"
#include "mesh_concept.h"

namespace vcl {

namespace mesh {

/* Port concepts into the mesh namespace */
template<typename T>
concept HasBoundingBox = comp::HasBoundingBox<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasName = comp::HasName<T>;
template<typename T>
concept HasTexturePaths = comp::HasTexturePaths<T>;
template<typename T>
concept HasTransformMatrix = comp::HasTransformMatrix<T>;

} // namespace vcl::mesh

/**
 * @brief Concept that is evaluated true if a Mesh has the BoundingBox component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasBoundingBox = MeshConcept<MeshType> && mesh::HasBoundingBox<MeshType>;

/**
 * @brief Concept that is evaluated true if a Mesh has the Color component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasColor = MeshConcept<MeshType> && mesh::HasColor<MeshType>;

/**
 * @brief Concept that is evaluated true if a Mesh has the Mark component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasMark = MeshConcept<MeshType> && mesh::HasMark<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the Name component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasName = MeshConcept<MeshType> && mesh::HasName<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the TexturePaths component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTexturePaths = MeshConcept<MeshType> && mesh::HasTexturePaths<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the TransformMatrix component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTransformMatrix = MeshConcept<MeshType> && mesh::HasTransformMatrix<MeshType>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_PER_MESH_H
