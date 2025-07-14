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
#include <vclib/space/complex/mesh_edge_util.h>
#include <vclib/space/complex/mesh_pos.h>

#include <map>
#include <set>
#include <stack>

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

template<FaceMeshConcept MeshType>
std::vector<bool> nonManifoldVerticesVectorBool(const MeshType& m)
    requires HasPerFaceAdjacentFaces<MeshType>
{
    requirePerFaceAdjacentFaces(m);

    using FaceType = MeshType::FaceType;

    std::vector<bool> nonManifoldVertices(m.vertexContainerSize(), false);

    std::vector<uint> TD(m.vertexContainerSize(), 0);
    std::vector<bool> nonManifoldInc(m.vertexContainerSize(), false);
    // First Loop, count how many faces are incident on a vertex and store it in
    // TD, and flag how many vertices are incident on non manifold edges.
    for (const FaceType& f : m.faces()) {
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            TD[m.index(f.vertex(i))]++;
            if (!isFaceManifoldOnEdge(f, i)) {
                nonManifoldInc[m.index(f.vertex(i))]        = true;
                nonManifoldInc[m.index(f.vertexMod(i + 1))] = true;
            }
        }
    }

    std::vector<bool> visited(m.vertexContainerSize(), false);
    for (const FaceType& f : m.faces()) {
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            if (!visited[m.index(f.vertex(i))]) {
                visited[m.index(f.vertex(i))] = true;
                MeshPos pos(&f, i);
                uint    starSize = pos.numberOfAdjacentFacesToV();
                if (starSize != TD[m.index(f.vertex(i))])
                    nonManifoldVertices[m.index(f.vertex(i))] = true;
            }
        }
    }

    return nonManifoldVertices;
}

template<FaceMeshConcept MeshType>
uint numberEdges(
    const MeshType& m,
    uint&           numBoundaryEdges,
    uint&           numNonManifoldEdges)
{
    std::vector<ConstMeshEdgeUtil<MeshType>> edgeVec =
        fillAndSortMeshEdgeUtilVector(m);

    uint numEdges       = 0;
    numBoundaryEdges    = 0;
    numNonManifoldEdges = 0;

    size_t f_on_cur_edge = 1;
    for (size_t i = 0; i < edgeVec.size(); ++i) {
        if (((i + 1) == edgeVec.size()) || !(edgeVec[i] == edgeVec[i + 1])) {
            ++numEdges;
            if (f_on_cur_edge == 1)
                ++numBoundaryEdges;
            if (f_on_cur_edge > 2)
                ++numNonManifoldEdges;
            f_on_cur_edge = 1;
        }
        else {
            ++f_on_cur_edge;
        }
    }
    return numEdges;
}

inline std::list<uint>                             dummyUintList;
inline std::list<std::list<std::pair<uint, uint>>> dummyListOfLists;
inline std::vector<std::pair<uint, uint>>          dummyVectorOfPairs;

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
 *
 * @ingroup mesh_stat
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
 *
 * @ingroup mesh_stat
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
 *
 * @ingroup mesh_stat
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
 *
 * @ingroup mesh_stat
 */
template<FaceMeshConcept MeshType>
uint countVerticesToDuplicateByWedgeTexCoords(
    const MeshType&                     mesh,
    std::vector<std::pair<uint, uint>>& vertWedgeMap =
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
 *
 * @ingroup mesh_stat
 * @ingroup clean
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
 * @brief Returns the number of non-deleted unreferenced vertices of the mesh.
 *
 * This function calculates the number of vertices that are not referenced by
 * any of the elements of the mesh, and which have not been marked as deleted.
 *
 * @tparam MeshType: the type of the input Mesh. It must satisfy the
 * MeshConcept.
 *
 * @param[in] m: The input mesh for which to calculate the number of
 * unreferenced vertices.
 * @return The number of non-deleted unreferenced vertices in the mesh.
 *
 * @ingroup mesh_stat
 * @ingroup clean
 */
template<MeshConcept MeshType>
uint numberUnreferencedVertices(const MeshType& m)
{
    uint nV = 0;
    // store the number of unref vertices into nV
    referencedVertices<std::vector<bool>>(m, nV);

    return nV;
}

/**
 * @brief Counts the number of non-manifold vertices in the input mesh.
 *
 * This function counts the number of vertices in the input mesh that are
 * non-manifold, meaning that they are connected to more than two faces. A
 * non-manifold vertex is one that belongs to two or more different edges that
 * are not part of the same face.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: The input mesh for which to count the number of non-manifold
 * vertices. This mesh will not be modified by the function.
 *
 * @return The number of non-manifold vertices in the input mesh.
 *
 * @ingroup mesh_stat
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint numberNonManifoldVertices(const MeshType& m)
{
    std::vector<bool> nonManifoldVertices =
        detail::nonManifoldVerticesVectorBool(m);
    return std::count(
        nonManifoldVertices.begin(), nonManifoldVertices.end(), true);
}

/**
 * @brief Determines whether the input mesh is water tight.
 *
 * This function performs a simple test of water tightness on the input mesh,
 * checking that there are no boundary and no non-manifold edges, assuming that
 * the mesh is orientable. It could be debated whether a closed non-orientable
 * surface is water tight or not, but this function does not take orientability
 * into account.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m The input mesh to check for water tightness.
 *
 * @return `true` if the input mesh is water tight (i.e., closed and manifold),
 * `false` otherwise.
 *
 * @ingroup mesh_stat
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
bool isWaterTight(const MeshType& m)
{
    uint numEdgeBorder, numNonManifoldEdges;
    detail::numberEdges(m, numEdgeBorder, numNonManifoldEdges);
    return numEdgeBorder == 0 && numNonManifoldEdges == 0;
}

/**
 * @brief Counts the number of holes in the input mesh.
 *
 * This function counts the number of holes in the input mesh, where a hole is
 * defined as a closed loop of border edges. The function uses a depth-first
 * search algorithm to traverse the mesh and find all the holes. The function
 * requires the input MeshType to have per-face adjacent faces, and uses the
 * `vcl::requirePerFaceAdjacentFaces` function to enforce this requirement.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * FaceMeshConcept and have per-face adjacent faces.
 *
 * @param[in] m: The input mesh for which to count the number of holes.
 *
 * @return The number of holes in the input mesh.
 *
 * @ingroup mesh_stat
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint numberHoles(const MeshType& m) requires HasPerFaceAdjacentFaces<MeshType>
{
    requirePerFaceAdjacentFaces(m);

    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    uint loopNum = 0;

    // create a vector of bools to keep track of visited faces.
    std::vector<bool> visitedFaces(m.faceContainerSize(), false);

    // Traverse the mesh using a depth-first search algorithm to find all the
    // holes.
    for (const FaceType& f : m.faces()) {
        uint e = 0;
        for (const VertexType* v : f.vertices()) {
            if (!visitedFaces[m.index(f)] && f.adjFace(e) == nullptr) {
                MeshPos<FaceType> startPos(&f, e);
                MeshPos<FaceType> curPos = startPos;
                do {
                    curPos.nextEdgeOnBorderAdjacentToV();
                    curPos.flipVertex();
                    visitedFaces[m.index(curPos.face())] = true;
                } while (curPos != startPos);
                ++loopNum;
            }
            ++e;
        }
    }
    return loopNum;
}

/**
 * @brief Computes the connected components of the input mesh based on its
 * topology.
 *
 * This function computes the connected components of the input mesh based on
 * its topology, and returns a vector of sets, where each set represents a
 * connected component and contains the face indices of the mesh that compose
 * it. The function uses a depth-first search algorithm to traverse the mesh and
 * find the connected components. The function requires the input MeshType to
 * have per-face adjacent faces, and uses the `vcl::requirePerFaceAdjacentFaces`
 * function to enforce this requirement.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * FaceMeshConcept and have per-face adjacent faces.
 *
 * @param[in] m: The input mesh for which to compute the connected components.
 * @return A vector of sets representing the connected components of the input
 * mesh. Each set contains the face indices of the mesh that compose a connected
 * component.
 *
 * @ingroup mesh_stat
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
std::vector<std::set<uint>> connectedComponents(const MeshType& m)
    requires HasPerFaceAdjacentFaces<MeshType>
{
    requirePerFaceAdjacentFaces(m);

    using FaceType = MeshType::FaceType;

    std::vector<std::set<uint>> cc;

    // create a vector of bools to keep track of visited faces.
    std::vector<bool> visitedFaces(m.faceContainerSize(), false);

    // create a stack to hold the faces that need to be visited during the
    // depth-first search.
    std::stack<const FaceType*> sf;

    // traverse the mesh using a depth-first search algorithm to find the
    // connected components.
    for (const FaceType& f : m.faces()) {
        if (!visitedFaces[m.index(f)]) { // first time I see this face
            visitedFaces[m.index(f)] = true;

            // new connected component
            cc.emplace_back();
            std::set<uint>& ccf = cc[cc.size() - 1];
            ccf.insert(m.index(f));

            // while the stack is empty, visit the adjacent faces of the top
            // face of the stack
            sf.push(&f);
            while (!sf.empty()) {
                const FaceType* fpt = sf.top();
                // remove the top face and add it to the connected component
                sf.pop();
                ccf.insert(m.index(fpt));

                // add the adjacent faces of the current visited in the stack
                for (uint j = 0; j < fpt->vertexNumber(); ++j) {
                    const FaceType* adjf = fpt->adjFace(j);
                    // if there is an adj face and it has not been visited
                    if (adjf != nullptr && !visitedFaces[m.index(adjf)]) {
                        sf.push(adjf);
                        visitedFaces[m.index(adjf)] = true;
                    }
                }
            }
        }
    }
    return cc;
}

/**
 * @brief Computes the number of connected components of the input mesh based on
 * its topology.
 *
 * This function computes the number of connected components of the input mesh
 * based on its topology, and returns the result as an unsigned integer. The
 * function simply calls the `connectedComponents` function to compute the
 * connected components and then returns the size of the resulting vector.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the
 * FaceMeshConcept and have per-face adjacent faces.
 *
 * @param[in] m: The input mesh for which to compute the number of connected
 * components.
 * @return The number of connected components of the input mesh.
 *
 * @ingroup mesh_stat
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint numberConnectedComponents(const MeshType& m)
{
    return connectedComponents(m).size();
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H
