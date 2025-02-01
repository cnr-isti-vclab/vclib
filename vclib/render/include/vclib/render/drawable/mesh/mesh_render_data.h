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

#include "mesh_buffer_id.h"
#include "mesh_render_settings.h"

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
    BuffersToFill mBuffersToFill = BUFFERS_TO_FILL_ALL;

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

public:
    MeshRenderData() = default;

    MeshRenderData(
        const MeshType& m,
        BuffersToFill   buffersToFill = BUFFERS_TO_FILL_ALL) :
            mBuffersToFill(buffersToFill)
    {
        update(m);
    }

    void update(const MeshType& mesh)
    {
        using enum MeshBufferId;

        clear();

        std::vector<std::pair<uint, uint>>          vwm;
        std::list<uint>                             vtd;
        std::list<std::list<std::pair<uint, uint>>> ftr;

        if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
            if (mesh.isPerFaceWedgeTexCoordsEnabled()) {
                countVerticesToDuplicateByWedgeTexCoords(mesh, vwm, vtd, ftr);
            }
        }

        TriPolyIndexBiMap indexMap;
        uint              numTris = 0;

        fillMeshAttribs(mesh);

        if (mBuffersToFill[toUnderlying(VERTICES)]) {
            // vertex buffer (coordinates)
            createVertexCoordsBuffer(mesh, vwm, vtd, ftr);

            // vertex buffer (normals)
            createVertexNormalsBuffer(mesh, vwm, vtd, ftr);

            // vertex buffer (colors)
            createVertexColorsBuffer(mesh, vwm, vtd, ftr);

            // vertex buffer (UVs)
            createVertexTexCoordsBuffer(mesh, vwm, vtd, ftr);

            // vertex wedges buffer (duplicated vertices)
            createWedgeTexCoordsBuffer(mesh, vwm, vtd, ftr);

            if (mBuffersToFill[toUnderlying(TRIANGLES)]) {
                // triangle index buffer
                createTriangleIndicesBuffer(mesh, vwm, vtd, ftr, indexMap);

                // triangle normal buffer
                createTriangleNormalsBuffer(mesh, indexMap);

                // triangle color buffer
                createTriangleColorsBuffer(mesh, indexMap);

                // triangle vertex texture indices buffer
                createVertexTextureIndicesBuffer(mesh, indexMap);

                // triangle wedge texture indices buffer
                createWedgeTextureIndicesBuffer(mesh, indexMap);
            }

            if (mBuffersToFill[toUnderlying(EDGES)]) {
                // edge index buffer
                createEdgeIndicesBuffer(mesh);

                // edge normal buffer
                createEdgeNormalsBuffer(mesh);

                // edge color buffer
                createEdgeColorsBuffer(mesh);
            }

            if (mBuffersToFill[toUnderlying(WIREFRAME)]) {
                // wireframe index buffer
                createWireframeIndicesBuffer(mesh);
            }

            if (mBuffersToFill[toUnderlying(TEXTURES)]) {
                // textures
                createTextureUnits(mesh);
            }
        }
    }

    void clear()
    {
        mVerts.clear();
        mTris.clear();
        mEdges.clear();
        mWireframe.clear();
        mVNormals.clear();
        mVColors.clear();
        mTNormals.clear();
        mTColors.clear();
        mVTexCoords.clear();
        mVTexIds.clear();
        mWTexCoords.clear();
        mWTexIds.clear();
        mENormals.clear();
        mEColors.clear();
        mMeshColor = {0.5, 0.5, 0.5, 1};
        mTextures.clear();
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

    void createVertexCoordsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        uint nv = mesh.vertexNumber() + vtd.size();

        mVerts.resize(nv * 3);

        vertexCoordsToBuffer(mesh, mVerts.data());
        appendDuplicateVertexCoordsToBuffer(mesh, vtd, mVerts.data());
    }

    void createVertexNormalsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (mBuffersToFill[toUnderlying(VERT_NORMALS)]) {
                if (vcl::isPerVertexNormalAvailable(mesh)) {
                    uint nv = mesh.vertexNumber() + vtd.size();

                    mVNormals.resize(nv * 3);

                    vertexNormalsToBuffer(mesh, mVNormals.data());
                    appendDuplicateVertexNormalsToBuffer(
                        mesh, vtd, mVNormals.data());
                }
            }
        }
    }

    void createVertexColorsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (mBuffersToFill[toUnderlying(VERT_COLORS)]) {
                if (vcl::isPerVertexColorAvailable(mesh)) {
                    uint nv = mesh.vertexNumber() + vtd.size();

                    mVColors.resize(nv);

                    vertexColorsToBuffer(
                        mesh, mVColors.data(), Color::Format::ABGR);
                    appendDuplicateVertexColorsToBuffer(
                        mesh, vtd, mVColors.data(), Color::Format::ABGR);
                }
            }
        }
    }

    void createVertexTexCoordsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (mBuffersToFill[toUnderlying(VERT_TEXCOORDS)]) {
                if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                    uint nv = mesh.vertexNumber() + vtd.size();

                    mVTexCoords.resize(nv * 2);

                    vertexTexCoordsToBuffer(mesh, mVTexCoords.data());
                    appendDuplicateVertexTexCoordsToBuffer(
                        mesh, vtd, mVTexCoords.data());
                }
            }
        }
    }

    void createWedgeTexCoordsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (mBuffersToFill[toUnderlying(WEDGE_TEXCOORDS)]) {
                if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                    uint nv = mesh.vertexNumber() + vtd.size();

                    mWTexCoords.resize(nv * 2);

                    wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
                        mesh, vmw, ftr, mWTexCoords.data());
                }
            }
        }
    }

    void createTriangleIndicesBuffer(
        const MeshType&    mesh,
        const auto&        vmw,
        const auto&        vtd,
        const auto&        ftr,
        TriPolyIndexBiMap& indexMap)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasFaces<MeshType>) {
            const uint NUM_TRIS = vcl::countTriangulatedTriangles(mesh);

            mTris.resize(NUM_TRIS * 3);

            triangulatedFaceIndicesToBuffer(
                mesh,
                mTris.data(),
                indexMap,
                MatrixStorageType::ROW_MAJOR,
                NUM_TRIS);
            replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
                mesh, vtd, ftr, indexMap, mTris.data());
        }
    }

    void createTriangleNormalsBuffer(
        const MeshType&          mesh,
        const TriPolyIndexBiMap& indexMap)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerFaceNormal<MeshType>) {
            if (mBuffersToFill[toUnderlying(TRI_NORMALS)]) {
                if (vcl::isPerFaceNormalAvailable(mesh)) {
                    const uint NUM_TRIS = indexMap.triangleNumber();

                    mTNormals.resize(NUM_TRIS * 3);

                    triangulatedFaceNormalsToBuffer(
                        mesh,
                        mTNormals.data(),
                        indexMap,
                        MatrixStorageType::ROW_MAJOR);
                }
            }
        }
    }

    void createTriangleColorsBuffer(
        const MeshType&          mesh,
        const TriPolyIndexBiMap& indexMap)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerFaceColor<MeshType>) {
            if (mBuffersToFill[toUnderlying(TRI_COLORS)]) {
                if (vcl::isPerFaceColorAvailable(mesh)) {
                    const uint NUM_TRIS = indexMap.triangleNumber();

                    mTColors.resize(NUM_TRIS);

                    triangulatedFaceColorsToBuffer(
                        mesh, mTColors.data(), indexMap, Color::Format::ABGR);
                }
            }
        }
    }

    void createVertexTextureIndicesBuffer(
        const MeshType&          mesh,
        const TriPolyIndexBiMap& indexMap)
    {
        using enum MeshBufferId;

        if constexpr (
            vcl::HasFaces<MeshType> && vcl::HasPerVertexTexCoord<MeshType>) {
            if (mBuffersToFill[toUnderlying(VERT_TEXCOORDS)]) {
                if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                    const uint NUM_TRIS = vcl::countTriangulatedTriangles(mesh);

                    mVTexIds.resize(NUM_TRIS);

                    vertexTexCoordIndicesAsTriangulatedFaceWedgeTexCoordIndicesToBuffer(
                        mesh, mVTexIds.data(), indexMap);
                }
            }
        }
    }

    void createWedgeTextureIndicesBuffer(
        const MeshType&          mesh,
        const TriPolyIndexBiMap& indexMap)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (mBuffersToFill[toUnderlying(WEDGE_TEXCOORDS)]) {
                if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                    const uint NUM_TRIS = indexMap.triangleNumber();

                    mWTexIds.resize(NUM_TRIS);

                    triangulatedFaceWedgeTexCoordIndicesToBuffer(
                        mesh, mWTexIds.data(), indexMap);
                }
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
        using enum MeshBufferId;

        if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
            if (mBuffersToFill[toUnderlying(EDGE_NORMALS)]) {
                if (vcl::isPerEdgeNormalAvailable(mesh)) {
                    mENormals.resize(mesh.edgeNumber() * 3);

                    edgeNormalsToBuffer(mesh, mENormals.data());
                }
            }
        }
    }

    void createEdgeColorsBuffer(const MeshType& mesh)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
            if (mBuffersToFill[toUnderlying(EDGE_COLORS)]) {
                if (vcl::isPerEdgeColorAvailable(mesh)) {
                    mEColors.resize(mesh.edgeNumber());

                    edgeColorsToBuffer(
                        mesh, mEColors.data(), Color::Format::ABGR);
                }
            }
        }
    }

    void createWireframeIndicesBuffer(const MeshType& mesh)
    {
        using enum MeshBufferId;

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
