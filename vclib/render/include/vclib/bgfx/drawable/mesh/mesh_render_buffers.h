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
#include <vclib/bgfx/drawable/drawable_lines.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/texture_unit.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/space/core/image.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers
{
    using MRI = MeshRenderInfo;

    MRI::BuffersBitSet mBuffersToFill = MRI::BUFFERS_ALL;

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

    GPUGeneratedLines mWireframeLines;

    std::vector<std::unique_ptr<TextureUnit>> mTextureUnits;

    DrawableMeshUniforms mMeshUniforms;

    // data used when vertices are duplicated for per-face wedge texcoords:

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
    MeshRenderBuffers() = default;

    MeshRenderBuffers(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToFill = MRI::BUFFERS_ALL) :
            mBuffersToFill(buffersToFill)
    {
        createBGFXBuffers(mesh, buffersToFill);
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

        mWireframeLines.swap(other.mWireframeLines);
        swap(mTextureUnits, other.mTextureUnits);
        swap(mMeshUniforms, other.mMeshUniforms);
        swap(mVertWedgeMap, other.mVertWedgeMap);
        swap(mVertsToDuplicate, other.mVertsToDuplicate);
        swap(mFacesToReassign, other.mFacesToReassign);
        swap(mIndexMap, other.mIndexMap);
    }

    friend void swap(MeshRenderBuffers& a, MeshRenderBuffers& b) { a.swap(b); }

    void update(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToUpdate = MRI::BUFFERS_ALL)
    {
        destroyBGFXBuffers(buffersToUpdate);
        createBGFXBuffers(mesh, buffersToUpdate);
    }

    void bindVertexBuffers(const MeshRenderSettings& mrs) const
    {
        // bgfx allows a maximum number of 4 vertex streams...

        mVertexCoordsBuffer.bind(0);
        mVertexNormalsBuffer.bind(1);
        mVertexColorsBuffer.bind(2);

        if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_VERTEX_TEX)) {
            mVertexUVBuffer.bind(3);
        }
        else if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_WEDGE_TEX)) {
            mVertexWedgeUVBuffer.bind(3);
        }
    }

    void bindIndexBuffers(
        MRI::Buffers indexBufferToBind = MRI::Buffers::TRIANGLES) const
    {
        using enum MRI::Buffers;

        if (indexBufferToBind == TRIANGLES) {
            mTriangleIndexBuffer.bind();

            mTriangleNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

            mTriangleColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);

            mTriangleTextureIndexBuffer.bind(
                VCL_MRB_TRIANGLE_TEXTURE_ID_BUFFER);
        }
        else if (indexBufferToBind == EDGES) {
            mEdgeIndexBuffer.bind();

            mEdgeNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

            mEdgeColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);
        }
    }

    void setWireframeSettings(const MeshRenderSettings& settings)
    {
        using enum MRI::Wireframe;

        LineSettings& wSettings = mWireframeLines.settings();
        wSettings.setThickness(settings.wireframeWidth());

        if (settings.isWireframe(COLOR_USER)) {
            vcl::Color generalColor = settings.wireframeUserColor();
            wSettings.setGeneralColor(LinesVertex::COLOR(
                generalColor.redF(),
                generalColor.greenF(),
                generalColor.blueF(),
                generalColor.alphaF()));
            wSettings.setColorToUse(LineColorToUse::GENERAL_COLOR);
        }

        if (settings.isWireframe(COLOR_MESH)) {
            const float* colorPerMesh = mMeshUniforms.currentMeshColor();
            wSettings.setGeneralColor(LinesVertex::COLOR(
                colorPerMesh[0],
                colorPerMesh[1],
                colorPerMesh[2],
                colorPerMesh[3]));
            wSettings.setColorToUse(LineColorToUse::GENERAL_COLOR);
        }

        if (settings.isWireframe(COLOR_VERTEX)) {
            wSettings.setColorToUse(LineColorToUse::PER_VERTEX_COLOR);
        }
    }

    void drawWireframe(uint viewId) const 
    {
        mWireframeLines.draw(viewId);
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
    void createBGFXBuffers(
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
            mMeshUniforms.update(mesh);
        }
    }

    void createVertexCoordsBuffer(const MeshType& mesh)
    {
        uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 3);

        vertexCoordsToBuffer(mesh, buffer);
        appendDuplicateVertexCoordsToBuffer(mesh, mVertsToDuplicate, buffer);

        mVertexCoordsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT,
            false,
            releaseFn);
    }

    void createVertexNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(nv * 3);

                vertexNormalsToBuffer(mesh, buffer);
                appendDuplicateVertexNormalsToBuffer(
                    mesh, mVertsToDuplicate, buffer);

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

    void createVertexColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(nv);

                vertexColorsToBuffer(mesh, buffer, Color::Format::ABGR);
                appendDuplicateVertexColorsToBuffer(
                    mesh, mVertsToDuplicate, buffer, Color::Format::ABGR);

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

    void createVertexTexCoordsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(nv * 2);

                vertexTexCoordsToBuffer(mesh, buffer);
                appendDuplicateVertexTexCoordsToBuffer(
                    mesh, mVertsToDuplicate, buffer);

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

    void createWedgeTexCoordsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint nv = mesh.vertexNumber() + mVertsToDuplicate.size();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(nv * 2);

                wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
                    mesh, mVertWedgeMap, mFacesToReassign, buffer);

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

    void createTriangleIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            const uint NUM_TRIS = vcl::countTriangulatedTriangles(mesh);

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(NUM_TRIS * 3);

            triangulatedFaceIndicesToBuffer(
                mesh,
                buffer,
                mIndexMap,
                MatrixStorageType::ROW_MAJOR,
                NUM_TRIS);
            replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
                mesh, mVertsToDuplicate, mFacesToReassign, mIndexMap, buffer);

            mTriangleIndexBuffer.create(buffer, NUM_TRIS * 3, true, releaseFn);
        }
    }

    void createTriangleNormalsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceNormal<MeshType>) {
            if (vcl::isPerFaceNormalAvailable(mesh)) {
                const uint NUM_TRIS = mIndexMap.triangleNumber();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(NUM_TRIS * 3);

                triangulatedFaceNormalsToBuffer(
                    mesh, buffer, mIndexMap, MatrixStorageType::ROW_MAJOR);

                mTriangleNormalBuffer.createForCompute(
                    buffer,
                    NUM_TRIS * 3,
                    PrimitiveType::FLOAT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createTriangleColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceColor<MeshType>) {
            if (vcl::isPerFaceColorAvailable(mesh)) {
                const uint NUM_TRIS = mIndexMap.triangleNumber();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(NUM_TRIS);

                triangulatedFaceColorsToBuffer(
                    mesh, buffer, mIndexMap, Color::Format::ABGR);

                mTriangleColorBuffer.createForCompute(
                    buffer,
                    NUM_TRIS,
                    PrimitiveType::UINT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createWedgeTextureIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                const uint NUM_TRIS = mIndexMap.triangleNumber();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(NUM_TRIS);

                triangulatedFaceWedgeTexCoordIndicesToBuffer(
                    mesh, buffer, mIndexMap);

                mTriangleTextureIndexBuffer.createForCompute(
                    buffer,
                    NUM_TRIS,
                    PrimitiveType::UINT,
                    bgfx::Access::Read,
                    releaseFn);
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
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
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

    void createEdgeColorsBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
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

    void createWireframeIndicesBuffer(const MeshType& mesh)
    {
        using enum MRI::Buffers;

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
            mWireframeLines.setPoints(wireframe);

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

    void destroyBGFXBuffers(
        MRI::BuffersBitSet buffersToDestroy = MRI::BUFFERS_ALL)
    {
        using enum MRI::Buffers;

        if (buffersToDestroy[toUnderlying(TEXTURES)])
            mTextureUnits.clear();
    }

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
