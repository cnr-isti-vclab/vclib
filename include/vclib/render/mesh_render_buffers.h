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

#ifndef VCL_RENDER_MESH_RENDER_BUFFERS_H
#define VCL_RENDER_MESH_RENDER_BUFFERS_H

#include <vclib/algorithms/core/polygon.h>
#include <vclib/math/min_max.h>
#include <vclib/mesh/requirements.h>
#include <vclib/mesh/utils/tri_poly_index_bimap.h>
#include <vclib/space/image.h>

#include "mesh_render_settings.h"

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers
{
    std::vector<float>    mVerts;
    std::vector<uint32_t> mTris;
    std::vector<uint32_t> mWireframe;

    std::vector<float>    mVNormals;
    std::vector<uint32_t> mVColors;
    std::vector<float>    mVTexCoords;

    std::vector<float>    mTNormals;
    std::vector<uint32_t> mTColors;
    std::vector<float>    mWTexCoords;
    std::vector<short>    mWTexIds;

    std::array<float, 4> mMeshColor = {0.5, 0.5, 0.5, 1};

    vcl::Point3d            mBBMin, mBBMax;
    vcl::TriPolyIndexBiMap  mTriPolyMap;
    std::vector<vcl::Image> mTextures;

public:
    enum {
        VERT_NORMALS   = 1 << 0,
        VERT_COLORS    = 1 << 1,
        VERT_TEXCOORDS = 1 << 2,

        TRIANGLES       = 1 << 3,
        TRI_NORMALS     = 1 << 4,
        TRI_COLORS      = 1 << 5,
        WEDGE_TEXCOORDS = 1 << 6,
        WIREFRAME       = 1 << 7,

        EDGES = 1 << 8,

        TEXTURES = 1 << 9,

        ALL = 0xFFFFFFFF,
    };

    MeshRenderBuffers() = default;

    MeshRenderBuffers(const MeshType& m, uint buffersToFill = ALL)
    {
        update(m, buffersToFill);
    }

    void update(const MeshType& m, uint buffersToFill = ALL)
    {
        clear();
        fillVertices(m, buffersToFill);

        if (buffersToFill & TRIANGLES)
            fillTriangles(m, buffersToFill);

        if (buffersToFill & WIREFRAME)
            fillWireframe(m);

        if (buffersToFill & TEXTURES)
            fillTextures(m);

        fillMeshAttribs(m);
    }

    void clear()
    {
        mVerts.clear();
        mTris.clear();
        mWireframe.clear();
        mVNormals.clear();
        mVColors.clear();
        mTNormals.clear();
        mTColors.clear();
        mVTexCoords.clear();
        mWTexCoords.clear();
        mWTexIds.clear();
        mMeshColor = {0.5, 0.5, 0.5, 1};
        mTriPolyMap.clear();
        mTextures.clear();
        clearBB();
    }

    uint vertexNumber() const { return mVerts.size() / 3; }

    uint triangleNumber() const { return mTris.size() / 3; }

    uint edgeNumber() const { return mWireframe.size() / 2; }

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

    const uint vertexBufferSize() const { return mVerts.size(); }

    const uint32_t* triangleBufferData() const
    {
        if (mTris.empty())
            return nullptr;
        return mTris.data();
    }

    const uint triangleBufferSize() const { return mTris.size(); }

    const uint32_t* wireframeBufferData() const
    {
        if (mWireframe.empty())
            return nullptr;
        return mWireframe.data();
    }

    const uint wireframeBufferSize() const { return mWireframe.size(); }

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

    const short* wedgeTextureIDsBufferData() const
    {
        if (mWTexIds.empty())
            return nullptr;
        return mWTexIds.data();
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

    void fillVertices(const MeshType& m, uint buffersToFill)
    {
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

        mVerts.resize(m.vertexNumber() * 3);

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (buffersToFill & VERT_NORMALS) {
                if (vcl::isPerVertexNormalAvailable(m)) {
                    mVNormals.resize(m.vertexNumber() * 3);
                }
            }
        }

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (buffersToFill & VERT_COLORS) {
                if (vcl::isPerVertexColorAvailable(m)) {
                    mVColors.resize(m.vertexNumber());
                }
            }
        }

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (buffersToFill & VERT_TEXCOORDS) {
                if (vcl::isPerVertexTexCoordAvailable(m)) {
                    mVTexCoords.resize(m.vertexNumber() * 2);
                }
            }
        }

        uint vi = 0;
        for (const auto& v : m.vertices()) {
            uint i = vi * 2; // use i for size 2, like texcoords
            uint j = vi * 3; // use j for size 3, like coords and normals

            mVerts[j + 0] = v.coord().x();
            mVerts[j + 1] = v.coord().y();
            mVerts[j + 2] = v.coord().z();

            if (bbToInitialize) {
                mBBMin = vcl::min(mBBMin, v.coord().template cast<double>());
                mBBMax = vcl::max(mBBMax, v.coord().template cast<double>());
            }

            if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                if (buffersToFill & VERT_NORMALS) {
                    if (vcl::isPerVertexNormalAvailable(m)) {
                        mVNormals[j + 0] = v.normal().x();
                        mVNormals[j + 1] = v.normal().y();
                        mVNormals[j + 2] = v.normal().z();
                    }
                }
            }

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (buffersToFill & VERT_COLORS) {
                    if (vcl::isPerVertexColorAvailable(m)) {
                        mVColors[vi] = v.color().abgr();
                    }
                }
            }

            if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                if (buffersToFill & VERT_TEXCOORDS) {
                    if (vcl::isPerVertexTexCoordAvailable(m)) {
                        mVTexCoords[i + 0] = v.texCoord().u();
                        mVTexCoords[i + 1] = v.texCoord().v();
                    }
                }
            }

            vi++;
        }
    }

    void fillTriangles(const MeshType& m, uint buffersToFill)
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            std::vector<std::vector<uint>> vinds; // necessary for wedge attribs

            if constexpr (vcl::HasTriangles<MeshType>) {
                mTris.resize(m.faceNumber() * 3);

                uint i = 0;
                for (const auto& f : m.faces()) {
                    mTris[i + 0] = m.vertexIndexIfCompact(m.index(f.vertex(0)));
                    mTris[i + 1] = m.vertexIndexIfCompact(m.index(f.vertex(1)));
                    mTris[i + 2] = m.vertexIndexIfCompact(m.index(f.vertex(2)));
                    i += 3;
                }
            }
            else {
                vinds.reserve(m.faceNumber());
                mTriPolyMap.reserve(m.faceNumber(), m.faceNumber());
                mTris.reserve(m.faceNumber());

                uint nt = 0;
                for (const auto& f : m.faces()) {
                    if (f.vertexNumber() == 3) {
                        mTriPolyMap.insert(
                            nt, m.faceIndexIfCompact(m.index(f)));
                        mTris.push_back(
                            m.vertexIndexIfCompact(m.index(f.vertex(0))));
                        mTris.push_back(
                            m.vertexIndexIfCompact(m.index(f.vertex(1))));
                        mTris.push_back(
                            m.vertexIndexIfCompact(m.index(f.vertex(2))));
                        nt += 1;
                        vinds.push_back({0, 1, 2});
                    }
                    else {
                        std::vector<uint> vind = vcl::earCut(f);
                        for (uint vi = 0; vi < vind.size(); vi += 3) {
                            mTriPolyMap.insert(
                                nt + vi / 3, m.faceIndexIfCompact(m.index(f)));
                            mTris.push_back(m.vertexIndexIfCompact(
                                m.index(f.vertex(vind[vi + 0]))));
                            mTris.push_back(m.vertexIndexIfCompact(
                                m.index(f.vertex(vind[vi + 1]))));
                            mTris.push_back(m.vertexIndexIfCompact(
                                m.index(f.vertex(vind[vi + 2]))));
                        }
                        nt += vind.size() / 3;
                        vinds.push_back(vind);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                if (buffersToFill & TRI_NORMALS) {
                    if (vcl::isPerFaceNormalAvailable(m)) {
                        mTNormals.reserve(m.faceNumber() * 3);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceColor<MeshType>) {
                if (buffersToFill & TRI_COLORS) {
                    if (vcl::isPerFaceColorAvailable(m)) {
                        mTColors.reserve(m.faceNumber());
                    }
                }
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                if (buffersToFill & WEDGE_TEXCOORDS) {
                    if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                        mWTexCoords.reserve(m.faceNumber() * 3 * 2);
                        mWTexIds.reserve(m.faceNumber());
                    }
                }
            }

            for (const auto& f : m.faces()) {
                if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                    if (buffersToFill & TRI_NORMALS) {
                        if (vcl::isPerFaceNormalAvailable(m)) {
                            if constexpr (vcl::HasTriangles<MeshType>) {
                                mTNormals.push_back(f.normal().x());
                                mTNormals.push_back(f.normal().y());
                                mTNormals.push_back(f.normal().z());
                            }
                            else {
                                const uint fi =
                                    m.faceIndexIfCompact(m.index(f));
                                for (uint i = 0;
                                     i < mTriPolyMap.triangleNumber(fi);
                                     i++)
                                {
                                    mTNormals.push_back(f.normal().x());
                                    mTNormals.push_back(f.normal().y());
                                    mTNormals.push_back(f.normal().z());
                                }
                            }
                        }
                    }
                }

                if constexpr (vcl::HasPerFaceColor<MeshType>) {
                    if (buffersToFill & TRI_COLORS) {
                        if (vcl::isPerFaceColorAvailable(m)) {
                            if constexpr (vcl::HasTriangles<MeshType>) {
                                mTColors.push_back(f.color().abgr());
                            }
                            else {
                                const uint fi =
                                    m.faceIndexIfCompact(m.index(f));
                                for (uint i = 0;
                                     i < mTriPolyMap.triangleNumber(fi);
                                     i++)
                                {
                                    mTColors.push_back(f.color().abgr());
                                }
                            }
                        }
                    }
                }

                if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                    if (buffersToFill & WEDGE_TEXCOORDS) {
                        if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                            if constexpr (vcl::HasTriangles<MeshType>) {
                                mWTexCoords.push_back(f.wedgeTexCoord(0).u());
                                mWTexCoords.push_back(f.wedgeTexCoord(0).v());
                                mWTexCoords.push_back(f.wedgeTexCoord(1).u());
                                mWTexCoords.push_back(f.wedgeTexCoord(1).v());
                                mWTexCoords.push_back(f.wedgeTexCoord(2).u());
                                mWTexCoords.push_back(f.wedgeTexCoord(2).v());
                                mWTexIds.push_back(f.textureIndex());
                            }
                            else {
                                const uint fi =
                                    m.faceIndexIfCompact(m.index(f));
                                // triangulation of f
                                const std::vector<uint>& vind = vinds[fi];
                                // for each triangle of f
                                for (uint vi = 0; vi < vind.size(); vi += 3) {
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vind[vi + 0]).u());
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vind[vi + 0]).v());
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vind[vi + 1]).u());
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vind[vi + 1]).v());
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vind[vi + 2]).u());
                                    mWTexCoords.push_back(
                                        f.wedgeTexCoord(vind[vi + 2]).v());
                                    mWTexIds.push_back(f.textureIndex());
                                }
                            }
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
        if constexpr (vcl::HasTexturePaths<MeshType>) {
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

#endif // VCL_RENDER_MESH_RENDER_BUFFERS_H
