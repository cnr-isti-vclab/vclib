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

#ifndef VCL_MESH_UTILS_MESH_POS_H
#define VCL_MESH_UTILS_MESH_POS_H

#include <vclib/mesh/components/adjacent_faces.h>
#include <vclib/mesh/elements/face.h>

namespace vcl {

/**
 * @brief The MeshPos class describes a "Position in a Mesh" that can be
 * identified with a triplet of Face-Vertex-Edge, where:
 * - the Vertex is part of the Face;
 * - the edge is an index less than the number of vertices of the Face
 * - The Vertex is part of the Edge in the Face.
 *
 * Its utility is to navigate the topology of the Mesh trough a set of
 * well-defined operations that allow to "move" the MeshPos object.
 *
 * To be used, the MeshPos requires that the Mesh on which the given
 * Face-Vertex-Edge triplet is defined, has per Face Adjacent Faces topology
 * informtion.
 */
template<face::HasAdjacentFaces FaceType>
class MeshPos
{
    const FaceType* mFace = nullptr;

    const typename FaceType::VertexType* mVertex = nullptr;

    short mEdge = -1;

public:
    using VertexType = FaceType::VertexType;

    /**
     * @brief Empty constructor that creates a null (invalid) MeshPos.
     */
    MeshPos() = default;

    MeshPos(const FaceType* f, short e) : mFace(f), mEdge(e)
    {
        if (f != nullptr)
            mVertex = f->vertex(e);
        assert(isValid(f, v, e));
    }

    MeshPos(const FaceType* f, const VertexType* v) : mFace(f), mVertex(v)
    {
        for (uint i = 0; i < f->vertexNumber(); i++)
            if (f->vertex(i) == v)
                mEdge = i;
        assert(isValid(f, v, e));
    }

    /**
     * @brief Constructor that creates a MeshPos with the given facem vertex and
     * edge. The given triplet **must describe a valid MeshPos**. The
     * constructor asserts this condition.
     *
     * @param[in] f: the Face pointer on which place the MeshPos.
     * @param[in] v: the Vertex pointer on which place the MeshPos.
     * @param[in] e: the Edge (an positive index < f.vertexNumber() on which
     * place the MeshPos.
     */
    MeshPos(const FaceType* f, const VertexType* v, short e) :
            mFace(f), mVertex(v), mEdge(e)
    {
        assert(isValid(f, v, e));
    }

    /**
     * @brief Helper function to check if a MeshPos is valid, that is if:
     * - v and f are valid vertex/face pointers, and e >= 0
     * - the type of f has AdjacentFaces
     * - e is less than the number of vertices of f (that is the number of edges
     *   of f)
     * - v is the vertex of f in position e or (e+1)%f->numberVertices()
     *
     * @param f: a face pointer
     * @param v: a vertex pointer
     * @param e: an integer that represents the edge position in the face f
     * @return `true` if the triple (f,v,e) is a valid MeshPos.
     */
    static bool isValid(const FaceType* f, const VertexType* v, short e)
    {
        if (f == nullptr || v == nullptr || e < 0)
            return false;
        if (!comp::isAdjacentFacesAvailableOn(*f))
            return false;
        return (ushort) e < f->vertexNumber() &&
               (v == f->vertex(e) || v == f->vertexMod(e + 1));
    }

    const FaceType* face() const { return mFace; }

    const VertexType* vertex() const { return mVertex; }

    short edge() const { return mEdge; }

    const FaceType* adjFace() const { return mFace->adjFace(mEdge); }

    const VertexType* adjVertex() const
    {
        MeshPos<FaceType> tmpPos = *this;
        tmpPos.flipVertex();
        return tmpPos.vertex();
    }

    short adjEdge() const
    {
        MeshPos<FaceType> tmpPos = *this;
        tmpPos.flipEdge();
        return tmpPos.edge();
    }

    /**
     * @brief Returns `true` if this MeshPos is valid.
     * @return `true` if this MeshPos is valid.
     */
    bool isValid() const { return isValid(mFace, mVertex, mEdge); }

    /**
     * @brief Returns `true` if this is null, non initialized, MeshPos. The
     * result of this function is different from calling !isValid().
     * @return `true` if this MeshPos is null.
     */
    bool isNull() const
    {
        return mFace == nullptr || mVertex == nullptr || mEdge < 0;
    }

    /**
     * @brief Returns `true` if the current edge of this MeshPos is on a border.
     * To check if is on border, this function checks wheter exists an adjacent
     * face to the current face along the current edge of this MeshPos. It does
     * not use border flags.
     * @return `true` if the current edge in the current face is on a border.
     */
    bool isEdgeOnBorder() const { return mFace->adjFace(mEdge) == nullptr; }

    /**
     * @brief Returns `true` if the current vertex of the MeshPos corresponts to
     * the first vertex of the current edge. If this member function returns
     * `true`, the next operations performed on the MeshPos will move in
     * CounderClockWise order.
     *
     * @return
     */
    bool isCCWOriented() const { return mFace->vertex(mEdge) == mVertex; }

    /**
     * @brief Moves this MeshPos to the face adjacent to the current face that
     * shares the same vertex and the same edge of this MeshPos. The function
     * returns `true` if the current face is changed. If the adjacent face does
     * not exist because the edge of the current face is on border, the current
     * face does not change, and the MeshPos remains valid. In this case, the
     * function returns `false`.
     *
     * @return `true` if the current face is changed, `false` otherwise (because
     * the current edge is on border).
     */
    bool flipFace()
    {
        const FaceType* nf = mFace->adjFace(mEdge);
        if (nf != nullptr) {
            mEdge = nf->indexOfAdjFace(mFace);
            mFace = nf;
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief Moves this MeshPos to the vertex adjacent to the current vertex
     * that shares the same face and the same edge of this MeshPos.
     */
    void flipVertex()
    {
        if (mFace->vertexMod(mEdge) == mVertex) {
            mVertex = mFace->vertexMod(mEdge + 1);
        }
        else {
            mVertex = mFace->vertexMod(mEdge);
        }
    }

    /**
     * @brief Moves this MeshPos to the edge adjacent to the current edge that
     * shares the same face and the same vertex of this MeshPos.
     */
    void flipEdge()
    {
        if (mFace->vertexMod(mEdge + 1) == mVertex) {
            mEdge = (mEdge + 1) % (short) mFace->vertexNumber();
        }
        else {
            short n = mFace->vertexNumber();
            mEdge   = ((mEdge - 1) % n + n) %
                    n; // be sure to get the right index of the previous edge
        }
    }

    /**
     * @brief Moves this MeshPos to the next edge that is adjacent to the
     * current vertex of the MeshPos. This Move corresponds to make a
     * `flipEdge()` and then a `flipFace()` moves. Using this move, it is
     * possible to visit the next face and the next edge adjacent to the current
     * vertex, and can be used to navigate in the star of the current vertex.
     *
     * Note that if a "next face" does not exists because we are on border, the
     * MeshPos flips only the current edge, staying on the same face (see the
     * `flipFace()` function for more details).
     */
    void nextEdgeAdjacentToV()
    {
        flipEdge();
        flipFace();
    }

    /**
     * @brief Moves the MeshPos to the next edge on border that is adjacent to
     * the current vertex of the MeshPos. Basically, cycles on the edges
     * adjacent to the current vertex, and stops when finds a border edge.
     *
     * @return `true` if it finds a border edge that is different to the current
     * one. Return `false` if no border edge was found.
     */
    bool nextEdgeOnBorderAdjacentToV()
    {
        MeshPos<FaceType> startPos = *this;
        do {
            nextEdgeAdjacentToV();
        } while (*this != startPos && !isEdgeOnBorder());
        return (*this != startPos);
    }

    /**
     * @brief Returns the number of adjacent faces to the current vertex of this
     * MeshPos. This works also for vertices that are on a border, and the
     * starting edge and face is ininfluent. The only requirement is that this
     * MeshPos is valid.
     * @return The number of adjacent Faces to the current vertex of this
     * MeshPos.
     */
    uint numberOfAdjacentFacesToV() const
    {
        bool onBorder = false;
        uint count    = countAdjacentFacesToV(onBorder);
        // if we visited a border, it means that we visited the all faces
        // adjacent to v twice to reach the same starting MeshPos.
        if (onBorder)
            count /= 2;
        return count;
    }

    bool operator==(const MeshPos& op) const
    {
        return mFace == op.mFace && mVertex == op.mVertex && mEdge == op.mEdge;
    }

    bool operator!=(const MeshPos& op) const { return !(*this == op); }

    bool operator<(const MeshPos& op) const
    {
        if (mFace == op.mFace) {
            if (mEdge == op.mEdge)
                return mVertex < op.mVertex;
            else
                return mEdge < op.mEdge;
        }
        else {
            return mFace < op.mFace;
        }
    }

private:
    uint countAdjacentFacesToV(bool& onBorder) const
    {
        uint count = 0;
        onBorder   = false;
        // start from this MeshPos
        MeshPos<FaceType> mp = *this;
        do {
            // go to the next edge in the star of v (if face is on border, will
            // change just edge)
            mp.nextEdgeAdjacentToV();
            ++count;
            // if we are visiting a border, it means that we will start to
            // navigate in the opposite sense as we were before.
            if (mp.isEdgeOnBorder()) // flag that we visited a border
                onBorder = true;
        }
        // we end when we will reach the same pos (same triplet f-v-e) of start
        while (mp != *this);
        return count;
    }
};

} // namespace vcl

#endif // VCL_MESH_UTILS_MESH_POS_H
