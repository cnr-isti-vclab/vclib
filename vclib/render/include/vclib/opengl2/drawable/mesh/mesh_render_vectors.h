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

#ifndef VCL_OPENGL2_DRAWABLE_MESH_MESH_RENDER_VECTORS_H
#define VCL_OPENGL2_DRAWABLE_MESH_MESH_RENDER_VECTORS_H

#include <vclib/algorithms/mesh/import_export/append_replace_to_buffer.h>
#include <vclib/algorithms/mesh/import_export/export_buffer.h>
#include <vclib/algorithms/mesh/stat/topology.h>
#include <vclib/math/min_max.h>
#include <vclib/mesh/requirements.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_info.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>
#include <vclib/space/core/image.h>
#include <vclib/space/core/texture.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderVectors : protected MeshRenderData<MeshType>
{
    using Base = MeshRenderData<MeshType>;
    using MRI = MeshRenderInfo;

    std::vector<float>    mVerts;
    std::vector<float>    mVNormals;
    std::vector<uint32_t> mVColors;
    std::vector<float>    mVTexCoords;
    std::vector<float>    mWTexCoords;

    std::vector<uint32_t> mTris;
    std::vector<float>    mTNormals;
    std::vector<uint32_t> mTColors;
    std::vector<uint32_t> mVTexIds;
    std::vector<uint32_t> mWTexIds;

    std::vector<uint32_t> mEdges;
    std::vector<float>    mENormals;
    std::vector<uint32_t> mEColors;

    std::vector<uint32_t> mWireframe;

    std::vector<vcl::Image> mTextures;

    std::array<float, 4> mMeshColor = {0.5, 0.5, 0.5, 1};

public:
    MeshRenderVectors() = default;

    MeshRenderVectors(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToFill = MRI::BUFFERS_ALL) :
            Base(mesh, buffersToFill)
    {
        createBuffers(mesh, buffersToFill);
    }

    void swap(MeshRenderVectors& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mVerts, other.mVerts);
        swap(mVNormals, other.mVNormals);
        swap(mVColors, other.mVColors);
        swap(mVTexCoords, other.mVTexCoords);
        swap(mWTexCoords, other.mWTexCoords);
        swap(mTris, other.mTris);
        swap(mTNormals, other.mTNormals);
        swap(mTColors, other.mTColors);
        swap(mVTexIds, other.mVTexIds);
        swap(mWTexIds, other.mWTexIds);
        swap(mEdges, other.mEdges);
        swap(mENormals, other.mENormals);
        swap(mEColors, other.mEColors);
        swap(mWireframe, other.mWireframe);
        swap(mTextures, other.mTextures);
        swap(mMeshColor, other.mMeshColor);
    }

    void update(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToUpdate = MRI::BUFFERS_ALL)
    {
        createBuffers(mesh, buffersToUpdate);
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
    void createBuffers(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToUpdate = MRI::BUFFERS_ALL)
    {
        using enum MRI::Buffers;

        Base::preUpdate(mesh, buffersToUpdate);

        MRI::BuffersBitSet btu = Base::mBuffersToFill & buffersToUpdate;

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

    void createVertexCoordsBuffer(const MeshType& mesh)
    {
        uint nv = Base::numVerts();

        mVerts.resize(nv * 3);

        Base::fillVertexCoords(mesh, mVerts.data());
    }

    void createVertexNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                uint nv = Base::numVerts();

                mVNormals.resize(nv * 3);

                Base::fillVertexNormals(mesh, mVNormals.data());
            }
        }
    }

    void createVertexColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(mesh)) {
                uint nv = Base::numVerts();

                mVColors.resize(nv);

                Base::fillVertexColors(
                    mesh, mVColors.data(), Color::Format::ABGR);
            }
        }
    }

    void createVertexTexCoordsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                uint nv = Base::numVerts();

                mVTexCoords.resize(nv * 2);

                Base::fillVertexTexCoords(mesh, mVTexCoords.data());
            }
        }
    }

    void createWedgeTexCoordsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint nv = Base::numVerts();

                mWTexCoords.resize(nv * 2);

                Base::fillWedgeTexCoords(mesh, mWTexCoords.data());
            }
        }
    }

    void createTriangleIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            uint nt = Base::numTris();

            mTris.resize(nt * 3);

            Base::fillTriangleIndices(mesh, mTris.data());
        }
    }

    void createTriangleNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceNormal<MeshType>) {
            if (vcl::isPerFaceNormalAvailable(mesh)) {
                uint nt = Base::numTris();

                mTNormals.resize(nt * 3);

                Base::fillTriangleNormals(mesh, mTNormals.data());
            }
        }
    }

    void createTriangleColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceColor<MeshType>) {
            if (vcl::isPerFaceColorAvailable(mesh)) {
                uint nt = Base::numTris();

                mTColors.resize(nt);

                Base::fillTriangleColors(
                    mesh, mTColors.data(), Color::Format::ABGR);
            }
        }
    }

    void createVertexTextureIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (
            vcl::HasFaces<MeshType> && vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                uint nt = Base::numTris();

                mVTexIds.resize(nt);

                Base::fillVertexTextureIndices(mesh, mVTexIds.data());
            }
        }
    }

    void createWedgeTextureIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint nt = Base::numTris();

                mWTexIds.resize(nt);

                Base::fillWedgeTextureIndices(mesh, mWTexIds.data());
            }
        }
    }

    void createEdgeIndicesBuffer(const MeshType& mesh)
    {
        if constexpr (vcl::HasEdges<MeshType>) {
            mEdges.resize(mesh.edgeNumber() * 2);

            Base::fillEdgeIndices(mesh, mEdges.data());
        }
    }

    void createEdgeNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
            if (vcl::isPerEdgeNormalAvailable(mesh)) {
                mENormals.resize(mesh.edgeNumber() * 3);

                Base::fillEdgeNormals(mesh, mENormals.data());
            }
        }
    }

    void createEdgeColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
            if (vcl::isPerEdgeColorAvailable(mesh)) {
                mEColors.resize(mesh.edgeNumber());

                Base::fillEdgeColors(
                    mesh, mEColors.data(), Color::Format::ABGR);
            }
        }
    }

    void createWireframeIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            const uint nw = Base::numWireframeLines();

            mWireframe.resize(nw * 2);

            Base::fillWireframeIndices(mesh, mWireframe.data());
        }
    }

    void createTextureUnits(const MeshType& mesh)
    {
        mTextures.clear();
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

#endif // VCL_OPENGL2_DRAWABLE_MESH_MESH_RENDER_VECTORS_H
