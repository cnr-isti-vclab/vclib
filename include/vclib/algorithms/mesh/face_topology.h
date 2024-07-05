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

#ifndef VCL_ALGORITHMS_MESH_FACE_TOPOLOGY_H
#define VCL_ALGORITHMS_MESH_FACE_TOPOLOGY_H

#include <set>

#include <vclib/concepts/mesh.h>
#include <vclib/exceptions/mesh.h>
#include <vclib/mesh/iterators/face/edge_adj_face_iterator.h>
#include <vclib/misc/comparators.h>
#include <vclib/space/complex/mesh_pos.h>
#include <vclib/space/core/polygon.h>
#include <vclib/views/mesh.h>

namespace vcl {

/**
 * @brief Check if an edge in the given face is manifold.
 *
 * An edge is considered manifold if it is either a boundary edge or if it is
 * shared by exactly two faces.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is
 * not available on \p f.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to check.
 * @param[in] edge: The index of the edge to check.
 *
 * @return true if the edge is manifold, false otherwise.
 */
template<FaceConcept FaceType>
bool isFaceManifoldOnEdge(const FaceType& f, uint edge)
    requires comp::HasAdjacentFaces<FaceType>
{
    // Check if the AdjacentFaces component is available for the given face.
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    // Check if the edge is a boundary edge.
    if (f.adjFace(edge) == nullptr) {
        return true;
    }
    else { // Check if the edge is shared by exactly two faces.
        return f.adjFace(edge)->indexOfAdjFace(&f) != UINT_NULL;
    }
}

/**
 * @brief Check if an edge in the given face is on the border.
 *
 * An edge is considered to be on the border if it is not shared by another
 * face.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is
 * not available on \p f.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to check.
 * @param[in] edge: The index of the edge to check.
 *
 * @return true if the edge is on the border, false otherwise.
 */
template<FaceConcept FaceType>
bool isFaceEdgeOnBorder(const FaceType& f, uint edge)
    requires comp::HasAdjacentFaces<FaceType>
{
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    return f->adjFace(edge) == nullptr;
}

/**
 * @brief Returns a boolean value indicating whether the edge flip operation is
 * allowed or not.
 *
 * This function requires AdjacentFaces component, that must be available and
 * computed before calling this function.
 *
 * The function first checks if the specified edge is a boundary edge, in which
 * case the flip operation is not allowed. If the edge is not a boundary edge,
 * the function checks whether the mesh is well-oriented by verifying that the
 * vertices of the edge to be flipped are the same in the adjacent face.
 *
 * Next, the function checks if the flipped edge already exists in the mesh. To
 * do this, the function performs a depth-first search starting from the current
 * face, following the edges that share the opposite vertex to the edge being
 * flipped. The search checks if the vertex on the other end of the flipped edge
 * is already connected to any other face in the mesh. If it is, the flip
 * operation is not allowed.
 *
 * The depth-first search is limited to the faces that share the opposite vertex
 * to the edge being flipped, so it does not perform an exhaustive search of the
 * entire mesh. However, it is sufficient to detect non-manifoldness caused by
 * the flipped edge.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is
 * not available on \p f.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face that contains the edge to flip.
 * @param[in] edge: The index of the edge to flip.
 *
 * @return true if the edge flip is allowed, false otherwise.
 */
template<FaceConcept FaceType>
bool checkFlipEdge(const FaceType& f, uint edge)
    requires comp::HasAdjacentFaces<FaceType>
{
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    using VertexType = FaceType::VertexType;

    if (f.vertexNumber() > 3)
        return false;

    if (vcl::isFaceEdgeOnBorder(f, edge))
        return false;

    const VertexType* v0 = f.vertex(edge);
    const VertexType* v1 = f.vertexMod(edge + 1);

    const FaceType* of = f.adjFace(edge);
    uint            oe = of->indexOfAdjFace(&f);
    assert(oe != UINT_NULL);

    // check if the vertices of the edge are the same
    // e.g. the mesh has to be well oriented
    if (of->vertex(oe) != v1 || of->vertexMod(oe + 1) != v0)
        return false;

    // check if the flipped edge is already present in the mesh
    // f_v2 and of_v2 are the vertices of the new edge
    const VertexType* f_v2  = f.vertexMod(edge + 2);
    const VertexType* of_v2 = of->vertexMod(oe + 2);

    MeshPos<FaceType> pos(&f, f_v2);
    MeshPos<FaceType> startPos = pos;
    // loop in the one ring of f_v2
    do {
        pos.nextEdgeAdjacentToV();
        if (pos.adjVertex() == of_v2)
            return false;
    } while (pos != startPos);

    return true;
}

/**
 * @brief Returns the number of adjacent faces to the given edge of the face f.
 *
 * If the given edge is manifold, the returned number will be 1 (if the edge is
 * on border - just one face) or 2 (two adjacent faces on the edge). If the edge
 * is non manifold, the number of faces adjacent to the given face will be
 * counted.
 *
 * This function requires AdjacentFaces component, that must be available and
 * computed before calling this function.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is
 * not available on \p f.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the face containing the edge
 * @param[in] edge: the index of the edge of the face
 * @return the number of adjacent faces to the given edge of the face \p f
 */
template<FaceConcept FaceType>
uint edgeAdjacentFacesNumber(const FaceType& f, uint edge)
    requires comp::HasAdjacentFaces<FaceType>
{
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    ConstEdgeAdjFaceIterator<FaceType> begin(f, edge), end;
    uint                               cnt = 0;
    for (auto it = begin; it != end; ++it) {
        ++cnt;
    }

    return cnt;
}

/**
 * @brief Returns the number of edges that are on border (no adjacent faces) on
 * the given face.
 *
 * This function requires AdjacentFaces component, that must be available and
 * computed before calling this function.
 *
 * @throws vcl::MissingComponentException if the "AdjacentFaces" component is
 * not available on \p f.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to check for border edges.
 * @return The number of edges on the border of the face.
 */
template<FaceConcept FaceType>
uint faceEdgesOnBorderNumber(const FaceType& f)
    requires comp::HasAdjacentFaces<FaceType>
{
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    uint cnt = 0;
    for (uint i = 0; i < f.vertexNumber(); ++i)
        if (isFaceEdgeOnBorder(f, i))
            cnt++;

    return cnt;
}

/**
 * * @brief Compute the signed dihedral angle between the normals of the given
 * face and its adjacent face on the edge \p e.
 *
 * The angle between the normal is signed according to the concavity/convexity
 * of the dihedral angle: negative if the edge shared between the two faces is
 * concave, positive otherwise. The surface it is assumend to be oriented. It
 * simply use the projection of  the opposite vertex onto the plane of the other
 * one. It does not assume anything on face normals.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f The face for which to compute the dihedral angle on an edge.
 * @param[in] e The index of the edge shared between the two faces.
 * @return The signed dihedral angle between the normals of the given face and
 * its adjacent face on the edge e.
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is
 * not available on \p f.
 */
template<FaceConcept FaceType>
auto faceDihedralAngleOnEdge(const FaceType& f, uint e)
    requires comp::HasAdjacentFaces<FaceType>
{
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    /*
     *     v0 ___________ vf1
     *       |\          |
     *       | e\     f1 |
     *       |    \e1    |
     *       |f     \    |
     *       |        \  |
     *       |__________\|
     *    vf0             v1
     */

    assert(f.adjFace(e) != nullptr);
    const FaceType& f1 = *(f.adjFace(e));

    uint e1 = f1.indexOfAdjFace(&f);
    assert(e1 != UINT_NULL);

    const auto& vf0 = *(f.vertexMod((int) e - 1));
    const auto& vf1 = *(f1.vertexMod(e1 - 1));

    auto n0 = faceNormal(f);
    auto n1 = faceNormal(f1);

    auto off0 = n0 * vf0.coord();
    auto off1 = n1 * vf1.coord();

    auto dist01 = off0 - n0 * vf1.coord();
    auto dist10 = off1 - n1 * vf0.coord();

    auto sign = std::abs(dist01) > std::abs(dist10) ? dist01 : dist10;

    auto angleRad = n0.angle(n1);
    return sign > 0 ? angleRad : -angleRad;
}

/**
 * @brief Detaches the face \p f on the given edge, which consists on updating
 * adjacent faces such that any face that was linking the face \p f won't link
 * it anymore. It manages also non-manifold edges.
 *
 * If the given pair face-edge is on border, nothing is done.
 * If the given pair face-edge is a normal manifold edge, this operation will
 * set nullptr as adjacent face of f (making edge a border) and vice versa on
 * the adjacent face. If the given pair face-edge is a non-manifold edge, the
 * function will "remove" the current face from the ring of faces incident on
 * the edge. The given face f will have the given edge set as a border
 * (nullptr).
 *
 * This function is designed to work with faces that have an available
 * "AdjacentFaces" component. If the component is not available, a
 * MissingComponentException is thrown.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is
 * not available on \p f.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to detach on the given edge.
 * @param[in] edge: The index of the edge to detach the face from.
 */
template<FaceConcept FaceType>
void detachAdjacentFacesOnEdge(FaceType& f, uint edge)
    requires comp::HasAdjacentFaces<FaceType>
{
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    FaceType* nextFace = f.adjFace(edge);

    // if nextFace == nullptr there is nothing to do
    // the face is already detached on the edge
    if (nextFace != nullptr) {
        FaceType*                          prevFace;
        ConstEdgeAdjFaceIterator<FaceType> begin(f, edge), end;
        for (auto it = begin; it != end; ++it) {
            prevFace = *it;
        }

        if (nextFace == prevFace) { // manifold case
            uint en = nextFace->indexOfAdjFace(&f);
            assert(en != UINT_NULL);
            nextFace->setAdjFace(en, nullptr);
        }
        else { // non manifold case
            // the prev face does not have to point to f anymore, but to
            // nextFace
            uint pn = prevFace->indexOfAdjFace(&f);
            assert(pn != UINT_NULL);
            prevFace->setAdjFace(pn, nextFace);
        }
        f.setAdjFace(edge, nullptr);
    }
}

/**
 * @brief Detaches the given face from all its adjacent vertices and adjacent
 * faces.
 *
 * The detachFace function detaches the given face from all its adjacent
 * vertices and adjacent faces. This means that every vertex and face will no
 * longer link to the face \p f as an adjacent face, and \p f will no longer
 * have any adjacent faces (all their values are set to nullptr). The vertices
 * of the face \p f are unchanged.
 *
 * This function is designed to work with faces that have an available "Adjacent
 * Faces" component. If the component is not available, a
 * MissingComponentException is thrown.
 *
 * @throws MissingComponentException if the adjacent faces component is not
 * available on the face.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to detach from its vertices and adjacent faces.
 */
template<FaceConcept FaceType>
void detachFace(FaceType& f) requires comp::HasAdjacentFaces<FaceType>
{
    if (!comp::isAdjacentFacesAvailableOn(f)) {
        throw vcl::MissingComponentException(
            "Face has no Adjacent Faces component.");
    }

    using VertexType = FaceType::VertexType;

    for (uint e = 0; e < f.vertexNumber(); ++e) {
        detachAdjacentFacesOnEdge(f, e);

        // if the vertices have adjacent faces
        if constexpr (comp::HasAdjacentFaces<VertexType>) {
            if (comp::isAdjacentFacesAvailableOn(f.vertex(e))) {
                VertexType* v    = f.vertex(e);
                uint        vpos = v->indexOfAdjFace(&f);
                if (vpos != UINT_NULL) {   // may happen if vertex adj faces are
                                           // not initialized / updated
                    v->eraseAdjFace(vpos); // the vertex v has not anymore the
                                           // adjacent face f
                }
            }
        }
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_FACE_TOPOLOGY_H
