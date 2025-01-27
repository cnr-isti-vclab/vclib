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

#include <vclib/algorithms/core/polygon.h>
#include <vclib/math/min_max.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>
#include <vclib/space/core/image.h>
#include <vclib/space/core/texture.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderData
{
    bool mDuplicatedVertices = false;

    std::vector<float>    mVerts;
    std::vector<uint32_t> mTris;
    std::vector<uint32_t> mEdges;
    std::vector<uint32_t> mWireframe;

    std::vector<float>    mVNormals;
    std::vector<uint32_t> mVColors;
    std::vector<float>    mVTexCoords;

    std::vector<float>    mTNormals;
    std::vector<uint32_t> mTColors;
    std::vector<float>    mWTexCoords;
    std::vector<uint32_t> mWTexIds;

    std::vector<float>    mENormals;
    std::vector<uint32_t> mEColors;

    std::array<float, 4> mMeshColor = {0.5, 0.5, 0.5, 1};

    vcl::Point3d            mBBMin, mBBMax;
    vcl::TriPolyIndexBiMap  mTriPolyMap;
    std::vector<vcl::Image> mTextures;

public:
    MeshRenderData() = default;

    MeshRenderData(
        const MeshType& m,
        BuffersToFill   buffersToFill = BUFFERS_TO_FILL_ALL)
    {
        update(m, buffersToFill);
    }

    void update(
        const MeshType& m,
        BuffersToFill   buffersToFill = BUFFERS_TO_FILL_ALL)
    {
        using enum MeshBufferId;

        clear();

        if (buffersToFill[toUnderlying(VERTICES)]) {
            std::vector<std::vector<uint>> triVertIndices;
            fillVertices(m, buffersToFill, triVertIndices);

            if (buffersToFill[toUnderlying(TRIANGLES)])
                fillTriangles(m, buffersToFill, triVertIndices);

            if (buffersToFill[toUnderlying(EDGES)])
                fillEdges(m, buffersToFill);

            if (buffersToFill[toUnderlying(WIREFRAME)])
                fillWireframe(m);
        }

        if (buffersToFill[toUnderlying(TEXTURES)])
            fillTextures(m);

        fillMeshAttribs(m);
    }

    void clear()
    {
        mDuplicatedVertices = false;

        mVerts.clear();
        mTris.clear();
        mEdges.clear();
        mWireframe.clear();
        mVNormals.clear();
        mVColors.clear();
        mTNormals.clear();
        mTColors.clear();
        mVTexCoords.clear();
        mWTexCoords.clear();
        mWTexIds.clear();
        mENormals.clear();
        mEColors.clear();
        mMeshColor = {0.5, 0.5, 0.5, 1};
        mTriPolyMap.clear();
        mTextures.clear();
        clearBB();
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

    const vcl::Point3d& bbMin() const { return mBBMin; }

    const vcl::Point3d& bbMax() const { return mBBMax; }

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
    void clearBB()
    {
        mBBMin = Point3d(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max());
        mBBMax = Point3d(
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest());
    }

    void fillVertices(
        const MeshType&                 m,
        BuffersToFill                   buffersToFill,
        std::vector<std::vector<uint>>& triVertIndices)
    {
        using enum MeshBufferId;

        // used only when we duplicate vertices
        std::vector<bool> vertAlreadyFound;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                if (buffersToFill[toUnderlying(WEDGE_TEXCOORDS)]) {
                    mDuplicatedVertices = true;
                    vertAlreadyFound.resize(m.vertexContainerSize(), false);
                }
            }
        }

        // not using Mesh's bounding box if:
        // - it has not bounding box, or
        // - it has bounding box, but it is null (not valid)
        bool bbToInitialize = !vcl::HasBoundingBox<MeshType>;
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (m.boundingBox().isNull()) {
                bbToInitialize = true;
            }
        }

        // if mesh has bounding box, I set it anyway from its bb
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            mBBMin = m.boundingBox().min().template cast<double>();
            mBBMax = m.boundingBox().max().template cast<double>();
        }
        // if I need to compute bb, I initialize to invalid numbers
        if (bbToInitialize) {
            clearBB();
        }

        uint vn = m.vertexNumber();

        if constexpr (vcl::HasFaces<MeshType>) {
            if (mDuplicatedVertices)
                vn = m.faceNumber() * 3;
        }

        mVerts.reserve(vn * 3);

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (buffersToFill[toUnderlying(VERT_NORMALS)]) {
                if (vcl::isPerVertexNormalAvailable(m)) {
                    mVNormals.reserve(vn * 3);
                }
            }
        }

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (buffersToFill[toUnderlying(VERT_COLORS)]) {
                if (vcl::isPerVertexColorAvailable(m)) {
                    mVColors.reserve(vn);
                }
            }
        }

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (buffersToFill[toUnderlying(VERT_TEXCOORDS)]) {
                if (vcl::isPerVertexTexCoordAvailable(m)) {
                    mVTexCoords.reserve(vn * 2);
                }
            }
        }

        // lambda to avoid code duplication
        auto fillVertex = [&](const auto& v) {
            mVerts.push_back(v.coord().x());
            mVerts.push_back(v.coord().y());
            mVerts.push_back(v.coord().z());

            if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                if (buffersToFill[toUnderlying(VERT_NORMALS)]) {
                    if (vcl::isPerVertexNormalAvailable(m)) {
                        mVNormals.push_back(v.normal().x());
                        mVNormals.push_back(v.normal().y());
                        mVNormals.push_back(v.normal().z());
                    }
                }
            }

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (buffersToFill[toUnderlying(VERT_COLORS)]) {
                    if (vcl::isPerVertexColorAvailable(m)) {
                        mVColors.push_back(v.color().abgr());
                    }
                }
            }

            if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                if (buffersToFill[toUnderlying(VERT_TEXCOORDS)]) {
                    if (vcl::isPerVertexTexCoordAvailable(m)) {
                        mVTexCoords.push_back(v.texCoord().u());
                        mVTexCoords.push_back(v.texCoord().v());
                    }
                }
            }
        };

        // first, fill the vertices (in order to fill also unreferenced ones)
        for (const auto& v : m.vertices()) {
            if (bbToInitialize) {
                mBBMin = vcl::min(mBBMin, v.coord().template cast<double>());
                mBBMax = vcl::max(mBBMax, v.coord().template cast<double>());
            }

            fillVertex(v);
        }

        if constexpr (vcl::HasFaces<MeshType>) {
            uint fi  = 0;
            uint vdi = m.vertexNumber();
            triVertIndices.resize(m.faceNumber());
            for (const auto& f : m.faces()) {
                triVertIndices[fi].reserve(f.vertexNumber());
                for (const auto* v : f.vertices()) {
                    uint vi = m.vertexIndexIfCompact(v->index());
                    if (mDuplicatedVertices) {
                        if (!vertAlreadyFound[vi]) {
                            vertAlreadyFound[vi] = true;
                            triVertIndices[fi].push_back(vi);
                        }
                        else {
                            fillVertex(*v);
                            triVertIndices[fi].push_back(vdi++);
                        }
                    }
                    else {
                        triVertIndices[fi].push_back(vi);
                    }
                }
                fi++;
            }
        }
    }

    void fillTriangles(
        const MeshType&                       m,
        BuffersToFill                         buffersToFill,
        const std::vector<std::vector<uint>>& triVertIndices)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasFaces<MeshType>) {
            std::vector<std::vector<uint>> vinds; // necessary for wedge attribs

            if constexpr (vcl::HasTriangles<MeshType>) {
                mTris.reserve(m.faceNumber() * 3);

                uint fi = 0;
                for (const auto& f : m.faces()) {
                    uint i = 0;
                    mTris.push_back(triVertIndices[fi][i++]);
                    mTris.push_back(triVertIndices[fi][i++]);
                    mTris.push_back(triVertIndices[fi][i++]);
                    ++fi;
                }
            }
            else {
                vinds.reserve(m.faceNumber());
                mTriPolyMap.reserve(m.faceNumber(), m.faceNumber());
                mTris.reserve(m.faceNumber());

                uint fi = 0;
                uint nt = 0;
                for (const auto& f : m.faces()) {
                    if (f.vertexNumber() == 3) {
                        uint i = 0;
                        mTriPolyMap.insert(nt, fi);
                        mTris.push_back(triVertIndices[fi][i++]);
                        mTris.push_back(triVertIndices[fi][i++]);
                        mTris.push_back(triVertIndices[fi][i++]);
                        vinds.push_back({0, 1, 2});
                        ++nt;
                    }
                    else {
                        std::vector<uint> vind = vcl::earCut(f);
                        for (uint vi = 0; vi < vind.size(); vi += 3) {
                            mTriPolyMap.insert(nt + vi / 3, fi);

                            mTris.push_back(triVertIndices[fi][vind[vi + 0]]);
                            mTris.push_back(triVertIndices[fi][vind[vi + 1]]);
                            mTris.push_back(triVertIndices[fi][vind[vi + 2]]);
                        }
                        nt += vind.size() / 3;
                        vinds.push_back(vind);
                    }
                    ++fi;
                }
            }

            if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                if (buffersToFill[toUnderlying(TRI_NORMALS)]) {
                    if (vcl::isPerFaceNormalAvailable(m)) {
                        mTNormals.reserve(m.faceNumber() * 3);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceColor<MeshType>) {
                if (buffersToFill[toUnderlying(TRI_COLORS)]) {
                    if (vcl::isPerFaceColorAvailable(m)) {
                        mTColors.reserve(m.faceNumber());
                    }
                }
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                if (buffersToFill[toUnderlying(WEDGE_TEXCOORDS)]) {
                    if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                        mWTexCoords.reserve(m.faceNumber() * 3 * 2);
                        mWTexCoords.resize(m.vertexNumber() * 2);
                        mWTexIds.reserve(m.faceNumber());
                    }
                }
            }

            // used only when we duplicate vertices
            std::vector<bool> vertAlreadyFound;
            if (mDuplicatedVertices) {
                vertAlreadyFound.resize(m.vertexNumber(), false);
            }

            uint fi = 0;
            for (const auto& f : m.faces()) {
                if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                    if (buffersToFill[toUnderlying(TRI_NORMALS)]) {
                        if (vcl::isPerFaceNormalAvailable(m)) {
                            if constexpr (vcl::HasTriangles<MeshType>) {
                                mTNormals.push_back(f.normal().x());
                                mTNormals.push_back(f.normal().y());
                                mTNormals.push_back(f.normal().z());
                            }
                            else {
                                for (uint i = 0;
                                     i < mTriPolyMap.triangleNumber(fi);
                                     i++) {
                                    mTNormals.push_back(f.normal().x());
                                    mTNormals.push_back(f.normal().y());
                                    mTNormals.push_back(f.normal().z());
                                }
                            }
                        }
                    }
                }

                if constexpr (vcl::HasPerFaceColor<MeshType>) {
                    if (buffersToFill[toUnderlying(TRI_COLORS)]) {
                        if (vcl::isPerFaceColorAvailable(m)) {
                            if constexpr (vcl::HasTriangles<MeshType>) {
                                mTColors.push_back(f.color().abgr());
                            }
                            else {
                                for (uint i = 0;
                                     i < mTriPolyMap.triangleNumber(fi);
                                     i++) {
                                    mTColors.push_back(f.color().abgr());
                                }
                            }
                        }
                    }
                }

                if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                    if (buffersToFill[toUnderlying(WEDGE_TEXCOORDS)]) {
                        if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                            // vertices are duplicated here - each face has
                            // its one. We store the texture coordinates on it.
                            // order is the same used when creating the
                            // vertex buffer data.
                            for (uint vi = 0; vi < f.vertexNumber(); ++vi) {
                                const auto* v = f.vertex(vi);
                                if (!vertAlreadyFound[v->index()]) {
                                    vertAlreadyFound[v->index()] = true;
                                    uint mvi =
                                        m.vertexIndexIfCompact(v->index());
                                    mWTexCoords[mvi * 2] =
                                        f.wedgeTexCoord(vi).u();
                                    mWTexCoords[mvi * 2 + 1] =
                                        f.wedgeTexCoord(vi).v();
                                }
                                else {
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vi).u());
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vi).v());
                                }
                            }

                            if constexpr (vcl::HasTriangles<MeshType>) {
                                mWTexIds.push_back(f.textureIndex());
                            }
                            else {
                                for (uint i = 0;
                                     i < mTriPolyMap.triangleNumber(fi);
                                     i++) {
                                    mWTexIds.push_back(f.textureIndex());
                                }
                            }
                        }
                    }
                }
                ++fi;
            }
        }
    }

    void fillEdges(const MeshType& m, BuffersToFill buffersToFill)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasEdges<MeshType>) {
            mEdges.reserve(m.edgeNumber() * 2);

            if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
                if (buffersToFill[toUnderlying(EDGE_NORMALS)]) {
                    if (vcl::isPerEdgeNormalAvailable(m)) {
                        mENormals.reserve(m.edgeNumber() * 3);
                    }
                }
            }

            if constexpr (vcl::HasPerEdgeColor<MeshType>) {
                if (buffersToFill[toUnderlying(EDGE_COLORS)]) {
                    if (vcl::isPerEdgeColorAvailable(m)) {
                        mEColors.reserve(m.edgeNumber());
                    }
                }
            }

            for (const auto& e : m.edges()) {
                mEdges.push_back(m.vertexIndexIfCompact(m.index(e.vertex(0))));
                mEdges.push_back(m.vertexIndexIfCompact(m.index(e.vertex(1))));

                if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
                    if (buffersToFill[toUnderlying(EDGE_NORMALS)]) {
                        if (vcl::isPerEdgeNormalAvailable(m)) {
                            mENormals.push_back(e.normal().x());
                            mENormals.push_back(e.normal().y());
                            mENormals.push_back(e.normal().z());
                        }
                    }
                }

                if constexpr (vcl::HasPerEdgeColor<MeshType>) {
                    if (buffersToFill[toUnderlying(EDGE_COLORS)]) {
                        if (vcl::isPerEdgeColorAvailable(m)) {
                            mEColors.push_back(e.color().abgr());
                        }
                    }
                }
            }
        }
    }

    void fillWireframe(const MeshType& m)
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            using FaceType = MeshType::FaceType;
            if constexpr (FaceType::VERTEX_NUMBER < 0) {
                // assuming faces are triangles; if they are not, reallocation
                // during push_back will be possible
                mWireframe.reserve(6 * m.faceNumber()); // 2 indices * 3 edges
            }
            else {
                mWireframe.reserve(
                    2 * FaceType::VERTEX_NUMBER * m.faceNumber());
            }

            for (const auto& f : m.faces()) {
                for (uint i = 0; i < f.vertexNumber(); ++i) {
                    mWireframe.push_back(
                        m.vertexIndexIfCompact(m.index(f.vertex(i))));
                    mWireframe.push_back(
                        m.vertexIndexIfCompact(m.index(f.vertexMod((i + 1)))));
                }
            }
        }
    }

    void fillTextures(const MeshType& m)
    {
        if constexpr (vcl::HasTextureImages<MeshType>) {
            for (const vcl::Texture& t : m.textures()) {
                if (t.image().isNull()) {
                    vcl::Image txt(m.meshBasePath() + t.path());
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
            for (uint i = 0; i < m.textureNumber(); ++i) {
                vcl::Image txt(m.meshBasePath() + m.texturePath(i));
                txt.mirror();
                mTextures.push_back(txt);
            }
        }
    }

    void fillMeshAttribs(const MeshType& m)
    {
        if constexpr (vcl::HasColor<MeshType>) {
            mMeshColor[0] = m.color().redF();
            mMeshColor[1] = m.color().greenF();
            mMeshColor[2] = m.color().blueF();
            mMeshColor[3] = m.color().alphaF();
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H
