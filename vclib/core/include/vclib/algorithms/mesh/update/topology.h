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
 * f->vertexNumber() adjacent faces set to nullptr at the end of this
 * function.
 *
 * @param m: the mesh on which clear the per element adjacent faces.
 */
template<uint ELEM_ID, MeshConcept MeshType>
void clearPerElementAdjacentFaces(MeshType& m)
{
    requirePerElementComponent<ELEM_ID, CompId::ADJACENT_FACES>(m);

    using AdjacentFacesType = comp::ComponentTypeFromID<
        CompId::ADJACENT_FACES,
        typename MeshType::template ElementType<ELEM_ID>::Components>;

    for (auto& e : m.template elements<ELEM_ID>()) {
        if constexpr (comp::IsTiedToVertexNumber<AdjacentFacesType>) {
            for (uint i = 0; i < e.adjFacesNumber(); ++i) {
                e.setAdjFace(i, nullptr);
            }
        }
        else {
            e.clearAdjFaces();
        }
    }
}

/**
 * @brief Clears the adjacent vertices of each element having ELEM_ID in the
 * mesh.
 *
 * Each element will have 0 adjacent vertices at the end of this function.
 *
 * @param m: the mesh on which clear the per element adjacent vertices.
 */
template<uint ELEM_ID, MeshConcept MeshType>
void clearPerElementAdjacentVertices(MeshType& m)
{
    requirePerElementComponent<ELEM_ID, CompId::ADJACENT_VERTICES>(m);

    for (auto& e : m.template elements<ELEM_ID>()) {
        e.clearAdjVertices();
    }
}

/**
 * @brief Clears the adjacent faces of each vertex of the mesh.
 *
 * Since the number of adjacent faces per vertex is dynamic, at the end of this
 * function each vertex will have 0 adjacent Faces.
 *
 * @param m: the mesh on which clear the per vertex adjacent faces.
 */
template<FaceMeshConcept MeshType>
void clearPerVertexAdjacentFaces(MeshType& m)
{
    clearPerElementAdjacentFaces<ElemId::VERTEX>(m);
}

/**
 * @brief Updates the adjacent faces of each vertex of the mesh.
 *
 * @param m:  the mesh on which update the per vertex adjacent faces.
 */
template<FaceMeshConcept MeshType>
void updatePerVertexAdjacentFaces(MeshType& m)
{
    clearPerVertexAdjacentFaces(m);

    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    for (VertexType& v : m.vertices()) {
        v.clearAdjFaces();
    }

    for (FaceType& f : m.faces()) {
        for (VertexType* v : f.vertices()) {
            v->pushAdjFace(&f);
        }
    }
}

/**
 * @brief Clears the adjacent vertices of each vertex of the mesh.
 *
 * Since the number of adjacent vertices per vertex is dynamic, at the end of
 * this function each vertex will have 0 adjacent vertices.
 *
 * @param m: the mesh on which clear the per vertex adjacent vertices.
 */
template<MeshConcept MeshType>
void clearPerVertexAdjacentVertices(MeshType& m)
{
    clearPerElementAdjacentVertices<ElemId::VERTEX>(m);
}

/**
 * @brief Updates the adjacent vertices of each vertex of the mesh.
 *
 * @param m:  the mesh on which update the per vertex adjacent faces.
 */
template<FaceMeshConcept MeshType>
void updatePerVertexAdjacentVertices(MeshType& m)
{
    clearPerVertexAdjacentVertices(m);

    using VertexType = MeshType::VertexType;

    // vector that contains edges sorted trough unordered vertex pointers
    // it contains clusters of "same" edges, but each one of them has its face
    // pointer note that in case on non-manifold mesh, clusters may be of size
    // >= 2
    std::vector<MeshEdgeUtil<MeshType>> vec = fillAndSortMeshEdgeUtilVector(m);

    // store the last pair of vertices
    VertexType* v1 = nullptr;
    VertexType* v2 = nullptr;
    for (uint i = 0; i < vec.size(); ++i) {
        // if this pair is different from the last pair
        if (vec[i].v[0] != v1 || vec[i].v[1] != v2) {
            // update last pair
            v1 = vec[i].v[0];
            v2 = vec[i].v[1];
            v1->pushAdjVertex(v2); // set the pair as adjacent
            v2->pushAdjVertex(v1);
        }
    }
}

/**
 * @brief Clears the adjacent faces of each face of the mesh.
 *
 * Since the number of adjacent faces per face is tied to the number of vertices
 * of the face, at the end of this function each face will have
 * f->vertexNumber() adjacent faces set to nullptr.
 *
 * @param m:  the mesh on which clear the per face adjacent faces.
 */
template<FaceMeshConcept MeshType>
void clearPerFaceAdjacentFaces(MeshType& m)
{
    clearPerElementAdjacentFaces<ElemId::FACE>(m);
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
 * f0->adjFace(e0) == f1;
 * f1->adjFace(e1) == f2;
 * f2->adjFace(e2) == f0;
 * @endcode
 *
 * or a similar permutation of the faces.
 *
 * In general, an edge is non-manifold if it is true the expression:
 *
 * @code{.cpp}
 * fj = fi->adjFace(ei);
 * fj->adjFace(ej) != fi; // if true, the edge is non-manifold
 * @endcode
 *
 * @param m:  the mesh on which update the per face adjacent faces.
 */
template<FaceMeshConcept MeshType>
void updatePerFaceAdjacentFaces(MeshType& m)
{
    requirePerFaceAdjacentFaces(m);

    // vector that contains edges sorted trough unordered vertex pointers
    // it contains clusters of "same" edges, but each one of them has its face
    // pointer note that in case on non-manifold mesh, clusters may be of size
    // >= 2
    std::vector<MeshEdgeUtil<MeshType>> vec = fillAndSortMeshEdgeUtilVector(m);

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
            if (j != vec.end()) {
                // case of cluster composed of one element. adj of i is nullptr
                if (*i != *j) {
                    i->f->setAdjFace(i->e, nullptr);
                }
                else { // at least two edges in the cluster
                    while (j != vec.end() && *i == *j) {
                        i->f->setAdjFace(i->e, j->f);
                        ++i;
                        ++j;
                    }
                    // i now is the last element that was equal to first
                    i->f->setAdjFace(i->e, first->f);
                }
            }

            // j is the first different edge from first (or it is vec.end()!)
            base = j;
        }
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_TOPOLOGY_H
