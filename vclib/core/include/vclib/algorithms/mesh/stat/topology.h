// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H
#define VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H

#include "../face_topology.h"
#include "../sort.h"

#include <vclib/mesh.h>
#include <vclib/space/complex.h>

#include <map>
#include <set>
#include <stack>

namespace vcl {

namespace detail {

// function called for the Container Cont of the Mesh
// mesh: the mesh
// refs: vector of bools, one for each vertex of the mesh, to store if the
//       vertex is referenced or not
// nRefs: number of referenced vertices
template<typename Cont>
void setReferencedVertices(const auto& mesh, auto& refs, auto& nRefs)
{
    // check if the Cont container of the Mesh has vertex references
    if constexpr (comp::HasVertexReferences<typename Cont::ElementType>) {
        // if there are still some vertices non-referenced
        if (nRefs < mesh.vertexCount()) {
            constexpr ElemId ELEM_ID = Cont::ElementType::ELEMENT_ID;
            // for each element of the Cont container
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                // for each vertex of the element
                for (auto vi : el.vertexIndices()) {
                    if (vi != mesh.NULL_INDEX && !mesh.vertex(vi).deleted()) {
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
}

// function called for each container of the Mesh, using variadic templates
template<typename... Cont>
void setReferencedVertices(
    const auto& mesh,
    auto&       refs,
    auto&       nRefs,
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
    uint              texCoordIndex;

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
    requires HasFaceAdjacentFaces<MeshType>
{
    requireFaceAdjacentFaces(m);

    std::vector<bool> nonManifoldVertices(m.vertexContainerSize(), false);

    std::vector<uint> TD(m.vertexContainerSize(), 0);
    std::vector<bool> nonManifoldInc(m.vertexContainerSize(), false);
    // First Loop, count how many faces are incident on a vertex and store it in
    // TD, and flag how many vertices are incident on non manifold edges.
    for (const auto& f : m.faces()) {
        for (uint i = 0; i < f.vertexCount(); ++i) {
            auto vi = f.vertexIndex(i);
            if (vi != m.NULL_INDEX) {
                TD[vi]++;
                if (!isFaceManifoldOnEdge(f, i)) {
                    nonManifoldInc[vi] = true;
                    auto viNext        = f.vertexIndexMod(i + 1);
                    if (viNext != m.NULL_INDEX) {
                        nonManifoldInc[viNext] = true;
                    }
                }
            }
        }
    }

    std::vector<bool> visited(m.vertexContainerSize(), false);
    for (const auto& f : m.faces()) {
        for (uint i = 0; i < f.vertexCount(); ++i) {
            auto vi = f.vertexIndex(i);
            if (vi != m.NULL_INDEX && !visited[vi]) {
                visited[vi] = true;
                MeshPos<MeshType> pos(f, i);
                uint              starSize = pos.numberOfAdjacentFacesToV();
                if (starSize != TD[vi])
                    nonManifoldVertices[vi] = true;
            }
        }
    }

    return nonManifoldVertices;
}

template<FaceMeshConcept MeshType>
auto edgeCount(
    const MeshType& m,
    auto&           numBoundaryEdges,
    auto&           numNonManifoldEdges)
{
    using IndexType = typename MeshType::IndexType;

    std::vector<MeshEdgeUtil<MeshType>> edgeVec =
        fillAndSortMeshEdgeUtilVector(m);

    IndexType numEdges  = 0;
    numBoundaryEdges    = 0;
    numNonManifoldEdges = 0;

    std::size_t f_on_cur_edge = 1;
    for (std::size_t i = 0; i < edgeVec.size(); ++i) {
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

} // namespace detail

/**
 * @brief Count the total number of vertex references in the mesh faces.
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
auto faceVertexReferencesCount(const FaceMeshConcept auto& mesh)
{
    using MeshType  = std::remove_cvref_t<decltype(mesh)>;
    using IndexType = typename MeshType::IndexType;

    IndexType nRefs = 0;

    if constexpr (TriangleMeshConcept<MeshType>) {
        return static_cast<IndexType>(mesh.faceCount() * 3);
    }
    else {
        for (const auto& f : mesh.faces()) {
            nRefs += f.vertexCount();
        }
    }

    return nRefs;
}

/**
 * @brief Returns the largest face size in the mesh.
 *
 * If the mesh has static face size, the function returns the static size.
 * Otherwise, the function iterates through all the faces of the mesh to find
 * the largest face size.
 *
 * @param[in] mesh: The input mesh. It must satisfy the FaceMeshConcept.
 * @return The largest face size in the mesh.
 *
 * @ingroup mesh_stat
 */
uint largestFaceSize(const FaceMeshConcept auto& mesh)
{
    using FaceType = FaceType<decltype(mesh)>;

    uint maxFaceSize = 0;

    if constexpr (FaceType::VERTEX_COUNT > 0) {
        return FaceType::VERTEX_COUNT;
    }
    else {
        for (const auto& f : mesh.faces()) {
            maxFaceSize = std::max(maxFaceSize, f.vertexCount());
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
auto triangulatedFaceCount(const FaceMeshConcept auto& mesh)
{
    using MeshType  = std::remove_cvref_t<decltype(mesh)>;
    using IndexType = typename MeshType::IndexType;

    IndexType nTris = 0;

    for (const auto& f : mesh.faces()) {
        nTris += f.vertexCount() - 2;
    }

    return nTris;
}

/**
 * @brief Returns the largest number of vertex adjacent vertices in the
 * mesh.
 *
 * The function requires that vertex adjacent vertices have been
 * precomputed for the mesh (see `vcl::updateVertexAdjacentVertices`).
 *
 * @throws vcl::MissingComponentException if the mesh does not have vertex
 * adjacent vertices available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the MeshConcept.
 * @return The largest number of vertex adjacent vertices in the mesh.
 *
 * @ingroup mesh_stat
 */
uint largestVertexAdjacentVerticesCount(const MeshConcept auto& mesh)
{
    requireVertexAdjacentVertices(mesh);

    uint maxAVC = 0;

    for (const auto& v : mesh.vertices()) {
        maxAVC = std::max(maxAVC, v.adjVertexCount());
    }

    return maxAVC;
}

/**
 * @brief Returns the largest number of element adjacent faces in
 * the mesh.
 *
 * The function requires that element adjacent faces have been
 * precomputed for the mesh (see `vcl::update<Element>AdjacentFaces`).
 *
 * @note If the element has Tied To Vertex Count Adjacent Faces, the returned
 * value is equal to the number of vertices of the element.
 *
 * @throws vcl::MissingComponentException if the mesh does not have element
 * adjacent faces available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the FaceMeshConcept.
 * @return The largest number of element adjacent faces in the mesh.
 *
 * @ingroup mesh_stat
 */
template<ElemId ELEM_ID>
uint largestElementAdjacentFacesCount(const FaceMeshConcept auto& mesh)
{
    requireElementComponent<ELEM_ID, CompId::ADJACENT_FACES>(mesh);

    uint maxAFC = 0;

    for (const auto& e : mesh.template elements<ELEM_ID>()) {
        maxAFC = std::max(maxAFC, e.adjFaceCount());
    }

    return maxAFC;
}

/**
 * @brief Returns the largest number of vertex adjacent faces in the mesh.
 *
 * The function requires that vertex adjacent faces have been
 * precomputed for the mesh (see `vcl::updateVertexAdjacentFaces`).
 *
 * @throws vcl::MissingComponentException if the mesh does not have vertex
 * adjacent faces available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the FaceMeshConcept.
 * @return The largest number of vertex adjacent faces in the mesh.
 *
 * @ingroup mesh_stat
 */
uint largestVertexAdjacentFacesCount(const FaceMeshConcept auto& mesh)
{
    return largestElementAdjacentFacesCount<ElemId::VERTEX>(mesh);
}

/**
 * @brief Returns the largest number of edge adjacent faces in the mesh.
 *
 * The function requires that edge adjacent faces have been
 * precomputed for the mesh.
 *
 * @throws vcl::MissingComponentException if the mesh does not have edge
 * adjacent faces available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the FaceMeshConcept and
 * EdgeMeshConcept.
 * @return The largest number of edge adjacent faces in the mesh.
 *
 * @ingroup mesh_stat
 */
uint largestEdgeAdjacentFacesCount(const FaceMeshConcept auto& mesh)
    requires EdgeMeshConcept<decltype(mesh)>
{
    return largestElementAdjacentFacesCount<ElemId::EDGE>(mesh);
}

/**
 * @brief Returns the largest number of ELEM_ID element adjacent edges in
 * the mesh.
 *
 * The function requires that ELEM_ID element adjacent edges have been
 * precomputed for the mesh (see `vcl::update<Element>AdjacentEdges`).
 *
 * @note If the element has Tied To Vertex Count Adjacent Edges, the returned
 * value is equal to the number of vertices of the element.
 *
 * @throws vcl::MissingComponentException if the mesh does not have element
 * adjacent edges available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the EdgeMeshConcept.
 * @return The largest number of element adjacent edges in the mesh.
 *
 * @ingroup mesh_stat
 */
template<ElemId ELEM_ID>
uint largestElementAdjacentEdgesCount(const EdgeMeshConcept auto& mesh)
{
    requireElementComponent<ELEM_ID, CompId::ADJACENT_EDGES>(mesh);

    uint maxAEC = 0;

    for (const auto& e : mesh.template elements<ELEM_ID>()) {
        maxAEC = std::max(maxAEC, e.adjEdgeCount());
    }

    return maxAEC;
}

/**
 * @brief Returns the largest number of vertex adjacent edges in the mesh.
 *
 * The function requires that vertex adjacent edges have been
 * precomputed for the mesh (see `vcl::updateVertexAdjacentEdges`).
 *
 * @throws vcl::MissingComponentException if the mesh does not have vertex
 * adjacent edges available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the EdgeMeshConcept.
 * @return The largest number of vertex adjacent edges in the mesh.
 *
 * @ingroup mesh_stat
 */
uint largestVertexAdjacentEdgesCount(const EdgeMeshConcept auto& mesh)
{
    return largestElementAdjacentEdgesCount<ElemId::VERTEX>(mesh);
}

/**
 * @brief Returns the largest number of face adjacent edges in the mesh.
 *
 * The function requires that face adjacent edges have been
 * precomputed for the mesh (see `vcl::updateFaceAdjacentEdges`).
 *
 * @throws vcl::MissingComponentException if the mesh does not have face
 * adjacent edges available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the EdgeMeshConcept and
 * FaceMeshConcept.
 * @return The largest number of face adjacent edges in the mesh.
 *
 * @ingroup mesh_stat
 */
uint largestFaceAdjacentEdgesCount(const EdgeMeshConcept auto& mesh)
    requires FaceMeshConcept<decltype(mesh)>
{
    return largestElementAdjacentEdgesCount<ElemId::FACE>(mesh);
}

/**
 * @brief Returns the largest number of edge adjacent edges in the mesh.
 *
 * The function requires that edge adjacent edges have been
 * precomputed for the mesh (see `vcl::updateEdgeAdjacentEdges`).
 *
 * @throws vcl::MissingComponentException if the mesh does not have edge
 * adjacent edges available.
 *
 * @param[in] mesh: The input mesh. It must satisfy the EdgeMeshConcept.
 * @return The largest number of edge adjacent edges in the mesh.
 *
 * @ingroup mesh_stat
 */
uint largestEdgeAdjacentEdgesCount(const EdgeMeshConcept auto& mesh)
{
    return largestElementAdjacentEdgesCount<ElemId::EDGE>(mesh);
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
template<
    FaceMeshConcept MeshType,
    typename VertWedgeMap,
    typename VertsToDuplicate,
    typename FacesToReassign>
auto verticesToDuplicateByWedgeTexCoordsCount(
    const MeshType&   mesh,
    VertWedgeMap&     vertWedgeMap,
    VertsToDuplicate& vertsToDuplicate,
    FacesToReassign&  facesToReassign)
{
    using IndexType = typename MeshType::IndexType;
    vcl::requireFaceWedgeTexCoords(mesh);

    bool hasMaterials = false;
    if constexpr (HasFaceMaterialIndex<MeshType>) {
        hasMaterials = mesh.isFaceMaterialIndexAvailable();
    }

    using WedgeTexCoordType  = TexCoordType<WedgeType<FaceType<MeshType>>>;
    using WedgeTexCoordsInfo = detail::WedgeTexCoordsInfo<WedgeTexCoordType>;

    // list of faces that reference a wedge texcoord, with the index of the
    // vertex in the face
    using FaceList = std::list<std::pair<IndexType, uint>>;

    vertWedgeMap.resize(mesh.vertexContainerSize());
    vertsToDuplicate.clear();
    facesToReassign.clear();

    IndexType count = 0;

    // for each vertex, I'll store a map of WedgeTexCoordsInfo
    // each element of the map represent a unique wedge texcoord(the texcoord
    // itself and the index of the texcoord), and for each element it maps a
    // list of faces that reference the texcoord
    std::vector<std::map<WedgeTexCoordsInfo, FaceList>> wedges(
        mesh.vertexContainerSize());

    for (const auto& f : mesh.faces()) {
        for (uint i = 0; i < f.vertexCount(); ++i) {
            auto vi = f.vertexIndex(i);
            if (vi == mesh.NULL_INDEX) {
                continue;
            }

            // check if the i-th wedge texcoord of the face already exists
            uint matIdx = 0;
            if constexpr (HasFaceMaterialIndex<MeshType>) {
                if (hasMaterials) {
                    matIdx = f.materialIndex();
                }
            }
            WedgeTexCoordsInfo wi = {f.wedge(i).texCoord(), matIdx};
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
    IndexType vi = 0;
    for (auto& map : wedges) {
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
                vertWedgeMap[vi] = {mesh.NULL_INDEX, UINT_NULL};
            }
        }
        ++vi;
    }

    assert(vertWedgeMap.size() == mesh.vertexContainerSize());
    assert(vertsToDuplicate.size() == count);
    assert(facesToReassign.size() == count);

    return count;
}

template<FaceMeshConcept MeshType>
auto verticesToDuplicateByWedgeTexCoordsCount(const MeshType& mesh)
{
    using IndexType = typename MeshType::IndexType;
    std::vector<std::pair<IndexType, uint>>          vertWedgeMap;
    std::list<IndexType>                             vertsToDuplicate;
    std::list<std::list<std::pair<IndexType, uint>>> facesToReassign;
    return verticesToDuplicateByWedgeTexCoordsCount(
        mesh, vertWedgeMap, vertsToDuplicate, facesToReassign);
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
 * @param[in] onlyFaces: If true, only the faces of the mesh are considered,
 * and any other element will not be checked for vertex references.
 * @return a Container of values interpreted as booleans telling, for each
 * vertex of the mesh, if it is referenced.
 *
 * @ingroup mesh_stat
 */
template<
    typename Container = std::vector<bool>,
    MeshConcept MeshType,
    typename Integer = typename MeshType::IndexType>
requires (!std::is_same_v<std::remove_cvref_t<Integer>, bool>)
Container referencedVertices(
    const MeshType& mesh,
    Integer&        nUnref,
    bool            onlyFaces = false)
{
    using IndexType = typename MeshType::IndexType;
    IndexType nRefs = 0;

    Container refVertices(mesh.vertexContainerSize(), false);

    if (onlyFaces) {
        if constexpr (FaceMeshConcept<MeshType>) {
            using FaceContainerType =
                typename MeshType::template ContainerType<ElemId::FACE>;
            detail::setReferencedVertices<FaceContainerType>(
                mesh, refVertices, nRefs);
        }
    }
    else {
        // Iterate over every element container present in this mesh at
        // compile-time and mark the vertices they reference.
        detail::setReferencedVertices(
            mesh, refVertices, nRefs, typename MeshType::ElementContainers());
    }

    nUnref = static_cast<Integer>(mesh.vertexCount() - nRefs);

    return refVertices;
}

template<typename Container = std::vector<bool>, MeshConcept MeshType>
Container referencedVertices(const MeshType& mesh, bool onlyFaces = false)
{
    typename MeshType::IndexType dummy = 0;
    return referencedVertices<Container>(mesh, dummy, onlyFaces);
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
 * @param[in] onlyFaces: If true, only the faces of the mesh are considered,
 * and any other element will not be checked for vertex references.
 * @return The number of non-deleted unreferenced vertices in the mesh.
 *
 * @ingroup mesh_stat
 */
template<MeshConcept MeshType>
auto unreferencedVertexCount(const MeshType& m, bool onlyFaces = false)
{
    using IndexType = typename MeshType::IndexType;
    IndexType nV    = 0;
    // store the number of unref vertices into nV
    referencedVertices<std::vector<bool>>(m, nV, onlyFaces);

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
 */
template<FaceMeshConcept MeshType>
auto nonManifoldVertexCount(const MeshType& m)
{
    using IndexType = typename MeshType::IndexType;
    std::vector<bool> nonManifoldVertices =
        detail::nonManifoldVerticesVectorBool(m);
    return static_cast<IndexType>(std::count(
        nonManifoldVertices.begin(), nonManifoldVertices.end(), true));
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
 */
template<FaceMeshConcept MeshType>
bool isWaterTight(const MeshType& m)
{
    typename MeshType::IndexType numEdgeBorder = 0, numNonManifoldEdges = 0;
    detail::edgeCount(m, numEdgeBorder, numNonManifoldEdges);
    return numEdgeBorder == 0 && numNonManifoldEdges == 0;
}

/**
 * @brief Counts the number of holes in the input mesh.
 *
 * This function counts the number of holes in the input mesh, where a hole is
 * defined as a closed loop of border edges. The function uses a depth-first
 * search algorithm to traverse the mesh and find all the holes. The function
 * requires the input MeshType to have face adjacent faces, and uses the
 * `vcl::requireFaceAdjacentFaces` function to enforce this requirement.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * FaceMeshConcept and have face adjacent faces.
 *
 * @param[in] m: The input mesh for which to count the number of holes.
 *
 * @return The number of holes in the input mesh.
 *
 * @ingroup mesh_stat
 */
template<FaceMeshConcept MeshType>
auto holeCount(const MeshType& m) requires HasFaceAdjacentFaces<MeshType>
{
    requireFaceAdjacentFaces(m);

    using IndexType   = typename MeshType::IndexType;
    IndexType loopNum = 0;

    // create a vector of bools to keep track of visited faces.
    std::vector<bool> visitedFaces(m.faceContainerSize(), false);

    // Traverse the mesh using a depth-first search algorithm to find all the
    // holes.
    for (const auto& f : m.faces()) {
        uint e = 0;
        for (const auto& v : f.vertices()) {
            if (!visitedFaces[f.index()] && !f.adjFace(e)) {
                MeshPos<MeshType> startPos(f, e);
                MeshPos<MeshType> curPos = startPos;
                do {
                    curPos.nextEdgeOnBorderAdjacentToV();
                    curPos.flipVertex();
                    visitedFaces[curPos.face().index()] = true;
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
 * have face adjacent faces, and uses the `vcl::requireFaceAdjacentFaces`
 * function to enforce this requirement.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * FaceMeshConcept and have face adjacent faces.
 *
 * @param[in] m: The input mesh for which to compute the connected components.
 * @return A vector of sets representing the connected components of the input
 * mesh. Each set contains the face indices of the mesh that compose a connected
 * component.
 *
 * @ingroup mesh_stat
 */
template<FaceMeshConcept MeshType>
auto connectedComponents(const MeshType& m)
    requires HasFaceAdjacentFaces<MeshType>
{
    requireFaceAdjacentFaces(m);

    using IndexType = typename MeshType::IndexType;
    using FaceType  = ConstFaceType<MeshType>;

    std::vector<std::set<IndexType>> cc;

    // create a vector of bools to keep track of visited faces.
    std::vector<bool> visitedFaces(m.faceContainerSize(), false);

    // create a stack to hold the faces that need to be visited during the
    // depth-first search.
    std::stack<FaceType> sf;

    // traverse the mesh using a depth-first search algorithm to find the
    // connected components.
    for (auto f : m.faces()) {
        if (!visitedFaces[f.index()]) { // first time I see this face
            visitedFaces[f.index()] = true;

            // new connected component
            cc.emplace_back();
            std::set<IndexType>& ccf = cc.back();
            ccf.insert(f.index());

            // while the stack is empty, visit the adjacent faces of the top
            // face of the stack
            sf.push(f);
            while (!sf.empty()) {
                FaceType fpt = sf.top();
                // remove the top face and add it to the connected component
                sf.pop();
                ccf.insert(fpt.index());

                // add the adjacent faces of the current visited in the stack
                for (uint j = 0; j < fpt.vertexCount(); ++j) {
                    FaceType adjf = fpt.adjFace(j);
                    // if there is an adj face and it has not been visited
                    if (adjf && !visitedFaces[adjf.index()]) {
                        sf.push(adjf);
                        visitedFaces[adjf.index()] = true;
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
 * FaceMeshConcept and have face adjacent faces.
 *
 * @param[in] m: The input mesh for which to compute the number of connected
 * components.
 * @return The number of connected components of the input mesh.
 *
 * @ingroup mesh_stat
 */
template<FaceMeshConcept MeshType>
auto connectedComponentCount(const MeshType& m)
{
    return connectedComponents(m).size();
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_TOPOLOGY_H
