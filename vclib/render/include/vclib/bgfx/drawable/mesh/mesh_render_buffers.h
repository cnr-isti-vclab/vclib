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
#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/texture_unit.h>
#include <vclib/render/drawable/mesh/mesh_buffer_id.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/space/core/image.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers
{
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

    GPUGeneratedLines mWireframeBH;

    std::vector<std::unique_ptr<TextureUnit>> mTextureUnits;

    DrawableMeshUniforms mMeshUniforms;

public:
    MeshRenderBuffers() = default;

    MeshRenderBuffers(
        const MeshType& mesh,
        BuffersToFill   buffersToFill = BUFFERS_TO_FILL_ALL) :
            mBuffersToFill(buffersToFill)
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

        mWireframeBH.swap(other.mWireframeBH);
        swap(mTextureUnits, other.mTextureUnits);
        swap(mMeshUniforms, other.mMeshUniforms);
    }

    friend void swap(MeshRenderBuffers& a, MeshRenderBuffers& b) { a.swap(b); }

    void update(const MeshType& mesh)
    {
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
    }

    void setWireframeSettings(const MeshRenderSettings& settings)
    {
        LineSettings& wSettings = mWireframeBH.settings();
        wSettings.setThickness(settings.wireframeWidth());

        if (settings.isWireframeColorUserDefined()) {
            vcl::Color generalColor = settings.wireframeUserColor();
            wSettings.setGeneralColor(LinesVertex::COLOR(
                generalColor.redF(),
                generalColor.greenF(),
                generalColor.blueF(),
                generalColor.alphaF()));
            wSettings.setColorToUse(LineColorToUse::GENERAL_COLOR);
        }

        if (settings.isWireframeColorPerMesh()) {
            const float* colorPerMesh = mMeshUniforms.currentMeshColor();
            wSettings.setGeneralColor(LinesVertex::COLOR(
                colorPerMesh[0],
                colorPerMesh[1],
                colorPerMesh[2],
                colorPerMesh[3]));
            wSettings.setColorToUse(LineColorToUse::GENERAL_COLOR);
        }

        if (settings.isWireframeColorPerVertex()) {
            wSettings.setColorToUse(LineColorToUse::PER_VERTEX_COLOR);
        }
    }

    void drawWireframe(uint viewId) const 
    {
        mWireframeBH.draw(viewId);
    }

    void bindTextures() const
    {
        uint i = VCL_MRB_TEXTURE0; // first slot available is VCL_MRB_TEXTURE0
        for (const auto& ptr : mTextureUnits) {
            ptr->bind(i);
            i++;
        }
    }

    void bindUniforms() const { mMeshUniforms.bind(); }

private:
    void createBGFXBuffers(const MeshType& mesh)
    {
        using enum MeshBufferId;

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

        mMeshUniforms.update(mesh);
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

        mVertexCoordsBuffer.create(
            buffer,
            nv,
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

                    mVertexNormalsBuffer.create(
                        buffer,
                        nv,
                        bgfx::Attrib::Normal,
                        3,
                        PrimitiveType::FLOAT,
                        false,
                        releaseFn);
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<uint>(nv);

                    vertexColorsToBuffer(mesh, buffer, Color::Format::ABGR);
                    appendDuplicateVertexColorsToBuffer(
                        mesh, vtd, buffer, Color::Format::ABGR);

                    mVertexColorsBuffer.create(
                        buffer,
                        nv,
                        bgfx::Attrib::Color0,
                        4,
                        PrimitiveType::UCHAR,
                        true,
                        releaseFn);
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<float>(nv * 2);

                    vertexTexCoordsToBuffer(mesh, buffer);
                    appendDuplicateVertexTexCoordsToBuffer(mesh, vtd, buffer);

                    mVertexUVBuffer.create(
                        buffer,
                        nv,
                        bgfx::Attrib::TexCoord0,
                        2,
                        PrimitiveType::FLOAT,
                        false,
                        releaseFn);
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<float>(nv * 2);

                    wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
                        mesh, vmw, ftr, buffer);

                    mVertexWedgeUVBuffer.create(
                        buffer,
                        nv,
                        bgfx::Attrib::TexCoord1,
                        2,
                        PrimitiveType::FLOAT,
                        false,
                        releaseFn);
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

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(NUM_TRIS * 3);

            triangulatedFaceIndicesToBuffer(
                mesh, buffer, indexMap, MatrixStorageType::ROW_MAJOR, NUM_TRIS);
            replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
                mesh, vtd, ftr, indexMap, buffer);

            mTriangleIndexBuffer.create(buffer, NUM_TRIS * 3, true, releaseFn);
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<float>(NUM_TRIS * 3);

                    triangulatedFaceNormalsToBuffer(
                        mesh, buffer, indexMap, MatrixStorageType::ROW_MAJOR);

                    mTriangleNormalBuffer.createForCompute(
                        buffer,
                        NUM_TRIS * 3,
                        PrimitiveType::FLOAT,
                        bgfx::Access::Read,
                        releaseFn);
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<uint>(NUM_TRIS);

                    triangulatedFaceColorsToBuffer(
                        mesh, buffer, indexMap, Color::Format::ABGR);

                    mTriangleColorBuffer.createForCompute(
                        buffer,
                        NUM_TRIS,
                        PrimitiveType::UINT,
                        bgfx::Access::Read,
                        releaseFn);
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

                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<uint>(NUM_TRIS);

                    triangulatedFaceWedgeTexCoordIndicesToBuffer(
                        mesh, buffer, indexMap);

                    mTriangleTextureIndexBuffer.createForCompute(
                        buffer,
                        NUM_TRIS,
                        PrimitiveType::UINT,
                        bgfx::Access::Read,
                        releaseFn);
                }
            }
        }
    }

    void createEdgeIndicesBuffer(const MeshType& mesh)
    {
        if constexpr (vcl::HasEdges<MeshType>) {
            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(mesh.edgeNumber() * 2);

            edgeIndicesToBuffer(mesh, buffer);

            mEdgeIndexBuffer.create(buffer, mesh.edgeNumber() * 2);
        }
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

                    mEdgeNormalBuffer.createForCompute(
                        buffer,
                        mesh.edgeNumber() * 3,
                        PrimitiveType::FLOAT,
                        bgfx::Access::Read,
                        releaseFn);
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
                    auto [buffer, releaseFn] =
                        getAllocatedBufferAndReleaseFn<uint>(mesh.edgeNumber());

                    edgeColorsToBuffer(mesh, buffer, Color::Format::ABGR);

                    mEdgeColorBuffer.createForCompute(
                        buffer,
                        mesh.edgeNumber(),
                        PrimitiveType::UINT,
                        bgfx::Access::Read,
                        releaseFn);
                }
            }
        }
    }

    void createWireframeIndicesBuffer(const MeshType& mesh)
    {
        using enum MeshBufferId;

        if constexpr (vcl::HasFaces<MeshType>) {
            // TODO: DATA DUPLICATION
            // Heavy refactoring needed here
            std::vector<LinesVertex> wireframe;
            wireframe.reserve(mesh.faceNumber() * 3);
            for (const auto& f : mesh.faces()) {
                for (uint i = 0; i < f.vertexNumber(); ++i) {
                    const auto& p0 = f.vertex(i)->coord();
                    const auto& p1 = f.vertexMod((i + 1))->coord();
                    const vcl::Color& c0 = f.vertex(i)->color();
                    const vcl::Color& c1 = f.vertexMod((i + 1))->color();
                    // TODO: NORMALS CAN ALSO NOT BE AVAILABLE
                    const auto& n0 = f.vertex(i)->normal();
                    const auto& n1 = f.vertexMod((i + 1))->normal();

                    wireframe.push_back(LinesVertex(
                        p0.x(), p0.y(), p0.z(),
                        LinesVertex::COLOR(
                            c0.redF(), c0.greenF(), c0.blueF(), c0.alphaF()),
                        n0.x(), n0.y(), n0.z()));

                    wireframe.push_back(LinesVertex(
                        p1.x(), p1.y(), p1.z(),
                        LinesVertex::COLOR(
                            c1.redF(), c1.greenF(), c1.blueF(), c1.alphaF()),
                        n1.x(), n1.y(), n1.z()));
                }
            }
            // wireframe index buffer
            mWireframeBH.update(wireframe);

            // TODO: Should Be:
            // const uint NUM_EDGES =
            // vcl::countPerFaceVertexReferences(mesh);

            // auto [buffer, releaseFn] =
            //    getAllocatedBufferAndReleaseFn<uint>(NUM_EDGES * 2);

            // wireframeIndicesToBuffer(mesh, buffer);

            // mWireframeIndexBuffer.create(buffer, NUM_EDGES * 2, true,
            // releaseFn);
        }
    }

    void createTextureUnits(const MeshType& mesh)
    {
        if constexpr (vcl::HasTexturePaths<MeshType>) {
            mTextureUnits.reserve(mesh.textureNumber());
            for (uint i = 0; i < mesh.textureNumber(); ++i) {
                vcl::Image txt;
                if constexpr (vcl::HasTextureImages<MeshType>) {
                    if (mesh.texture(i).image().isNull()) {
                        txt = vcl::Image(
                            mesh.meshBasePath() + mesh.texturePath(i));
                    }
                    else {
                        txt = mesh.texture(i).image();
                    }
                }
                else {
                    txt = vcl::Image(mesh.meshBasePath() + mesh.texturePath(i));
                }
                txt.mirror();

                const uint size = txt.width() * txt.height();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(size);

                const uint* tdata = reinterpret_cast<const uint*>(txt.data());

                std::copy(tdata, tdata + size, buffer);

                auto tu = std::make_unique<TextureUnit>();
                tu->set(
                    buffer,
                    vcl::Point2i(txt.width(), txt.height()),
                    "s_tex" + std::to_string(i),
                    false,
                    releaseFn);

                mTextureUnits.push_back(std::move(tu));
            }
        }
    }

    void destroyBGFXBuffers() { mTextureUnits.clear(); }

    template<typename T>
    std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(uint size)
    {
        T* buffer = new T[size];

        return std::make_pair(buffer, [](void* ptr, void*) {
            delete[] static_cast<T*>(ptr);
        });
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
