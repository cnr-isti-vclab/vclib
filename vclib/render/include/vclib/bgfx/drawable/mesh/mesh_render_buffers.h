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

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H

#include "mesh_render_buffers_macros.h"

#include <vclib/algorithms/mesh/import_export/append_replace_to_buffer.h>
#include <vclib/algorithms/mesh/import_export/export_buffer.h>
#include <vclib/algorithms/mesh/stat/topology.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers : public vcl::MeshRenderData<MeshType>
{
    using Base = vcl::MeshRenderData<MeshType>;

    BuffersToFill mBuffersToFill = BUFFERS_TO_FILL_ALL;

    VertexBuffer mVertexCoordsBuffer;
    VertexBuffer mVertexNormalsBuffer;
    VertexBuffer mVertexColorsBuffer;
    VertexBuffer mVertexUVBuffer;
    VertexBuffer mVertexWedgeUVBuffer;

    IndexBuffer mTriangleIndexBuffer;
    IndexBuffer mTriangleNormalBuffer;
    IndexBuffer mTriangleColorBuffer;

    IndexBuffer mTriangleTextureIndexBuffer;

    // TODO: manage wireframe with proper lines
    IndexBuffer mEdgeIndexBuffer;
    IndexBuffer mEdgeNormalBuffer;
    IndexBuffer mEdgeColorBuffer;

    // TODO: manage wireframe with proper lines
    bgfx::IndexBufferHandle mWireframeIndexBH = BGFX_INVALID_HANDLE;

    // TODO: manage with a TextureUnit class
    std::vector<std::pair<bgfx::TextureHandle, bgfx::UniformHandle>> mTexturesH;

public:
    MeshRenderBuffers() = default;

    MeshRenderBuffers(
        const MeshType& mesh,
        BuffersToFill   buffersToFill = BUFFERS_TO_FILL_ALL) :
            Base(mesh, buffersToFill), mBuffersToFill(buffersToFill)
    {
        createBGFXBuffers(mesh);
    }

    MeshRenderBuffers(const MeshRenderBuffers& other) = delete;

    MeshRenderBuffers(MeshRenderBuffers&& other) { swap(other); }

    ~MeshRenderBuffers() { destroyBGFXBuffers(); }

    MeshRenderBuffers& operator=(const MeshRenderBuffers& other) = delete;

    MeshRenderBuffers& operator=(MeshRenderBuffers&& other)
    {
        swap(other);
        return *this;
    }

    void swap(MeshRenderBuffers& other)
    {
        using std::swap;
        swap((Base&) *this, (Base&) other);
        swap(mBuffersToFill, other.mBuffersToFill);
        swap(mVertexCoordsBuffer, other.mVertexCoordsBuffer);
        swap(mVertexNormalsBuffer, other.mVertexNormalsBuffer);
        swap(mVertexColorsBuffer, other.mVertexColorsBuffer);
        swap(mVertexUVBuffer, other.mVertexUVBuffer);
        swap(mVertexWedgeUVBuffer, other.mVertexWedgeUVBuffer);
        swap(mTriangleIndexBuffer, other.mTriangleIndexBuffer);
        swap(mTriangleNormalBuffer, other.mTriangleNormalBuffer);
        swap(mTriangleColorBuffer, other.mTriangleColorBuffer);
        swap(mTriangleTextureIndexBuffer, other.mTriangleTextureIndexBuffer);
        swap(mEdgeIndexBuffer, other.mEdgeIndexBuffer);
        swap(mEdgeNormalBuffer, other.mEdgeNormalBuffer);
        swap(mEdgeColorBuffer, other.mEdgeColorBuffer);
        swap(mWireframeIndexBH, other.mWireframeIndexBH);
        swap(mTexturesH, other.mTexturesH);
    }

    friend void swap(MeshRenderBuffers& a, MeshRenderBuffers& b) { a.swap(b); }

    void update(const MeshType& mesh)
    {
        Base::update(mesh);
        destroyBGFXBuffers();
        createBGFXBuffers(mesh);
    }

    void bindVertexBuffers(const MeshRenderSettings& mrs) const
    {
        // bgfx allows a maximum number of 4 vertex streams...

        mVertexCoordsBuffer.bind(0);
        mVertexNormalsBuffer.bind(1);
        mVertexColorsBuffer.bind(2);

        if (mrs.isSurfaceColorPerVertexTexcoords()) {
            mVertexUVBuffer.bind(3);
        }
        else if (mrs.isSurfaceColorPerWedgeTexcoords()) {
            mVertexWedgeUVBuffer.bind(3);
        }
    }

    void bindIndexBuffers(
        MeshBufferId indexBufferToBind = MeshBufferId::TRIANGLES) const
    {
        if (indexBufferToBind == MeshBufferId::TRIANGLES) {
            mTriangleIndexBuffer.bind();

            mTriangleNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

            mTriangleColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);

            mTriangleTextureIndexBuffer.bind(
                VCL_MRB_TRIANGLE_TEXTURE_ID_BUFFER);
        }
        else if (indexBufferToBind == MeshBufferId::EDGES) {
            mEdgeIndexBuffer.bind();

            mEdgeNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

            mEdgeColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);
        }
        else if (indexBufferToBind == MeshBufferId::WIREFRAME) {
            bgfx::setIndexBuffer(mWireframeIndexBH);
        }
    }

    void bindTextures() const
    {
        uint i = VCL_MRB_TEXTURE0; // first slot available is VCL_MRB_TEXTURE0
        for (auto [th, uh] : mTexturesH) {
            bgfx::setTexture(i, uh, th);
            i++;
        }
    }

private:
    void createBGFXBuffers(const MeshType& mesh)
    {
        using enum MeshBufferId;

        std::vector<std::pair<uint, uint>> vwm;
        std::list<uint> vtd;
        std::list<std::list<std::pair<uint, uint>>> ftr;

        if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
            if (mesh.isPerFaceWedgeTexCoordsEnabled()) {
                countVerticesToDuplicateByWedgeTexCoords(mesh, vwm, vtd, ftr);
            }
        }

        TriPolyIndexBiMap indexMap;
        uint numTris = 0;

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

    void createVertexCoordsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        uint nv = mesh.vertexNumber() + vtd.size();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 3);

        vertexCoordsToBuffer(mesh, buffer);
        appendDuplicateVertexCoordsToBuffer(mesh, vtd, buffer);

        mVertexCoordsBuffer.set(
            buffer,
            nv * 3,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT,
            false,
            releaseFn);
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<float>(nv * 3);

                    vertexNormalsToBuffer(mesh, buffer);
                    appendDuplicateVertexNormalsToBuffer(mesh, vtd, buffer);

                    mVertexNormalsBuffer.set(
                        buffer,
                        nv * 3,
                        bgfx::Attrib::Normal,
                        3,
                        PrimitiveType::FLOAT,
                        false,
                        releaseFn);
                }
            }
        }
        // WAS:
        // if (Base::vertexNormalBufferData()) {
        //     mVertexNormalsBuffer.set(
        //         Base::vertexNormalBufferData(),
        //         Base::vertexNumber() * 3,
        //         bgfx::Attrib::Normal,
        //         3,
        //         PrimitiveType::FLOAT);
        // }
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<uint>(nv);

                    vertexColorsToBuffer(mesh, buffer, Color::Format::ABGR);
                    appendDuplicateVertexColorsToBuffer(
                        mesh, vtd, buffer, Color::Format::ABGR);

                    mVertexColorsBuffer.set(
                        buffer,
                        nv * 4,
                        bgfx::Attrib::Color0,
                        4,
                        PrimitiveType::UCHAR,
                        true,
                        releaseFn);

                }
            }
        }
        // WAS:
        // if (Base::vertexColorBufferData()) {
        //     mVertexColorsBuffer.set(
        //         Base::vertexColorBufferData(),
        //         Base::vertexNumber() * 4,
        //         bgfx::Attrib::Color0,
        //         4,
        //         PrimitiveType::UCHAR,
        //         true);
        // }
    }

    void createVertexTexCoordsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        if (Base::vertexTexCoordsBufferData()) {
            mVertexUVBuffer.set(
                Base::vertexTexCoordsBufferData(),
                Base::vertexNumber() * 2,
                bgfx::Attrib::TexCoord0,
                2,
                PrimitiveType::FLOAT);
        }
    }

    void createWedgeTexCoordsBuffer(
        const MeshType& mesh,
        const auto&     vmw,
        const auto&     vtd,
        const auto&     ftr)
    {
        if (Base::wedgeTexCoordsBufferData()) {
            mVertexWedgeUVBuffer.set(
                Base::wedgeTexCoordsBufferData(),
                Base::vertexNumber() * 2,
                bgfx::Attrib::TexCoord1,
                2,
                PrimitiveType::FLOAT);
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

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(
                    NUM_TRIS * 3);

            triangulatedFaceIndicesToBuffer(
                mesh, buffer, indexMap, MatrixStorageType::ROW_MAJOR, NUM_TRIS);
            replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
                mesh, vtd, ftr, indexMap, buffer);

            mTriangleIndexBuffer.set(
                buffer, NUM_TRIS * 3, true, releaseFn);
        }
        // WAS:
        // if (Base::triangleBufferData()) {
        //     mTriangleIndexBuffer.set(
        //         Base::triangleBufferData(), Base::triangleBufferSize());
        // }
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<float>(
                            NUM_TRIS * 3);

                    triangulatedFaceNormalsToBuffer(
                        mesh, buffer, indexMap, MatrixStorageType::ROW_MAJOR);

                    mTriangleNormalBuffer.setForCompute(
                        buffer,
                        NUM_TRIS * 3,
                        PrimitiveType::FLOAT,
                        bgfx::Access::Read,
                        releaseFn);
                }
            }
        }
        // WAS:
        // if (Base::triangleNormalBufferData()) {
        //     mTriangleNormalBuffer.setForCompute(
        //         Base::triangleNormalBufferData(),
        //         Base::triangleNumber() * 3,
        //         PrimitiveType::FLOAT);
        // }
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<uint>(NUM_TRIS);

                    triangulatedFaceColorsToBuffer(
                        mesh, buffer, indexMap, Color::Format::ABGR);

                    mTriangleColorBuffer.setForCompute(
                        buffer,
                        NUM_TRIS,
                        PrimitiveType::UINT,
                        bgfx::Access::Read,
                        releaseFn);
                }
            }
        }
        // WAS:
        // if (Base::triangleColorBufferData()) {
        //     mTriangleColorBuffer.setForCompute(
        //         Base::triangleColorBufferData(),
        //         Base::triangleNumber(),
        //         PrimitiveType::UINT);
        // }
    }

    void createWedgeTextureIndicesBuffer(
        const MeshType&          mesh,
        const TriPolyIndexBiMap& indexMap)
    {
        if (Base::wedgeTextureIDsBufferData()) {
            mTriangleTextureIndexBuffer.setForCompute(
                Base::wedgeTextureIDsBufferData(),
                Base::triangleNumber(),
                PrimitiveType::UINT);
        }
    }

    void createEdgeIndicesBuffer(const MeshType& mesh)
    {
        if constexpr(vcl::HasEdges<MeshType>) {
            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(mesh.edgeNumber() * 2);

            edgeIndicesToBuffer(mesh, buffer);

            mEdgeIndexBuffer.set(
                 buffer, mesh.edgeNumber() * 2);
        }
        // WAS:
        // if (Base::edgeBufferData()) {
        //     mEdgeIndexBuffer.set(
        //         Base::edgeBufferData(), Base::edgeBufferSize());
        // }
    }

    void createEdgeNormalsBuffer(const MeshType& mesh)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
            if (mBuffersToFill[toUnderlying(EDGE_NORMALS)]) {
                if (vcl::isPerEdgeNormalAvailable(mesh)) {
                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<float>(
                            mesh.edgeNumber() * 3);

                    edgeNormalsToBuffer(mesh, buffer);

                    mEdgeNormalBuffer.setForCompute(
                        buffer,
                        mesh.edgeNumber() * 3,
                        PrimitiveType::FLOAT,
                        bgfx::Access::Read,
                        releaseFn);
                }
            }
        }
        // WAS:
        // if (Base::edgeNormalBufferData()) {
        //     mEdgeNormalBuffer.setForCompute(
        //         Base::edgeNormalBufferData(),
        //         Base::edgeNumber() * 3,
        //         PrimitiveType::FLOAT);
        // }
    }

    void createEdgeColorsBuffer(const MeshType& mesh)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
            if (mBuffersToFill[toUnderlying(EDGE_COLORS)]) {
                if (vcl::isPerEdgeColorAvailable(mesh)) {
                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<uint>(
                            mesh.edgeNumber());

                    edgeColorsToBuffer(mesh, buffer, Color::Format::ABGR);

                    mEdgeColorBuffer.setForCompute(
                        buffer,
                        mesh.edgeNumber(),
                        PrimitiveType::UINT,
                        bgfx::Access::Read,
                        releaseFn);
                }
            }
        }
        // WAS:
        // if (Base::edgeColorBufferData()) {
        //     mEdgeColorBuffer.setForCompute(
        //         Base::edgeColorBufferData(),
        //         Base::edgeNumber(),
        //         PrimitiveType::UINT);
        // }
    }

    void createWireframeIndicesBuffer(const MeshType& mesh)
    {
        if (Base::wireframeBufferData()) {
            mWireframeIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::wireframeBufferData(),
                    Base::wireframeBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }
    }

    void createTextureUnits(const MeshType& mesh)
    {
        if (Base::textureNumber() > 0) {
            mTexturesH.reserve(Base::textureNumber());

            for (uint i = 0; i < Base::textureNumber(); ++i) {
                vcl::Point2i tSize = Base::textureSize(i);

                uint tBufSize = tSize.x() * tSize.y() * 4;

                auto th = bgfx::createTexture2D(
                    tSize.x(),
                    tSize.y(),
                    false,
                    1,
                    bgfx::TextureFormat::RGBA8,
                    0,
                    bgfx::makeRef(Base::textureBufferData(i), tBufSize));

                std::string uniformName = "s_tex" + std::to_string(i);

                auto uh = bgfx::createUniform(
                    uniformName.c_str(), bgfx::UniformType::Sampler);

                mTexturesH.push_back(std::make_pair(th, uh));
            }
        }
    }

    void destroyBGFXBuffers()
    {
        if (bgfx::isValid(mWireframeIndexBH))
            bgfx::destroy(mWireframeIndexBH);

        for (auto [th, uh] : mTexturesH) {
            bgfx::destroy(th);
            bgfx::destroy(uh);
        }
        mTexturesH.clear();
    }

    template<typename T>
    std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(
        uint size)
    {
        T* buffer = new T[size];

        return std::make_pair(buffer, [](void* ptr, void*) {
            delete[] static_cast<T*>(ptr);
        });
    }

};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
