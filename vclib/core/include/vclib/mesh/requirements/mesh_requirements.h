/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_MESH_REQUIREMENTS_MESH_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_MESH_REQUIREMENTS_H

#include "face_requirements.h"

#include <vclib/mesh/concepts/mesh_concept.h>
#include <vclib/mesh/mesh_components.h>

#include <vclib/exceptions.h>

/**
 * @defgroup mesh_concepts Mesh Concepts
 * @ingroup mesh_requirements
 *
 * @brief List of concepts for types related to the Mesh data structures of the
 * library. They allow to discriminate between different Mesh types, their
 * elements and the element components.
 */

namespace vcl {

/************
 * concepts *
 ************/

/**
 * @brief Concept that is evaluated true if a Mesh has the BoundingBox
 * component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasBoundingBox =
    MeshConcept<MeshType> && mesh::HasBoundingBox<MeshType>;

/**
 * @brief Concept that is evaluated true if a Mesh has the Color component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasColor = MeshConcept<MeshType> && mesh::HasColor<MeshType>;

/**
 * @brief Concept that is evaluated true if a Mesh has the CustomComponents
 * component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasCustomComponents =
    MeshConcept<MeshType> && mesh::HasCustomComponents<MeshType>;

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
 * @brief Concept that checks if a Mesh has the TextureImages component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTextureImages =
    MeshConcept<MeshType> && mesh::HasTextureImages<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the TexturePaths component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTexturePaths =
    MeshConcept<MeshType> && mesh::HasTexturePaths<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the TransformMatrix component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTransformMatrix =
    MeshConcept<MeshType> && mesh::HasTransformMatrix<MeshType>;

/********************
 * is/has functions *
 ********************/

/**
 * @brief Checks *at run time* if the mesh m is composed of triangles.
 *
 * This function works both for triangle and polygonal meshes composed of faces
 * having 3 vertices. If the size of the Face Vertices is static and 3, this
 * check is immediate. If it is not 3, the functions checks for each face if it
 * has 3 vertices.
 *
 * Complexity: *O(n)*
 *
 * @tparam MeshType: the type of the mesh. It must satisfy the MeshConcept.
 *
 * @param[in] m: the mesh on which check if each face has 3 vertices.
 * @return `true` if every face of the mesh is composed of 3 vertices.
 *
 * @ingroup mesh_requirements
 */
template<MeshConcept MeshType>
bool isTriangleMesh(const MeshType& m)
{
    if constexpr (HasTriangles<MeshType>) {
        return true;
    }
    else if constexpr (HasFaces<MeshType>) {
        using F = MeshType::FaceType;
        for (const F& f : m.faces()) {
            if (f.vertexNumber() != 3)
                return false;
        }
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Checks *at run time* if the mesh is composed of quads.
 *
 * This function works both for quad and polygonal meshes composed of faces
 * having 4 vertices. If the size of the Face Vertices is 4, this check
 * is immediate. If it is not 4, the functions checks for each face if it has 4
 * vertices.
 *
 * Complexity: *O(n)*
 *
 * @tparam MeshType: the type of the mesh. It must satisfy the MeshConcept.
 *
 * @param[in] m: the mesh on which check if each face has 4 vertices.
 * @return `true` if every face of the mesh is composed of 4 vertices.
 *
 * @ingroup mesh_requirements
 */
template<MeshConcept MeshType>
bool isQuadMesh(const MeshType& m)
{
    if constexpr (HasQuads<MeshType>) {
        return true;
    }
    else if constexpr (HasFaces<MeshType>) {
        using F = MeshType::FaceType;
        for (const F& f : m.faces()) {
            if (f.vertexNumber() != 4)
                return false;
        }
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Checks if a Mesh is compact, that is if it does not contains deleted
 * elements.
 *
 * It is equivalent to call `m.isCompact()`.
 *
 * @return `true` if `m` is compact, `false` otherwise.
 *
 * @ingroup mesh_requirements
 */
template<MeshConcept MeshType>
bool isCompact(const MeshType& m)
{
    return m.isCompact();
}

/*********************
 * require functions *
 *********************/

/**
 * @brief Checks if the mesh is composed of triangles, and if not, throws an
 * exception.
 *
 * This function works both for triangle and polygonal meshes composed of faces
 * having 3 vertices. If the size of the Face Vertices is static and 3, this
 * check is immediate. If it is not 3, the functions checks for each face if it
 * has 3 vertices.
 *
 * Complexity: *O(n)*
 *
 * @throws vcl::MissingTriangularRequirementException if the mesh is not
 * composed of triangles.
 *
 * @tparam MeshType: the type of the mesh. It must satisfy the FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check if each face has 3 vertices.
 *
 * @ingroup mesh_requirements
 */
template<FaceMeshConcept MeshType>
void requireTriangleMesh(const MeshType& m)
{
    if constexpr (!HasTriangles<MeshType>) {
        for (const auto& f : m.faces()) {
            if (f.vertexNumber() != 3) {
                throw MissingTriangularRequirementException(
                    "Triangle Mesh Required.");
            }
        }
    }
}

/**
 * @brief Checks if the mesh is composed of quads, and if not, throws an
 * exception.
 *
 * This function works both for quad and polygonal meshes composed of faces
 * having 4 vertices. If the size of the Face Vertices is static and 3, this
 * check is immediate. If it is not 4, the functions checks for each face if it
 * has 4 vertices.
 *
 * Complexity: *O(n)*
 *
 * @throws vcl::MissingQuadRequirementException if the mesh is not
 * composed of quads.
 *
 * @tparam MeshType: the type of the mesh. It must satisfy the FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check if each face has 4 vertices.
 *
 * @ingroup mesh_requirements
 */
template<FaceMeshConcept MeshType>
void requireQuadMesh(const MeshType& m)
{
    if constexpr (!HasQuads<MeshType>) {
        for (const auto& f : m.faces()) {
            if (f.vertexNumber() != 4) {
                throw MissingQuadRequirementException("Quad Mesh Required.");
            }
        }
    }
}

/**
 * @brief Checks if a Mesh is compact, that is if it does not contains deleted
 * elements, and if not, throws an exception.
 *
 * @throws vcl::MissingCompactnessException if the mesh is not compact.
 *
 * @tparam MeshType: the type of the mesh. It must satisfy the MeshConcept.
 *
 * @param[in] m: the mesh on which check if is compact.
 *
 * @ingroup mesh_requirements
 */
template<MeshConcept MeshType>
void requireCompactness(const MeshType& m)
{
    if (!m.isCompact())
        throw MissingCompactnessException("Mesh is not compact.");
}

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_REQUIREMENTS_H
