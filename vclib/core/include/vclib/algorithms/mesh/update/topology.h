// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_TOPOLOGY_H
#define VCL_ALGORITHMS_MESH_UPDATE_TOPOLOGY_H

#include <vclib/algorithms/mesh/sort.h>

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Clears the adjacent faces of each element having ELEM_ID in the mesh.
 *
 * If the number of adjacent faces is dynamic (i.e., the case of the Vertex),
 * each element will have 0 adjacent faces at the end of this function.
 *
 * If the number of adjacent faces is tied to the number of vertices of the
 * element (i.e., the case of the Face), each element will have
 * f->vertexCount() adjacent faces set to nullptr at the end of this
 * function.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * adjacent faces available.
 *
 * @param[in/out] mesh: the mesh on which clear the per element adjacent faces.
 */
template<ElemId ELEM_ID, FaceMeshConcept MeshType>
void clearElementAdjacentFaces(MeshType& mesh)
{
    requireElementComponent<ELEM_ID, CompId::ADJACENT_FACES>(mesh);

    using AdjacentFacesType = comp::ComponentTypeT<
        CompId::ADJACENT_FACES,
        typename MeshType::template ElementType<ELEM_ID>::Components>;

    for (auto e : mesh.template elements<ELEM_ID>()) {
        if constexpr (comp::IsTiedToVertexCount<AdjacentFacesType>) {
            for (uint i = 0; i < e.adjFaceCount(); ++i) {
                e.setAdjFace(i, MeshType::NULL_INDEX);
            }
        }
        else {
            e.clearAdjFaces();
        }
    }
}

/**
 * @brief Clears the adjacent edges of each element having ELEM_ID in the mesh.
 *
 * If the number of adjacent edges is dynamic (i.e., the case of the Vertex),
 * each element will have 0 adjacent edges at the end of this function.
 *
 * If the number of adjacent edges is tied to the number of vertices of the
 * element, each element will have e->vertexCount() adjacent edges set to
 * nullptr at the end of this function.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * adjacent edges available.
 *
 * @param[in/out] mesh: the mesh on which clear the per element adjacent edges.
 */
template<ElemId ELEM_ID, EdgeMeshConcept MeshType>
void clearElementAdjacentEdges(MeshType& mesh)
{
    requireElementComponent<ELEM_ID, CompId::ADJACENT_EDGES>(mesh);

    using AdjacentEdgesType = comp::ComponentTypeT<
        CompId::ADJACENT_EDGES,
        typename MeshType::template ElementType<ELEM_ID>::Components>;

    for (auto e : mesh.template elements<ELEM_ID>()) {
        if constexpr (comp::IsTiedToVertexCount<AdjacentEdgesType>) {
            for (uint i = 0; i < e.adjEdgeCount(); ++i) {
                e.setAdjEdge(i, MeshType::NULL_INDEX);
            }
        }
        else {
            e.clearAdjEdges();
        }
    }
}

/**
 * @brief Clears the adjacent vertices of each element having ELEM_ID in the
 * mesh.
 *
 * Each element will have 0 adjacent vertices at the end of this function.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * adjacent vertices available.
 *
 * @param[in/out] mesh: the mesh on which clear the per element adjacent
 * vertices.
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void clearElementAdjacentVertices(MeshType& mesh)
{
    requireElementComponent<ELEM_ID, CompId::ADJACENT_VERTICES>(mesh);

    for (auto e : mesh.template elements<ELEM_ID>()) {
        e.clearAdjVertices();
    }
}

/**
 * @brief Clears the adjacent faces of each vertex of the mesh.
 *
 * Since the number of adjacent faces per vertex is dynamic, at the end of this
 * function each vertex will have 0 adjacent Faces.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent faces available.
 *
 * @param[in/out] mesh: the mesh on which clear the per vertex adjacent faces.
 */
template<FaceMeshConcept MeshType>
void clearVertexAdjacentFaces(MeshType& mesh)
{
    clearElementAdjacentFaces<ElemId::VERTEX>(mesh);
}

/**
 * @brief Updates the adjacent faces of each vertex of the mesh.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent faces available.
 *
 * @param[in/out] mesh:  the mesh on which update the per vertex adjacent faces.
 */
template<FaceMeshConcept MeshType>
void updateVertexAdjacentFaces(MeshType& mesh)
{
    clearVertexAdjacentFaces(mesh);

    for (auto v : mesh.vertices()) {
        v.clearAdjFaces();
    }

    for (auto f : mesh.faces()) {
        for (auto v : f.vertices()) {
            v.pushAdjFace(f);
        }
    }
}

/**
 * @brief Clears the adjacent vertices of each vertex of the mesh.
 *
 * Since the number of adjacent vertices per vertex is dynamic, at the end of
 * this function each vertex will have 0 adjacent vertices.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent vertices available.
 *
 * @param[in/out] mesh: the mesh on which clear the per vertex adjacent
 * vertices.
 */
template<MeshConcept MeshType>
void clearVertexAdjacentVertices(MeshType& mesh)
{
    clearElementAdjacentVertices<ElemId::VERTEX>(mesh);
}

/**
 * @brief Updates the adjacent vertices of each vertex of the mesh.
 *
 * If includeOnlyFaces is true, only the faces are considered to find adjacent
 * vertices. Otherwise, also other elements (like edges) are considered.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent vertices available.
 *
 * @param[in/out] mesh: the mesh on which update the per vertex adjacent faces.
 */
template<MeshConcept MeshType>
void updateVertexAdjacentVertices(MeshType& mesh, bool includeOnlyFaces = false)
{
    clearVertexAdjacentVertices(mesh);

    using IndexType = MeshType::IndexType;

    if constexpr (FaceMeshConcept<MeshType>) {
        // vector that contains edges sorted trough unordered vertex pointers
        // it contains clusters of "same" edges, but each one of them has its
        // face pointer note that in case on non-manifold mesh, clusters may be
        // of size
        // >= 2
        std::vector<MeshEdgeUtil<MeshType>> vec =
            fillAndSortMeshEdgeUtilVector(mesh);

        // store the last pair of vertices
        IndexType v1 = MeshType::NULL_INDEX;
        IndexType v2 = MeshType::NULL_INDEX;
        for (uint i = 0; i < vec.size(); ++i) {
            // if this pair is different from the last pair
            if (vec[i].v[0] != v1 || vec[i].v[1] != v2) {
                // update last pair
                v1 = vec[i].v[0];
                v2 = vec[i].v[1];
                mesh.vertex(v1).pushAdjVertex(v2); // set the pair as adjacent
                mesh.vertex(v2).pushAdjVertex(v1);
            }
        }
    }

    if (!includeOnlyFaces) {
        if constexpr (EdgeMeshConcept<MeshType>) {
            for (auto e : mesh.edges()) {
                // set the pair as adjacent only if not already present
                if (!e.vertex(0).containsAdjVertex(e.vertex(1)))
                    e.vertex(0).pushAdjVertex(e.vertex(1));
                if (!e.vertex(1).containsAdjVertex(e.vertex(0)))
                    e.vertex(1).pushAdjVertex(e.vertex(0));
            }
        }
    }
}

/**
 * @brief Clears the adjacent edges of each vertex of the mesh.
 *
 * Since the number of adjacent edges per vertex is dynamic, at the end of this
 * function each vertex will have 0 adjacent Edges.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent edges available.
 *
 * @param[in/out] mesh: the mesh on which clear the per vertex adjacent edges.
 */
template<EdgeMeshConcept MeshType>
void clearVertexAdjacentEdges(MeshType& mesh)
{
    clearElementAdjacentEdges<ElemId::VERTEX>(mesh);
}

/**
 * @brief Updates the adjacent edges of each vertex of the mesh.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent edges available.
 *
 * @param[in/out] mesh:  the mesh on which update the per vertex adjacent edges.
 */
template<EdgeMeshConcept MeshType>
void updateVertexAdjacentEdges(MeshType& mesh)
{
    clearVertexAdjacentEdges(mesh);

    for (auto e : mesh.edges()) {
        e.vertex(0).pushAdjEdge(e);
        e.vertex(1).pushAdjEdge(e);
    }
}

/**
 * @brief Clears the adjacent faces of each face of the mesh.
 *
 * Since the number of adjacent faces per face is tied to the number of vertices
 * of the face, at the end of this function each face will have
 * f->vertexCount() adjacent faces set to nullptr.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * adjacent faces available.
 *
 * @param[in/out] mesh:  the mesh on which clear the per face adjacent faces.
 */
template<FaceMeshConcept MeshType>
void clearFaceAdjacentFaces(MeshType& mesh)
{
    clearElementAdjacentFaces<ElemId::FACE>(mesh);
}

/**
 * @brief Updates the per face adjacent face component.
 *
 * All the faces that does not have an adjacent face (border) will have the
 * adjacent face set to nullptr.
 *
 * If there are non-manifold edges (edges on which there are more than two
 * incident faces), then a chain-link composed of all the incident faces is
 * built.
 *
 * Assuming that we have 3 faces f0, f1 and f2 on the same edge composed of vi
 * and vj, we can define the edge indices in the three faces as:
 *
 * @code{.cpp}
 * e0 = f0.indexOfEdge(vi, vj);
 * e1 = f1.indexOfEdge(vi, vj);
 * e2 = f2.indexOfEdge(vi, vj);
 * @endcode
 *
 * The adjacencies will cycle like:
 *
 * @code{.cpp}
 * f0.adjFace(e0) == f1;
 * f1.adjFace(e1) == f2;
 * f2.adjFace(e2) == f0;
 * @endcode
 *
 * or a similar permutation of the faces.
 *
 * In general, an edge is non-manifold if it is true the expression:
 *
 * @code{.cpp}
 * fj = fi.adjFace(ei);
 * fj.adjFace(ej) != fi; // if true, the edge is non-manifold
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * adjacent faces available.
 *
 * @param[in/out] mesh:  the mesh on which update the per face adjacent faces.
 */
template<FaceMeshConcept MeshType>
void updateFaceAdjacentFaces(MeshType& mesh)
{
    requireFaceAdjacentFaces(mesh);

    // vector that contains edges sorted trough unordered vertex pointers
    // it contains clusters of "same" edges, but each one of them has its face
    // pointer. note that in case on non-manifold mesh, clusters may be of size
    // >= 2
    std::vector<MeshEdgeUtil<MeshType>> vec =
        fillAndSortMeshEdgeUtilVector(mesh);

    if (vec.size() > 0) {
        // in this loop, base will point to the first element of a cluster of
        // edges
        // increment of clusters into loop
        for (auto base = vec.begin(); base != vec.end();) {
            auto first = base; // remember the first to set adj to the last

            // i and j will increment together, and if i == j, i will be adj to
            // j, but j will not be adj to i (to manage non manifold edges and
            // make cyclic adj on the same edge)
            auto i = base;
            auto j = i + 1;
            if (j == vec.end() || *i != *j) {
                // case of cluster composed of one element. adj of i is
                // NULL_INDEX
                mesh.face(i->f).setAdjFace(i->e, MeshType::NULL_INDEX);
            }
            else { // at least two edges in the cluster
                while (j != vec.end() && *i == *j) {
                    mesh.face(i->f).setAdjFace(i->e, j->f);
                    ++i;
                    ++j;
                }
                // i now is the last element that was equal to first
                mesh.face(i->f).setAdjFace(i->e, first->f);
            }

            // j is the first different edge from first (or it is vec.end()!)
            base = j;
        }
    }
}

/**
 * @brief Clears the adjacent edges of each face of the mesh.
 *
 * Depending on the definition of the mesh, faces can have a number of
 * adjacent edges tied to the number of vertices of the face (e.g., in a
 * polygonal mesh, each face has as many adjacent edges as its vertices),
 * or a dynamic number of adjacent edges (e.g., the number of adjacent edges
 * of a face can be different from the number of its vertices).
 * If the number of adjacent edges is dynamic, each face will have 0 adjacent
 * edges at the end of this function. If the number of adjacent edges is tied to
 * the number of vertices of the face, each face will have f->vertexCount()
 * adjacent edges set to nullptr at the end of this function.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * adjacent edges available.
 *
 * @param[in/out] mesh:  the mesh on which clear the per face adjacent edges.
 */
template<FaceMeshConcept MeshType>
void clearFaceAdjacentEdges(MeshType& mesh) requires EdgeMeshConcept<MeshType>
{
    clearElementAdjacentEdges<ElemId::FACE>(mesh);
}

/**
 * @brief Updates the per face adjacent edge component.
 *
 * An edge is considered adjacent to a face if it shares with the face the two
 * vertices that define the edge.
 *
 * If the adjacent edges component has a number of adjacent edges tied to the
 * number of vertices of the face, then each face will have as many adjacent
 * edges as its vertices. Therefore, if an adjacent edge is not found for a pair
 * of vertices of the face, the corresponding adjacent edge will be set to
 * nullptr. Otherwise, the last edge found for that pair of vertices will be set
 * as the adjacent edge.
 *
 * If the adjacent edges component has a dynamic number of adjacent edges, then
 * each face will have as many adjacent edges as the number of edges found
 * sharing two vertices with the face.
 *
 * @param[in/out] mesh: the mesh on which update the per face adjacent edges.
 */
template<FaceMeshConcept MeshType>
void updateFaceAdjacentEdges(MeshType& mesh) requires EdgeMeshConcept<MeshType>
{
    using FaceType = FaceType<MeshType>;

    using AdjacentEdgesType = comp::
        ComponentTypeT<CompId::ADJACENT_EDGES, typename FaceType::Components>;

    clearFaceAdjacentEdges(mesh);

    // vector that contains edges sorted trough unordered vertex pointers
    std::vector<MeshEdgeUtil<MeshType>> vec =
        fillAndSortMeshEdgeUtilVector(mesh);

    for (auto e : mesh.edges()) {
        MeshEdgeUtil<MeshType> meu(e.vertexIndex(0), e.vertexIndex(1));

        // binary search for the edge in the sorted vector
        auto it = std::lower_bound(vec.begin(), vec.end(), meu);

        while (it != vec.end() && *it == meu) {
            auto f = it->f; // the face adjacent to the edge e
            if constexpr (comp::IsTiedToVertexCount<AdjacentEdgesType>) {
                mesh.face(f).setAdjEdge(it->e, e);
            }
            else {
                mesh.face(f).pushAdjEdge(e);
            }
            ++it;
        }
    }
}

/**
 * @brief Clears the adjacent faces of each edge of the mesh.
 *
 * Since the number of adjacent edges per face is dynamic, at the end of this
 * function each face will have 0 adjacent Edges.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * adjacent faces available.
 *
 * @param[in/out] mesh: the mesh on which clear the per edge adjacent faces.
 */
template<EdgeMeshConcept MeshType>
void clearEdgeAdjacentFaces(MeshType& mesh) requires FaceMeshConcept<MeshType>
{
    clearElementAdjacentFaces<ElemId::EDGE>(mesh);
}

/**
 * @brief Updates the per edge adjacent faces component.
 *
 * A face is considered adjacent to an edge if it shares with the face the two
 * vertices that define the edge.
 *
 * Each edge will have as many adjacent faces as the number of faces found
 * sharing two vertices with the edge.
 *
 * @param[in/out] mesh: the mesh on which update the per edge adjacent faces.
 */
template<EdgeMeshConcept MeshType>
void updateEdgeAdjacentFaces(MeshType& mesh) requires FaceMeshConcept<MeshType>
{
    clearEdgeAdjacentFaces(mesh);

    // vector that contains edges sorted trough unordered vertex pointers
    std::vector<MeshEdgeUtil<MeshType>> vec =
        fillAndSortMeshEdgeUtilVector(mesh);

    for (auto e : mesh.edges()) {
        MeshEdgeUtil<MeshType> meu(e.vertexIndex(0), e.vertexIndex(1));

        // binary search for the edge in the sorted vector
        auto it = std::lower_bound(vec.begin(), vec.end(), meu);

        while (it != vec.end() && *it == meu) {
            auto f = it->f; // the face adjacent to the edge e
            e.pushAdjFace(f);
            ++it;
        }
    }
}

/**
 * @brief Clears the adjacent edges of each edge of the mesh.
 *
 * Since the number of adjacent edges per face is dynamic, at the end of this
 * function each edge will have 0 adjacent Edges.
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * adjacent edges available.
 *
 * @param[in/out] mesh: the mesh on which clear the per edge adjacent edges.
 */
template<EdgeMeshConcept MeshType>
void clearEdgeAdjacentEdges(MeshType& mesh)
{
    clearElementAdjacentEdges<ElemId::EDGE>(mesh);
}

/**
 * @brief Updates the per edge adjacent edges component.
 *
 * A edge is considered adjacent to an edge if they share a vertex.
 *
 * @param[in/out] mesh: the mesh on which update the per edge adjacent faces.
 */
template<EdgeMeshConcept MeshType>
void updateEdgeAdjacentEdges(MeshType& mesh)
{
    using EdgeType = EdgeType<MeshType>;

    // for each vertex, save the edges incident in it
    std::vector<std::list<EdgeType>> edgeMap(mesh.vertexContainerSize());

    clearEdgeAdjacentEdges(mesh);

    for (EdgeType e : mesh.edges()) {
        edgeMap[e.vertexIndex(0)].push_back(e);
        edgeMap[e.vertexIndex(1)].push_back(e);
    }

    for (std::list<EdgeType>& edges : edgeMap) {
        // all the edges in edges are adjacent to each other
        for (auto it1 = edges.begin(); it1 != edges.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != edges.end(); ++it2) {
                it1->pushAdjEdge(it2->index());
                it2->pushAdjEdge(it1->index());
            }
        }
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_TOPOLOGY_H
