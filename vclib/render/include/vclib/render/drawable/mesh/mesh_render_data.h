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
#include <vclib/mesh/requirements.h>
#include <vclib/render/drawable/mesh/mesh_render_info.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>

namespace vcl {

template<typename MeshRenderDerived>
class MeshRenderData
{
    using MRI = MeshRenderInfo;

    // Auxiliary data that can be used by the derived class to properly allocate
    // and fill the buffers

    uint mNumVerts       = 0;
    uint mNumTris        = 0;
    uint mNumEdges       = 0;
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

    MRI::BuffersBitSet mBuffersToFill = MRI::BUFFERS_ALL;

public:
    void update(
        const MeshConcept auto& mesh,
        MRI::BuffersBitSet      buffersToUpdate = MRI::BUFFERS_ALL)
    {
        using enum MRI::Buffers;

        MRI::BuffersBitSet btu = mBuffersToFill & buffersToUpdate;

        // first thing to do
        setAuxiliaryData(mesh, btu);

        // set data for vertices
        setVerticesData(mesh, btu);

        // set data for faces
        setFacesData(mesh, btu);

        // set data for edges
        setEdgesData(mesh, btu);

        // set data for mesh
        setMeshData(mesh, btu);

        // set data for textures
        setTextureData(mesh, btu);
    }

protected:
    MeshRenderData() = default;

    MeshRenderData(MRI::BuffersBitSet buffersToFill) :
            mBuffersToFill(buffersToFill)
    {
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

    // functions that must be used by derived classes to allocate the buffers

    /**
     * @brief Returns the number of vertices that will be used to render the
     * mesh.
     *
     * The number of vertices must be used to compute the size of the buffers
     * that will store the vertex data (coordinates, normals, colors, etc).
     *
     * It can be used along with the functions `fillVertex*` provided by this
     * class. A common workflow is the following:
     *
     * @code{.cpp}
     * uint nv = numVerts();
     * // assuming that the buffer is a vector of floats
     * std::vector<float> vertexCoords(nv * 3);
     * fillVertexCoords(mesh, vertexCoords.data());
     * @encode
     *
     * @note The returned values may be different from the number of vertices
     * in the input mesh. This is because the mesh may have duplicated vertices
     * (e.g., when the mesh has wedge texture coordinates).
     *
     * @note Always check the required buffer size before filling the buffers
     * on the `fill*` functions documentation.
     *
     * @return The number of vertices that will be used to render the mesh.
     */
    uint numVerts() const { return mNumVerts; }

    uint numTris() const { return mNumTris; }

    uint numEdges() const { return mNumEdges; }

    uint numWireframeLines() const { return nWireframeLines; }

    // function that must be implemebted by the derived classes to create the
    // buffers (there will be a compiler error if they are missing):

    // void createVertexCoordsBuffer(const MeshConcept auto&);

    // functions that can be implemented by the derived classes to create the
    // buffers

    void createVertexNormalsBuffer(const MeshConcept auto&) {}

    void createVertexColorsBuffer(const MeshConcept auto&) {}

    void createVertexTexCoordsBuffer(const MeshConcept auto&) {}

    void createWedgeTexCoordsBuffer(const MeshConcept auto&) {}

    void createTriangleIndicesBuffer(const FaceMeshConcept auto&) {};

    void createTriangleNormalsBuffer(const FaceMeshConcept auto&) {}

    void createTriangleColorsBuffer(const FaceMeshConcept auto&) {}

    void createVertexTextureIndicesBuffer(const FaceMeshConcept auto&) {}

    void createWedgeTextureIndicesBuffer(const FaceMeshConcept auto&) {}

    void createWireframeIndicesBuffer(const FaceMeshConcept auto&) {}

    void createEdgeIndicesBuffer(const EdgeMeshConcept auto&) {}

    void createEdgeNormalsBuffer(const EdgeMeshConcept auto&) {}

    void createEdgeColorsBuffer(const EdgeMeshConcept auto&) {}

    void createTextureUnits(const MeshConcept auto&) {}

    void createMeshUniforms(const MeshConcept auto&) {}

    // utility functions to fill the buffers

    void fillVertexCoords(const MeshConcept auto& mesh, auto* data)
    {
        vertexCoordsToBuffer(mesh, data);
        appendDuplicateVertexCoordsToBuffer(mesh, mVertsToDuplicate, data);
    }

    void fillVertexNormals(const MeshConcept auto& mesh, auto* data)
    {
        vertexNormalsToBuffer(mesh, data);
        appendDuplicateVertexNormalsToBuffer(mesh, mVertsToDuplicate, data);
    }

    void fillVertexColors(
        const MeshConcept auto& mesh,
        auto*                   data,
        Color::Format           fmt)
    {
        vertexColorsToBuffer(mesh, data, fmt);
        appendDuplicateVertexColorsToBuffer(mesh, mVertsToDuplicate, data, fmt);
    }

    void fillVertexTexCoords(const MeshConcept auto& mesh, auto* data)
    {
        vertexTexCoordsToBuffer(mesh, data);
        appendDuplicateVertexTexCoordsToBuffer(mesh, mVertsToDuplicate, data);
    }

    void fillWedgeTexCoords(const FaceMeshConcept auto& mesh, auto* data)
    {
        wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
            mesh, mVertWedgeMap, mFacesToReassign, data);
    }

    void fillTriangleIndices(const FaceMeshConcept auto& mesh, auto* data)
    {
        triangulatedFaceIndicesToBuffer(
            mesh, data, mIndexMap, MatrixStorageType::ROW_MAJOR, mNumTris);
        replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
            mesh, mVertsToDuplicate, mFacesToReassign, mIndexMap, data);
    }

    void fillTriangleNormals(const FaceMeshConcept auto& mesh, auto* data)
    {
        triangulatedFaceNormalsToBuffer(
            mesh, data, mIndexMap, MatrixStorageType::ROW_MAJOR);
    }

    void fillTriangleColors(
        const FaceMeshConcept auto& mesh,
        auto*                       data,
        Color::Format               fmt)
    {
        triangulatedFaceColorsToBuffer(mesh, data, mIndexMap, fmt);
    }

    void fillVertexTextureIndices(const FaceMeshConcept auto& mesh, auto* data)
    {
        vertexTexCoordIndicesAsTriangulatedFaceTexCoordIndicesToBuffer(
            mesh, data, mIndexMap);
    }

    void fillWedgeTextureIndices(const FaceMeshConcept auto& mesh, auto* data)
    {
        triangulatedFaceWedgeTexCoordIndicesToBuffer(mesh, data, mIndexMap);
    }

    void fillEdgeIndices(const EdgeMeshConcept auto& mesh, auto* data)
    {
        edgeIndicesToBuffer(mesh, data);
    }

    void fillEdgeNormals(const EdgeMeshConcept auto& mesh, auto* data)
    {
        edgeNormalsToBuffer(mesh, data);
    }

    void fillEdgeColors(
        const EdgeMeshConcept auto& mesh,
        auto*                       data,
        Color::Format               fmt)
    {
        edgeColorsToBuffer(mesh, data, fmt);
    }

    void fillWireframeIndices(const FaceMeshConcept auto& mesh, auto* data)
    {
        wireframeIndicesToBuffer(mesh, data);
    }

private:
    MeshRenderDerived& derived()
    {
        return static_cast<MeshRenderDerived&>(*this);
    }

    const MeshRenderDerived& derived() const
    {
        return static_cast<const MeshRenderDerived&>(*this);
    }

    void setAuxiliaryData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

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

        if constexpr (HasEdges<MeshType>) {
            if (btu[toUnderlying(EDGES)])
                mNumEdges = mesh.edgeNumber();
        }
    }

    void setVerticesData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if (btu[toUnderlying(VERTICES)]) {
            // vertex buffer (coordinates)
            derived().createVertexCoordsBuffer(mesh);
        }

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                if (btu[toUnderlying(VERT_NORMALS)]) {
                    // vertex buffer (normals)
                    derived().createVertexNormalsBuffer(mesh);
                }
            }
        }

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(mesh)) {
                if (btu[toUnderlying(VERT_COLORS)]) {
                    // vertex buffer (colors)
                    derived().createVertexColorsBuffer(mesh);
                }
            }
        }

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                if (btu[toUnderlying(VERT_TEXCOORDS)]) {
                    // vertex buffer (UVs)
                    derived().createVertexTexCoordsBuffer(mesh);
                }
            }
        }
    }

    void setFacesData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            if (btu[toUnderlying(TRIANGLES)]) {
                // triangle index buffer
                derived().createTriangleIndicesBuffer(mesh);
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                    if (btu[toUnderlying(WEDGE_TEXCOORDS)]) {
                        // vertex wedges buffer (duplicated vertices)
                        derived().createWedgeTexCoordsBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                if (vcl::isPerFaceNormalAvailable(mesh)) {
                    if (btu[toUnderlying(TRI_NORMALS)]) {
                        // triangle normal buffer
                        derived().createTriangleNormalsBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceColor<MeshType>) {
                if (vcl::isPerFaceColorAvailable(mesh)) {
                    if (btu[toUnderlying(TRI_COLORS)]) {
                        // triangle color buffer
                        derived().createTriangleColorsBuffer(mesh);
                    }
                }
            }

            // texture indices are stored per face (each face has its own
            // texture index)
            if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                    if (btu[toUnderlying(VERT_TEXCOORDS)]) {
                        // triangle vertex texture indices buffer
                        derived().createVertexTextureIndicesBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                    if (btu[toUnderlying(WEDGE_TEXCOORDS)]) {
                        // triangle wedge texture indices buffer
                        derived().createWedgeTextureIndicesBuffer(mesh);
                    }
                }
            }

            if (btu[toUnderlying(WIREFRAME)]) {
                // wireframe index buffer
                derived().createWireframeIndicesBuffer(mesh);
            }
        }
    }

    void setEdgesData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if constexpr (vcl::HasEdges<MeshType>) {
            if (btu[toUnderlying(EDGES)]) {
                // edge index buffer
                derived().createEdgeIndicesBuffer(mesh);
            }

            if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
                if (vcl::isPerEdgeNormalAvailable(mesh)) {
                    if (btu[toUnderlying(EDGE_NORMALS)]) {
                        // edge normal buffer
                        derived().createEdgeNormalsBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerEdgeColor<MeshType>) {
                if (vcl::isPerEdgeColorAvailable(mesh)) {
                    if (btu[toUnderlying(EDGE_COLORS)]) {
                        // edge color buffer
                        derived().createEdgeColorsBuffer(mesh);
                    }
                }
            }
        }
    }

    void setMeshData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using enum MRI::Buffers;

        if (btu[toUnderlying(MESH_UNIFORMS)]) {
            // mesh uniforms
            derived().createMeshUniforms(mesh);
        }
    }

    void setTextureData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if constexpr (vcl::HasTexturePaths<MeshType>) {
            if (btu[toUnderlying(TEXTURES)]) {
                // textures
                derived().createTextureUnits(mesh);
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H
