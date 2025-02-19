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

#ifndef VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H
#define VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H

#include <vclib/algorithms/mesh/import_export/append_replace_to_buffer.h>
#include <vclib/algorithms/mesh/import_export/export_buffer.h>
#include <vclib/algorithms/mesh/stat/topology.h>
#include <vclib/render/drawable/mesh/mesh_render_info.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderData
{
    using MRI = MeshRenderInfo;

    uint mNumVerts       = 0;
    uint mNumTris        = 0;
    uint nWireframeLines = 0;

    // vector that tells, for each non-duplicated vertex, which wedges it
    // belongs to each pair is the face index and the wedge index in the face
    // allows to access the wedge texcoords for each non-duplicated vertex
    std::vector<std::pair<uint, uint>> mVertWedgeMap;

    // the list of vertices that has been duplicated (each element of the list
    // is the index of the vertex to duplicate)
    std::list<uint> mVertsToDuplicate;

    // a list that tells, for each duplicated vertex, the list of faces that
    // must be reassigned to the corresponding duplicated vertex
    // each duplicated vertex has a list of pairs face/vertex index in the face
    // that must be/have been reassigned to the duplicated vertex
    std::list<std::list<std::pair<uint, uint>>> mFacesToReassign;

    // data used to manage the mapping beteween the original polygonal faces
    // and the triangle faces

    // map that stores the correspondence between the original polygonal faces
    // and the triangle faces
    TriPolyIndexBiMap mIndexMap;

protected:
    MRI::BuffersBitSet mBuffersToFill = MRI::BUFFERS_ALL;

    MeshRenderData() = default;

    MeshRenderData(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToFill = MRI::BUFFERS_ALL) :
            mBuffersToFill(buffersToFill)
    {
        preUpdate(mesh);
    }

    void swap(MeshRenderData& other)
    {
        using std::swap;
        swap(mNumVerts, other.mNumVerts);
        swap(mNumTris, other.mNumTris);
        swap(mVertWedgeMap, other.mVertWedgeMap);
        swap(mVertsToDuplicate, other.mVertsToDuplicate);
        swap(mFacesToReassign, other.mFacesToReassign);
        swap(mIndexMap, other.mIndexMap);
        swap(mBuffersToFill, other.mBuffersToFill);
    }

    void preUpdate(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToUpdate = MRI::BUFFERS_ALL)
    {
        using enum MRI::Buffers;

        MRI::BuffersBitSet btu = mBuffersToFill & buffersToUpdate;

        if (btu[toUnderlying(VERTICES)] || btu[toUnderlying(WEDGE_TEXCOORDS)] ||
            btu[toUnderlying(TRIANGLES)]) {
            mVertWedgeMap.clear();
            mVertsToDuplicate.clear();
            mFacesToReassign.clear();

            if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                if (mesh.isPerFaceWedgeTexCoordsEnabled()) {
                    countVerticesToDuplicateByWedgeTexCoords(
                        mesh,
                        mVertWedgeMap,
                        mVertsToDuplicate,
                        mFacesToReassign);
                }
            }

            mNumVerts = mesh.vertexNumber() + mVertsToDuplicate.size();
        }

        if constexpr (HasFaces<MeshType>) {
            if (btu[toUnderlying(TRIANGLES)])
                mNumTris = countTriangulatedTriangles(mesh);
            if (btu[toUnderlying(WIREFRAME)])
                nWireframeLines = countPerFaceVertexReferences(mesh);
        }
    }

    uint numVerts() const { return mNumVerts; }

    uint numTris() const { return mNumTris; }

    uint numWireframeLines() const { return nWireframeLines; }

    void fillVertexCoords(const MeshType& mesh, auto* data)
    {
        vertexCoordsToBuffer(mesh, data);
        appendDuplicateVertexCoordsToBuffer(
            mesh, mVertsToDuplicate, data);
    }

    void fillVertexNormals(const MeshType& mesh, auto* data)
    {
        vertexNormalsToBuffer(mesh, data);
        appendDuplicateVertexNormalsToBuffer(
            mesh, mVertsToDuplicate, data);
    }

    void fillVertexColors(const MeshType& mesh, auto* data, Color::Format fmt)
    {
        vertexColorsToBuffer(mesh, data, fmt);
        appendDuplicateVertexColorsToBuffer(
            mesh, mVertsToDuplicate, data, fmt);
    }

    void fillVertexTexCoords(const MeshType& mesh, auto* data)
    {
        vertexTexCoordsToBuffer(mesh, data);
        appendDuplicateVertexTexCoordsToBuffer(
            mesh, mVertsToDuplicate, data);
    }

    void fillWedgeTexCoords(const MeshType& mesh, auto* data)
    {
        wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
            mesh, mVertWedgeMap, mFacesToReassign, data);
    }

    void fillTriangleIndices(const MeshType& mesh, auto* data)
    {
        triangulatedFaceIndicesToBuffer(
            mesh,
            data,
            mIndexMap,
            MatrixStorageType::ROW_MAJOR,
            mNumTris);
        replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
            mesh, mVertsToDuplicate, mFacesToReassign, mIndexMap, data);
    }

    void fillTriangleNormals(const MeshType& mesh, auto* data)
    {
        triangulatedFaceNormalsToBuffer(
            mesh,
            data,
            mIndexMap,
            MatrixStorageType::ROW_MAJOR);
    }

    void fillTriangleColors(const MeshType& mesh, auto* data, Color::Format fmt)
    {
        triangulatedFaceColorsToBuffer(
            mesh, data, mIndexMap, fmt);
    }

    void fillVertexTextureIndices(const MeshType& mesh, auto* data)
    {
        vertexTexCoordIndicesAsTriangulatedFaceWedgeTexCoordIndicesToBuffer(
            mesh, data, mIndexMap);
    }

    void fillWedgeTextureIndices(const MeshType& mesh, auto* data)
    {
        triangulatedFaceWedgeTexCoordIndicesToBuffer(
            mesh, data, mIndexMap);
    }

    void fillEdgeIndices(const MeshType& mesh, auto* data)
    {
        edgeIndicesToBuffer(mesh, data);
    }

    void fillEdgeNormals(const MeshType& mesh, auto* data)
    {
        edgeNormalsToBuffer(mesh, data);
    }

    void fillEdgeColors(const MeshType& mesh, auto* data, Color::Format fmt)
    {
        edgeColorsToBuffer(mesh, data, fmt);
    }

    void fillWireframeIndices(const MeshType& mesh, auto* data)
    {
        wireframeIndicesToBuffer(mesh, data);
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H
