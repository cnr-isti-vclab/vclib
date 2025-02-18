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

#include "mesh_render_info.h"

#include <vclib/algorithms/mesh/import_export/append_replace_to_buffer.h>
#include <vclib/algorithms/mesh/import_export/export_buffer.h>
#include <vclib/algorithms/mesh/stat/topology.h>
#include <vclib/math/min_max.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>
#include <vclib/space/core/image.h>
#include <vclib/space/core/texture.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderData
{
    using MRI = MeshRenderInfo;

    MRI::BuffersBitSet mBuffersToFill = MRI::BUFFERS_ALL;

    std::vector<float>    mVerts;
    std::vector<uint32_t> mTris;
    std::vector<uint32_t> mEdges;
    std::vector<uint32_t> mWireframe;

    std::vector<float>    mVNormals;
    std::vector<uint32_t> mVColors;
    std::vector<float>    mVTexCoords;
    std::vector<uint32_t> mVTexIds;

    std::vector<float>    mTNormals;
    std::vector<uint32_t> mTColors;
    std::vector<float>    mWTexCoords;
    std::vector<uint32_t> mWTexIds;

    std::vector<float>    mENormals;
    std::vector<uint32_t> mEColors;

    std::array<float, 4> mMeshColor = {0.5, 0.5, 0.5, 1};

    std::vector<vcl::Image> mTextures;

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

public:
    MeshRenderData() = default;

    MeshRenderData(
        const MeshType&    m,
        MRI::BuffersBitSet buffersToFill = MRI::BUFFERS_ALL) :
            mBuffersToFill(buffersToFill)
    {
        update(m);
    }

    void update(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToUpdate = MRI::BUFFERS_ALL)
    {
        using enum MRI::Buffers;

        MRI::BuffersBitSet btu = mBuffersToFill & buffersToUpdate;

        clear(btu);

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
        }

        if (btu[toUnderlying(VERTICES)]) {
            // vertex buffer (coordinates)
            createVertexCoordsBuffer(mesh);
        }

        if (btu[toUnderlying(VERT_NORMALS)]) {
            // vertex buffer (normals)
            createVertexNormalsBuffer(mesh);
        }

        if (btu[toUnderlying(VERT_COLORS)]) {
            // vertex buffer (colors)
            createVertexColorsBuffer(mesh);
        }

        if (btu[toUnderlying(VERT_TEXCOORDS)]) {
            // vertex buffer (UVs)
            createVertexTexCoordsBuffer(mesh);
        }

        if (btu[toUnderlying(WEDGE_TEXCOORDS)]) {
            // vertex wedges buffer (duplicated vertices)
            createWedgeTexCoordsBuffer(mesh);
        }

        if (btu[toUnderlying(TRIANGLES)]) {
            // triangle index buffer
            createTriangleIndicesBuffer(mesh);
        }

        if (btu[toUnderlying(TRI_NORMALS)]) {
            // triangle normal buffer
            createTriangleNormalsBuffer(mesh);
        }

        if (btu[toUnderlying(TRI_COLORS)]) {
            // triangle color buffer
            createTriangleColorsBuffer(mesh);
        }

        if (btu[toUnderlying(VERT_TEXCOORDS)]) {
            // triangle vertex texture indices buffer
            createVertexTextureIndicesBuffer(mesh);
        }

        if (btu[toUnderlying(WEDGE_TEXCOORDS)]) {
            // triangle wedge texture indices buffer
            createWedgeTextureIndicesBuffer(mesh);
        }

        if (btu[toUnderlying(EDGES)]) {
            // edge index buffer
            createEdgeIndicesBuffer(mesh);
        }

        if (btu[toUnderlying(EDGE_NORMALS)]) {
            // edge normal buffer
            createEdgeNormalsBuffer(mesh);
        }

        if (btu[toUnderlying(EDGE_COLORS)]) {
            // edge color buffer
            createEdgeColorsBuffer(mesh);
        }

        if (btu[toUnderlying(WIREFRAME)]) {
            // wireframe index buffer
            createWireframeIndicesBuffer(mesh);
        }

        if (btu[toUnderlying(TEXTURES)]) {
            // textures
            createTextureUnits(mesh);
        }

        if (btu[toUnderlying(MESH_UNIFORMS)]) {
            // mesh uniforms
            fillMeshAttribs(mesh);
        }
    }

    void clear(MRI::BuffersBitSet buffersToClear = MRI::BUFFERS_ALL)
    {
        using enum MRI::Buffers;

        if (buffersToClear[toUnderlying(VERTICES)])
            mVerts.clear();

        if (buffersToClear[toUnderlying(TRIANGLES)])
            mTris.clear();

        if (buffersToClear[toUnderlying(EDGES)])
            mEdges.clear();

        if (buffersToClear[toUnderlying(WIREFRAME)])
            mWireframe.clear();

        if (buffersToClear[toUnderlying(VERT_NORMALS)])
            mVNormals.clear();

        if (buffersToClear[toUnderlying(VERT_COLORS)])
            mVColors.clear();

        if (buffersToClear[toUnderlying(TRI_NORMALS)])
            mTNormals.clear();

        if (buffersToClear[toUnderlying(TRI_COLORS)])
            mTColors.clear();

        if (buffersToClear[toUnderlying(VERT_TEXCOORDS)]) {
            mVTexCoords.clear();
            mVTexIds.clear();
        }

        if (buffersToClear[toUnderlying(VERT_TEXCOORDS)]) {
            mWTexIds.clear();
            mWTexCoords.clear();
        }

        if (buffersToClear[toUnderlying(EDGE_NORMALS)])
            mENormals.clear();

        if (buffersToClear[toUnderlying(EDGE_COLORS)])
            mEColors.clear();

        if (buffersToClear[toUnderlying(TEXTURES)])
            mTextures.clear();

        if (buffersToClear[toUnderlying(MESH_UNIFORMS)])
            mMeshColor = {0.5, 0.5, 0.5, 1};
    }

    uint vertexNumber() const { return mVerts.size() / 3; }

    uint triangleNumber() const { return mTris.size() / 3; }

    uint edgeNumber() const { return mEdges.size() / 2; }

    uint wireframeEdgeNumber() const { return mWireframe.size() / 2; }

    uint textureNumber() const { return mTextures.size(); }

    vcl::Point2i textureSize(uint ti) const
    {
        return vcl::Point2i(mTextures[ti].width(), mTextures[ti].height());
    }

    const float* vertexBufferData() const
    {
        if (mVerts.empty())
            return nullptr;
        return mVerts.data();
    }

    uint vertexBufferSize() const { return mVerts.size(); }

    const uint32_t* triangleBufferData() const
    {
        if (mTris.empty())
            return nullptr;
        return mTris.data();
    }

    uint triangleBufferSize() const { return mTris.size(); }

    const uint32_t* edgeBufferData() const
    {
        if (mEdges.empty())
            return nullptr;
        return mEdges.data();
    }

    uint edgeBufferSize() const { return mEdges.size(); }

    const uint32_t* wireframeBufferData() const
    {
        if (mWireframe.empty())
            return nullptr;
        return mWireframe.data();
    }

    uint wireframeBufferSize() const { return mWireframe.size(); }

    const float* vertexNormalBufferData() const
    {
        if (mVNormals.empty())
            return nullptr;
        return mVNormals.data();
    }

    const uint32_t* vertexColorBufferData() const
    {
        if (mVColors.empty())
            return nullptr;
        return mVColors.data();
    }

    const float* triangleNormalBufferData() const
    {
        if (mTNormals.empty())
            return nullptr;
        return mTNormals.data();
    }

    const uint32_t* triangleColorBufferData() const
    {
        if (mTColors.empty())
            return nullptr;
        return mTColors.data();
    }

    const float* vertexTexCoordsBufferData() const
    {
        if (mVTexCoords.empty())
            return nullptr;
        return mVTexCoords.data();
    }

    const uint32_t* vertexTextureIDsBufferData() const
    {
        if (mVTexIds.empty())
            return nullptr;
        return mVTexIds.data();
    }

    const float* wedgeTexCoordsBufferData() const
    {
        if (mWTexCoords.empty())
            return nullptr;
        return mWTexCoords.data();
    }

    const uint32_t* wedgeTextureIDsBufferData() const
    {
        if (mWTexIds.empty())
            return nullptr;
        return mWTexIds.data();
    }

    const float* edgeNormalBufferData() const
    {
        if (mENormals.empty())
            return nullptr;
        return mENormals.data();
    }

    const uint32_t* edgeColorBufferData() const
    {
        if (mEColors.empty())
            return nullptr;
        return mEColors.data();
    }

    const float* meshColorBufferData() const { return mMeshColor.data(); }

    const unsigned char* textureBufferData(uint ti) const
    {
        return mTextures[ti].data();
    }

private:
    void fillMeshAttribs(const MeshType& m)
    {
        if constexpr (vcl::HasColor<MeshType>) {
            mMeshColor[0] = m.color().redF();
            mMeshColor[1] = m.color().greenF();
            mMeshColor[2] = m.color().blueF();
            mMeshColor[3] = m.color().alphaF();
        }
    }

    void createVertexCoordsBuffer(const MeshType& mesh)
    {
        uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

        mVerts.resize(nv * 3);

        vertexCoordsToBuffer(mesh, mVerts.data());
        appendDuplicateVertexCoordsToBuffer(
            mesh, mVertsToDuplicate, mVerts.data());
    }

    void createVertexNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                mVNormals.resize(nv * 3);

                vertexNormalsToBuffer(mesh, mVNormals.data());
                appendDuplicateVertexNormalsToBuffer(
                    mesh, mVertsToDuplicate, mVNormals.data());
            }
        }
    }

    void createVertexColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                mVColors.resize(nv);

                vertexColorsToBuffer(
                    mesh, mVColors.data(), Color::Format::ABGR);
                appendDuplicateVertexColorsToBuffer(
                    mesh,
                    mVertsToDuplicate,
                    mVColors.data(),
                    Color::Format::ABGR);
            }
        }
    }

    void createVertexTexCoordsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                mVTexCoords.resize(nv * 2);

                vertexTexCoordsToBuffer(mesh, mVTexCoords.data());
                appendDuplicateVertexTexCoordsToBuffer(
                    mesh, mVertsToDuplicate, mVTexCoords.data());
            }
        }
    }

    void createWedgeTexCoordsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                mWTexCoords.resize(nv * 2);

                wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
                    mesh, mVertWedgeMap, mFacesToReassign, mWTexCoords.data());
            }
        }
    }

    void createTriangleIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            const uint NUM_TRIS = vcl::countTriangulatedTriangles(mesh);

            mTris.resize(NUM_TRIS * 3);

            triangulatedFaceIndicesToBuffer(
                mesh,
                mTris.data(),
                mIndexMap,
                MatrixStorageType::ROW_MAJOR,
                NUM_TRIS);
            replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
                mesh,
                mVertsToDuplicate,
                mFacesToReassign,
                mIndexMap,
                mTris.data());
        }
    }

    void createTriangleNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceNormal<MeshType>) {
            if (vcl::isPerFaceNormalAvailable(mesh)) {
                const uint NUM_TRIS = mIndexMap.triangleNumber();

                mTNormals.resize(NUM_TRIS * 3);

                triangulatedFaceNormalsToBuffer(
                    mesh,
                    mTNormals.data(),
                    mIndexMap,
                    MatrixStorageType::ROW_MAJOR);
            }
        }
    }

    void createTriangleColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceColor<MeshType>) {
            if (vcl::isPerFaceColorAvailable(mesh)) {
                const uint NUM_TRIS = mIndexMap.triangleNumber();

                mTColors.resize(NUM_TRIS);

                triangulatedFaceColorsToBuffer(
                    mesh, mTColors.data(), mIndexMap, Color::Format::ABGR);
            }
        }
    }

    void createVertexTextureIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (
            vcl::HasFaces<MeshType> && vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                const uint NUM_TRIS = vcl::countTriangulatedTriangles(mesh);

                mVTexIds.resize(NUM_TRIS);

                vertexTexCoordIndicesAsTriangulatedFaceWedgeTexCoordIndicesToBuffer(
                    mesh, mVTexIds.data(), mIndexMap);
            }
        }
    }

    void createWedgeTextureIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                const uint NUM_TRIS = mIndexMap.triangleNumber();

                mWTexIds.resize(NUM_TRIS);

                triangulatedFaceWedgeTexCoordIndicesToBuffer(
                    mesh, mWTexIds.data(), mIndexMap);
            }
        }
    }

    void createEdgeIndicesBuffer(const MeshType& mesh)
    {
        if constexpr (vcl::HasEdges<MeshType>) {
            mEdges.resize(mesh.edgeNumber() * 2);

            edgeIndicesToBuffer(mesh, mEdges.data());
        }
    }

    void createEdgeNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
            if (vcl::isPerEdgeNormalAvailable(mesh)) {
                mENormals.resize(mesh.edgeNumber() * 3);

                edgeNormalsToBuffer(mesh, mENormals.data());
            }
        }
    }

    void createEdgeColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
            if (vcl::isPerEdgeColorAvailable(mesh)) {
                mEColors.resize(mesh.edgeNumber());

                edgeColorsToBuffer(mesh, mEColors.data(), Color::Format::ABGR);
            }
        }
    }

    void createWireframeIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            const uint NUM_EDGES = vcl::countPerFaceVertexReferences(mesh);

            mWireframe.resize(NUM_EDGES * 2);

            wireframeIndicesToBuffer(mesh, mWireframe.data());
        }
    }

    void createTextureUnits(const MeshType& mesh)
    {
        if constexpr (vcl::HasTextureImages<MeshType>) {
            for (const vcl::Texture& t : mesh.textures()) {
                if (t.image().isNull()) {
                    vcl::Image txt(mesh.meshBasePath() + t.path());
                    txt.mirror();
                    mTextures.push_back(txt);
                }
                else {
                    mTextures.push_back(t.image());
                    mTextures.back().mirror();
                }
            }
        }
        else if constexpr (vcl::HasTexturePaths<MeshType>) {
            for (uint i = 0; i < mesh.textureNumber(); ++i) {
                vcl::Image txt(mesh.meshBasePath() + mesh.texturePath(i));
                txt.mirror();
                mTextures.push_back(txt);
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H
