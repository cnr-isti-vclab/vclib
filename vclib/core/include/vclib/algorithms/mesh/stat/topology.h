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

#ifndef VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H
#define VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H

#include <vclib/concepts/mesh.h>

namespace vcl {

namespace detail {

// function called for the Container Cont of the Mesh
template<typename Cont>
void setReferencedVertices(const auto& mesh, auto& refs, uint& nRefs)
{
    // check if the Cont container of the Mesh has vertex references
    if constexpr (comp::HasVertexReferences<typename Cont::ElementType>) {
        // if there are still some vertices non-referenced
        if (nRefs < mesh.vertexNumber()) {
            constexpr uint ELEM_ID = Cont::ElementType::ELEMENT_ID;
            // for eache element of the Cont container
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                // for each vertex of the element
                for (uint vi : el.vertexIndices()) {
                    // vertex references shoule never be null
                    assert(vi != UINT_NULL);
                    if (!refs[vi]) {
                        // set the vertex as referenced
                        refs[vi] = true;
                        nRefs++;
                    }
                }
            }
        }
    }
}

// function called for each container of the Mesh, using variadic templates
template<typename... Cont>
void setReferencedVertices(
    const auto& mesh,
    auto&       refs,
    uint&       nRefs,
    TypeWrapper<Cont...>)
{
    // call the setReferencedVerticesOnVector function for each container of the
    // mesh
    (setReferencedVertices<Cont>(mesh, refs, nRefs), ...);
}

} // namespace detail

/**
 * @brief Count the number of references to vertices in the mesh faces.
 *
 * If the mesh is a TriangleMesh, the number of references is equal to the
 * number of faces times 3. Otherwise, the function counts the number of
 * references to vertices in each face of the mesh.
 *
 * @param[in] mesh: The input mesh. It must satisfy the MeshConcept.
 * @return The number of references to vertices in the mesh faces.
 */
uint countPerFaceVertexReferences(const MeshConcept auto& mesh)
{
    using MeshType = decltype(mesh);

    uint nRefs = 0;

    if constexpr (FaceMeshConcept<MeshType>) {
        if constexpr (TriangleMeshConcept<MeshType>) {
            return mesh.faceNumber() * 3;
        }
        else {
            for (const auto& f : mesh.faces()) {
                nRefs += f.vertexNumber();
            }
        }
    }

    return nRefs;
}

/**
 * @brief Counts the number of resulting triangles if the input mesh would be
 * triangulated by splitting each face into triangles.
 *
 * @param[in] mesh: The input mesh. It must satisfy the MeshConcept.
 * @return The number of resulting triangles if the input mesh would be
 * triangulated by splitting each face into triangles.
 */
uint countTriangulatedTriangles(const MeshConcept auto& mesh)
{
    using MeshType = decltype(mesh);

    uint nTris = 0;

    if constexpr (FaceMeshConcept<MeshType>) {
        for (const auto& f : mesh.faces()) {
            nTris += f.vertexNumber() - 2;
        }
    }

    return nTris;
}

/**
 * @brief Returns a Container of values interpreted as booleans telling, for
 * each vertex of the mesh, if it is referenced.
 *
 * If the parameter `onlyFaces` is `false` (default), the check is made for each
 * Element of the mesh that stores Vertex References. If `onlyFaces` is `true`,
 * the check is made only for the Faces of the mesh.
 *
 * The size of the returned container will be == to the vertexContainerSize of
 * the mesh, and all the deleted vertices are marked as unreferenced by default.
 *
 * @tparam Container: The type of the container to be used to store the boolean
 * values. It must be a container that can be indexed with integer values and
 * that can be initialized with a size and a default value.
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * MeshConcept.
 *
 * @param[in] mesh: The input mesh for which to calculate the referenced
 * vertices.
 * @param[out] nUnref: The number of non-referenced vertices.
 * @param[in] onlyFaces: If true, only the faces of the mesh are considered.
 * @return a Container of values interpreted as booleans telling, for each
 * vertex of the mesh, if it is referenced.
 */
template<typename Container, MeshConcept MeshType>
Container referencedVertices(
    const MeshType& mesh,
    uint&           nUnref,
    bool            onlyFaces = false)
{
    using VertexType = MeshType::VertexType;

    uint nRefs = 0;

    Container refVertices(mesh.vertexContainerSize(), false);

    if (onlyFaces) {
        if constexpr (FaceMeshConcept<MeshType>) {
            using FaceContainer = MeshType::FaceContainer;

            detail::setReferencedVertices(
                mesh, refVertices, nRefs, TypeWrapper<FaceContainer>());
        }
    }
    else {
        detail::setReferencedVertices(
            mesh, refVertices, nRefs, typename MeshType::Containers());
    }

    nUnref = mesh.vertexNumber() - nRefs;

    return refVertices;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H
