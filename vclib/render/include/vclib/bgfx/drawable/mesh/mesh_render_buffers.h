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

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/texture_unit.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/space/core/image.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers : public MeshRenderData<MeshType>
{
    using Base = MeshRenderData<MeshType>;
    using MRI = MeshRenderInfo;

    VertexBuffer mVertexCoordsBuffer;
    VertexBuffer mVertexNormalsBuffer;
    VertexBuffer mVertexColorsBuffer;
    VertexBuffer mVertexUVBuffer;
    VertexBuffer mVertexWedgeUVBuffer;

    IndexBuffer mTriangleIndexBuffer;
    IndexBuffer mTriangleNormalBuffer;
    IndexBuffer mTriangleColorBuffer;

    IndexBuffer mVertexTextureIndexBuffer;
    IndexBuffer mWedgeTextureIndexBuffer;

    // TODO: manage wireframe with proper lines
    IndexBuffer mEdgeIndexBuffer;
    IndexBuffer mEdgeNormalBuffer;
    IndexBuffer mEdgeColorBuffer;

    // TODO: manage wireframe with proper lines
    IndexBuffer mWireframeIndexBuffer;

    std::vector<std::unique_ptr<TextureUnit>> mTextureUnits;

    DrawableMeshUniforms mMeshUniforms;

public:
    MeshRenderBuffers() = default;

    MeshRenderBuffers(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToFill = MRI::BUFFERS_ALL) :
            Base(buffersToFill)
    {
        Base::update(mesh, buffersToFill);
    }

    MeshRenderBuffers(const MeshRenderBuffers& other) = delete;

    MeshRenderBuffers(MeshRenderBuffers&& other) { swap(other); }

    MeshRenderBuffers& operator=(const MeshRenderBuffers& other) = delete;

    MeshRenderBuffers& operator=(MeshRenderBuffers&& other)
    {
        swap(other);
        return *this;
    }

    void swap(MeshRenderBuffers& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mVertexCoordsBuffer, other.mVertexCoordsBuffer);
        swap(mVertexNormalsBuffer, other.mVertexNormalsBuffer);
        swap(mVertexColorsBuffer, other.mVertexColorsBuffer);
        swap(mVertexUVBuffer, other.mVertexUVBuffer);
        swap(mVertexWedgeUVBuffer, other.mVertexWedgeUVBuffer);
        swap(mTriangleIndexBuffer, other.mTriangleIndexBuffer);
        swap(mTriangleNormalBuffer, other.mTriangleNormalBuffer);
        swap(mTriangleColorBuffer, other.mTriangleColorBuffer);
        swap(mVertexTextureIndexBuffer, other.mVertexTextureIndexBuffer);
        swap(mWedgeTextureIndexBuffer, other.mWedgeTextureIndexBuffer);
        swap(mEdgeIndexBuffer, other.mEdgeIndexBuffer);
        swap(mEdgeNormalBuffer, other.mEdgeNormalBuffer);
        swap(mEdgeColorBuffer, other.mEdgeColorBuffer);
        swap(mWireframeIndexBuffer, other.mWireframeIndexBuffer);
        swap(mTextureUnits, other.mTextureUnits);
        swap(mMeshUniforms, other.mMeshUniforms);
    }

    friend void swap(MeshRenderBuffers& a, MeshRenderBuffers& b) { a.swap(b); }

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
        const MeshRenderSettings& mrs,
        MRI::Buffers indexBufferToBind = MRI::Buffers::TRIANGLES) const
    {
        using enum MRI::Buffers;

        if (indexBufferToBind == TRIANGLES) {
            mTriangleIndexBuffer.bind();

            mTriangleNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

            mTriangleColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);

            if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_VERTEX_TEX)) {
                mVertexTextureIndexBuffer.bind(
                    VCL_MRB_TRIANGLE_TEXTURE_ID_BUFFER);
            }
            else if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_WEDGE_TEX)) {
                mWedgeTextureIndexBuffer.bind(
                    VCL_MRB_TRIANGLE_TEXTURE_ID_BUFFER);
            }

        }
        else if (indexBufferToBind == EDGES) {
            mEdgeIndexBuffer.bind();

            mEdgeNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

            mEdgeColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);
        }
        else if (indexBufferToBind == WIREFRAME) {
            mWireframeIndexBuffer.bind();
        }
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
    void createVertexCoordsBuffer(const MeshType& mesh) override final
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 3);

        Base::fillVertexCoords(mesh, buffer);

        mVertexCoordsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT,
            false,
            releaseFn);
    }

    void createVertexNormalsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                uint nv = Base::numVerts();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(nv * 3);

                Base::fillVertexNormals(mesh, buffer);

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

    void createVertexColorsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(mesh)) {
                uint nv = Base::numVerts();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(nv);

                Base::fillVertexColors(mesh, buffer, Color::Format::ABGR);

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

    void createVertexTexCoordsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                uint nv = Base::numVerts();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(nv * 2);

                Base::fillVertexTexCoords(mesh, buffer);

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

    void createWedgeTexCoordsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint nv = Base::numVerts();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(nv * 2);

                Base::fillWedgeTexCoords(mesh, buffer);

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

    void createTriangleIndicesBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            uint nt = Base::numTris();

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(nt * 3);

            Base::fillTriangleIndices(mesh, buffer);

            mTriangleIndexBuffer.create(buffer, nt * 3, true, releaseFn);
        }
    }

    void createTriangleNormalsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceNormal<MeshType>) {
            if (vcl::isPerFaceNormalAvailable(mesh)) {
                uint nt = Base::numTris();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(nt * 3);

                Base::fillTriangleNormals(mesh, buffer);

                mTriangleNormalBuffer.createForCompute(
                    buffer,
                    nt * 3,
                    PrimitiveType::FLOAT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createTriangleColorsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceColor<MeshType>) {
            if (vcl::isPerFaceColorAvailable(mesh)) {
                uint nt = Base::numTris();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(nt);

                Base::fillTriangleColors(mesh, buffer, Color::Format::ABGR);

                mTriangleColorBuffer.createForCompute(
                    buffer,
                    nt,
                    PrimitiveType::UINT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createVertexTextureIndicesBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr(HasFaces<MeshType> && HasPerVertexTexCoord<MeshType>) {
            if (isPerVertexTexCoordAvailable(mesh)) {
                uint nt = Base::numTris();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(nt);

                Base::fillVertexTextureIndices(mesh, buffer);

                mVertexTextureIndexBuffer.createForCompute(
                    buffer,
                    nt,
                    PrimitiveType::UINT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createWedgeTextureIndicesBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint nt = Base::numTris();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(nt);

                Base::fillWedgeTextureIndices(mesh, buffer);

                mWedgeTextureIndexBuffer.createForCompute(
                    buffer,
                    nt,
                    PrimitiveType::UINT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createEdgeIndicesBuffer(const MeshType& mesh) override final
    {
        if constexpr (vcl::HasEdges<MeshType>) {
            uint ne = Base::numEdges();

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(ne * 2);

            Base::fillEdgeIndices(mesh, buffer);

            mEdgeIndexBuffer.create(buffer, ne * 2);
        }
    }

    void createEdgeNormalsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
            if (vcl::isPerEdgeNormalAvailable(mesh)) {
                uint ne = Base::numEdges();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<float>(
                        ne * 3);

                Base::fillEdgeNormals(mesh, buffer);

                mEdgeNormalBuffer.createForCompute(
                    buffer,
                    ne * 3,
                    PrimitiveType::FLOAT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createEdgeColorsBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
            if (vcl::isPerEdgeColorAvailable(mesh)) {
                uint ne = Base::numEdges();

                auto [buffer, releaseFn] =
                    getAllocatedBufferAndReleaseFn<uint>(ne);

                Base::fillEdgeColors(mesh, buffer, Color::Format::ABGR);

                mEdgeColorBuffer.createForCompute(
                    buffer,
                    ne,
                    PrimitiveType::UINT,
                    bgfx::Access::Read,
                    releaseFn);
            }
        }
    }

    void createWireframeIndicesBuffer(const MeshType& mesh) override final
    {
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            const uint nw = Base::numWireframeLines();

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(nw * 2);

            Base::fillWireframeIndices(mesh, buffer);

            mWireframeIndexBuffer.create(
                buffer, nw * 2, true, releaseFn);
        }
    }

    void createTextureUnits(const MeshType& mesh) override final
    {
        if constexpr (vcl::HasTexturePaths<MeshType>) {
            mTextureUnits.clear();
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

    void createMeshUniforms(const MeshType& mesh) override final
    {
        mMeshUniforms.update(mesh);
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
