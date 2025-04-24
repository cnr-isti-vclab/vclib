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

#include <vclib/algorithms/core/create.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/texture_unit.h>
#include <vclib/io/image/load.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/bgfx/context.h>
#include <vclib/space/core/image.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept Mesh>
class MeshRenderBuffers : public MeshRenderData<MeshRenderBuffers<Mesh>>
{
    using MeshType = Mesh;
    using Base     = MeshRenderData<MeshRenderBuffers<MeshType>>;
    using MRI      = MeshRenderInfo;

    friend Base;

    VertexBuffer mVertexCoordsBuffer;
    VertexBuffer mVertexNormalsBuffer;
    VertexBuffer mVertexColorsBuffer;
    VertexBuffer mVertexUVBuffer;
    VertexBuffer mVertexWedgeUVBuffer;

    // point splatting
    IndexBuffer  mVertexQuadIndexBuffer;
    bgfx::DynamicVertexBufferHandle mVertexQuadBuffer = BGFX_INVALID_HANDLE;

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

    ~MeshRenderBuffers() {
        if (bgfx::isValid(mVertexQuadBuffer)) {
            bgfx::destroy(mVertexQuadBuffer);
            mVertexQuadBuffer = BGFX_INVALID_HANDLE;
        }
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
        mVertexCoordsBuffer.bind(VCL_MRB_VERTEX_POSITION_STREAM);
        mVertexNormalsBuffer.bind(VCL_MRB_VERTEX_NORMAL_STREAM);
        mVertexColorsBuffer.bind(VCL_MRB_VERTEX_COLOR_STREAM);

        if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_VERTEX_TEX)) {
            mVertexUVBuffer.bind(VCL_MRB_VERTEX_TEXCOORD_STREAM);
        }
        else if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_WEDGE_TEX)) {
            mVertexWedgeUVBuffer.bind(VCL_MRB_VERTEX_TEXCOORD_STREAM);
        }
    }

    // to generate splats
    void bindComputeVertexBuffers(const MeshRenderSettings& mrs) const
    {
        mVertexCoordsBuffer.bindCompute(
            VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
        mVertexNormalsBuffer.bindCompute(
            VCL_MRB_VERTEX_NORMAL_STREAM, bgfx::Access::Read);
        mVertexColorsBuffer.bindCompute(
            VCL_MRB_VERTEX_COLOR_STREAM, bgfx::Access::Read);

        bgfx::setBuffer(4, mVertexQuadBuffer, bgfx::Access::Write);
    }

    // to draw splats
    void bindVertexQuadBuffer() const
    {
        assert(bgfx::isValid(mVertexQuadBuffer));
        bgfx::setVertexBuffer(0, mVertexQuadBuffer);
        mVertexQuadIndexBuffer.bind();
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
    void setVertexCoordsBuffer(const MeshType& mesh) // override
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


        // Creates the buffers to be used with compute for splatting
        if (Context::instance().supportsCompute())
        {
            // destroy the previous buffer if it exists
            if (bgfx::isValid(mVertexQuadBuffer))
            {
                bgfx::destroy(mVertexQuadBuffer);
            }

            // create a layout <coordinates, colors, normals, float>
            // 2 X vec4
            bgfx::VertexLayout layout; 
            layout.begin()
                .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
            .end();

            // create the dynamic vertex buffer
            mVertexQuadBuffer = bgfx::createDynamicVertexBuffer(
                mesh.vertexNumber() * 4,
                layout,
                BGFX_BUFFER_COMPUTE_WRITE);

            setVertexQuadBuffer(mesh);
        }
    }

    /**
     * @brief The function allocates and fills a GPU index buffer to render
     * a quad for each vertex of the mesh.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexQuadBuffer(const MeshType& mesh)
    {
        const uint totalIndices = mesh.vertexNumber() * 6;

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(totalIndices);

        Base::fillVertexQuadIndices(mesh, buffer);

        mVertexQuadIndexBuffer.create(
            buffer,
            totalIndices,
            true,
            releaseFn);
        
        assert(mVertexQuadIndexBuffer.isValid());
    }

    void setVertexNormalsBuffer(const MeshType& mesh) // override
    {
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

    void setVertexColorsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nv);

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

    void setVertexTexCoordsBuffer(const MeshType& mesh) // override
    {
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

    void setWedgeTexCoordsBuffer(const MeshType& mesh) // override
    {
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

    void setTriangleIndicesBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nt * 3);

        Base::fillTriangleIndices(mesh, buffer);

        mTriangleIndexBuffer.create(buffer, nt * 3, true, releaseFn);
    }

    void setTriangleNormalsBuffer(const MeshType& mesh) // override
    {
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

    void setTriangleColorsBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nt);

        Base::fillTriangleColors(mesh, buffer, Color::Format::ABGR);

        mTriangleColorBuffer.createForCompute(
            buffer, nt, PrimitiveType::UINT, bgfx::Access::Read, releaseFn);
    }

    void setVertexTextureIndicesBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nt);

        Base::fillVertexTextureIndices(mesh, buffer);

        mVertexTextureIndexBuffer.createForCompute(
            buffer, nt, PrimitiveType::UINT, bgfx::Access::Read, releaseFn);
    }

    void setWedgeTextureIndicesBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nt);

        Base::fillWedgeTextureIndices(mesh, buffer);

        mWedgeTextureIndexBuffer.createForCompute(
            buffer, nt, PrimitiveType::UINT, bgfx::Access::Read, releaseFn);
    }

    void setEdgeIndicesBuffer(const MeshType& mesh) // override
    {
        uint ne = Base::numEdges();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(ne * 2);

        Base::fillEdgeIndices(mesh, buffer);

        mEdgeIndexBuffer.create(buffer, ne * 2);
    }

    void setEdgeNormalsBuffer(const MeshType& mesh) // override
    {
        uint ne = Base::numEdges();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(ne * 3);

        Base::fillEdgeNormals(mesh, buffer);

        mEdgeNormalBuffer.createForCompute(
            buffer,
            ne * 3,
            PrimitiveType::FLOAT,
            bgfx::Access::Read,
            releaseFn);
    }

    void setEdgeColorsBuffer(const MeshType& mesh) // override
    {
        uint ne = Base::numEdges();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(ne);

        Base::fillEdgeColors(mesh, buffer, Color::Format::ABGR);

        mEdgeColorBuffer.createForCompute(
            buffer, ne, PrimitiveType::UINT, bgfx::Access::Read, releaseFn);
    }

    void setWireframeIndicesBuffer(const MeshType& mesh) // override
    {
        const uint nw = Base::numWireframeLines();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nw * 2);

        Base::fillWireframeIndices(mesh, buffer);

        mWireframeIndexBuffer.create(buffer, nw * 2, true, releaseFn);
    }

    void setTextureUnits(const MeshType& mesh) // override
    {
        mTextureUnits.clear();
        mTextureUnits.reserve(mesh.textureNumber());
        for (uint i = 0; i < mesh.textureNumber(); ++i) {
            vcl::Image txt;
            if constexpr (vcl::HasTextureImages<MeshType>) {
                if (mesh.texture(i).image().isNull()) {
                    txt = vcl::loadImage(
                        mesh.meshBasePath() + mesh.texturePath(i));
                }
                else {
                    txt = mesh.texture(i).image();
                }
            }
            else {
                txt = vcl::loadImage(mesh.meshBasePath() + mesh.texturePath(i));
            }
            if (txt.isNull()) {
                txt = vcl::createCheckBoardImage(512);
            }

            txt.mirror();

            const uint size = txt.width() * txt.height();
            assert(size > 0);

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

    void setMeshUniforms(const MeshType& mesh) // override
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
