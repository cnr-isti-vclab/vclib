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
#include <vclib/mesh/requirements.h>

#include <map>

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

// struct to store the information of the wedge texcoords
template<typename WedgeTexCoordType>
struct WedgeTexCoordsInfo
{
    WedgeTexCoordType texCoord;
    ushort            texCoordIndex;

    bool operator<(const WedgeTexCoordsInfo& other) const
    {
        if (texCoordIndex == other.texCoordIndex) {
            return texCoord < other.texCoord;
        }
        return texCoordIndex < other.texCoordIndex;
    }
};

static inline std::list<uint>                             dummyUintList;
static inline std::list<std::list<std::pair<uint, uint>>> dummyListOfLists;
static inline std::vector<std::pair<uint, uint>>          dummyVectorOfPairs;

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
 * @brief Returns the largest face size in the mesh.
 *
 * If the mesh is a TriangleMesh, the function returns 3. Otherwise, the
 * function returns the size of the largest face in the mesh.
 *
 * @param[in] mesh: The input mesh. It must satisfy the MeshConcept.
 * @return The largest face size in the mesh.
 */
uint largestFaceSize(const MeshConcept auto& mesh)
{
    using MeshType = decltype(mesh);

    uint maxFaceSize = 0;

    if constexpr (TriangleMeshConcept<MeshType>) {
        return 3;
    }
    else if constexpr (FaceMeshConcept<MeshType>) {
        for (const auto& f : mesh.faces()) {
            maxFaceSize = std::max(maxFaceSize, f.vertexNumber());
        }
    }

    return maxFaceSize;
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

/**
 * @brief This function counts the number of vertices that must be duplicated in
 * a mesh to have a unique texcoord per vertex, by checking the texcoords of the
 * wedges of the mesh faces.
 *
 * The function returns the number of vertices that must be duplicated (i.e.,
 * added to the mesh) to have a unique texcoord per vertex. The function also
 * returns a vector that tells, for each vertex, the pair face/wedge index in
 * the face that must be kept for the vertex, the list of vertices to duplicate,
 * and the list of faces that must be reassigned to the duplicated vertices.
 *
 * @param[in] mesh: The input mesh. It must satisfy the FaceMeshConcept and must
 * have per-face wedge texcoords.
 * @param[out] vertWedgeMap: A vector that tells, for each vertex, the pair
 * face/wedge index in the face that must be kept for the vertex (it allows to
 * index the texcoords of the vertex).
 * @param[out] vertsToDuplicate: a list of vertices that must be duplicated
 * (each element of the list is the index of the vertex to duplicate).
 * @param[out] facesToReassign: a list of lists of pairs face/wedge index in the
 * face that must be reassigned to the duplicated vertices (each list of pairs
 * is the list of faces that must be reassigned to the corresponding duplicated
 * vertex). The list contains a list for each vertex to duplicate.
 * @return The number of vertices that must be duplicated.
 */
template<FaceMeshConcept MeshType>
uint countVerticesToDuplicateByWedgeTexCoords(
    const MeshType&                    mesh,
    std::vector<std::pair<uint, uint>> vertWedgeMap =
        detail::dummyVectorOfPairs,
    std::list<uint>& vertsToDuplicate = detail::dummyUintList,
    std::list<std::list<std::pair<uint, uint>>>& facesToReassign =
        detail::dummyListOfLists)
{
    vcl::requirePerFaceWedgeTexCoords(mesh);

    using WedgeTexCoordType  = MeshType::FaceType::WedgeTexCoordType;
    using WedgeTexCoordsInfo = detail::WedgeTexCoordsInfo<WedgeTexCoordType>;

    // list of faces that reference a wedge texcoord, with the index of the
    // vertex in the face
    using FaceList = std::list<std::pair<uint, uint>>;

    vertWedgeMap.resize(mesh.vertexContainerSize());
    vertsToDuplicate.clear();
    facesToReassign.clear();

    uint count = 0;

    // for each vertex, I'll store a map of WedgeTexCoordsInfo
    // each element of the map represent a unique wedge texcoord(the texcoord
    // itself and the index of the texcoord), and for each element it maps a
    // list of faces that reference the texcoord
    std::vector<std::map<WedgeTexCoordsInfo, FaceList>> wedges(
        mesh.vertexContainerSize());

    for (const auto& f : mesh.faces()) {
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            uint vi = f.vertexIndex(i);

            // check if the i-th wedge texcoord of the face already exists
            WedgeTexCoordsInfo wi = {f.wedgeTexCoord(i), f.textureIndex()};
            auto               it = wedges[vi].find(wi);
            if (it == wedges[vi].end()) { // if it doesn't exist, add it
                // if there was already a texcoord for the vertex, it means that
                // the vertex will be duplicated
                if (!wedges[vi].empty()) {
                    count++;
                }
                wedges[vi][wi].emplace_back(f.index(), i);
            }
            else {
                // if it exists, add the face to the list of faces that
                // reference the texcoord
                it->second.emplace_back(f.index(), i);
            }
        }
    }

    // for each vertex, check if there are multiple texcoords
    // note: here we will modify the maps for convenience: at the end of this
    // loop, the info will be contained in the two lists vertsToDuplicate and
    // facesToReassign (the wedges vector of map will be inconsistent)
    for (uint vi = 0; auto& map : wedges) {
        if (map.size() > 1) {
            // there are multiple texcoords for the vertex vi, so it will be
            // duplicated

            // remove from the map the element having the higher number of
            // faces referencing the texcoord (we do this to reassign the less
            // number of faces)
            auto it = std::max_element(
                map.begin(), map.end(), [](const auto& a, const auto& b) {
                    return a.second.size() < b.second.size();
                });
            // store the reference of the texcoord to keep (pair face/vertex
            // index in the face)
            vertWedgeMap[vi] = it->second.front();
            map.erase(it);

            // store the vertex to duplicate, and the faces to reassign
            for (auto& [wi, fl] : map) {
                vertsToDuplicate.push_back(vi);
                facesToReassign.push_back(std::move(fl));
            }
        }
        else {
            if (map.size() == 1) {
                // there is only one texcoord for the vertex vi, so store its
                // reference (pair face/vertex index in the face)
                vertWedgeMap[vi] = map.begin()->second.front();
            }
            else { // the vertex was unreferenced
                vertWedgeMap[vi] = {UINT_NULL, UINT_NULL};
            }
        }
        ++vi;
    }

    assert(vertWedgeMap.size() == mesh.vertexContainerSize());
    assert(vertsToDuplicate.size() == count);
    assert(facesToReassign.size() == count);

    return count;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H
