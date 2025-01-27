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
        mVertexCoordsBuffer.set(
            Base::vertexBufferData(),
            Base::vertexNumber() * 3,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT);

        // vertex buffer (normals)
        if (Base::vertexNormalBufferData()) {
            mVertexNormalsBuffer.set(
                Base::vertexNormalBufferData(),
                Base::vertexNumber() * 3,
                bgfx::Attrib::Normal,
                3,
                PrimitiveType::FLOAT);
        }

        // vertex buffer (colors)
        if (Base::vertexColorBufferData()) {
            mVertexColorsBuffer.set(
                Base::vertexColorBufferData(),
                Base::vertexNumber() * 4,
                bgfx::Attrib::Color0,
                4,
                PrimitiveType::UCHAR,
                true);
        }

        // vertex buffer (UVs)
        if (Base::vertexTexCoordsBufferData()) {
            mVertexUVBuffer.set(
                Base::vertexTexCoordsBufferData(),
                Base::vertexNumber() * 2,
                bgfx::Attrib::TexCoord0,
                2,
                PrimitiveType::FLOAT);
        }

        // vertex wedges buffer (duplicated vertices)
        if (Base::wedgeTexCoordsBufferData()) {
            mVertexWedgeUVBuffer.set(
                Base::wedgeTexCoordsBufferData(),
                Base::vertexNumber() * 2,
                bgfx::Attrib::TexCoord1,
                2,
                PrimitiveType::FLOAT);
        }

        // triangle index buffer
        if (Base::triangleBufferData()) {
            mTriangleIndexBuffer.set(
                Base::triangleBufferData(), Base::triangleBufferSize());
        }

        // triangle normal buffer
        if (Base::triangleNormalBufferData()) {
            mTriangleNormalBuffer.setForCompute(
                Base::triangleNormalBufferData(),
                Base::triangleNumber() * 3,
                PrimitiveType::FLOAT);
        }

        // triangle color buffer
        if (Base::triangleColorBufferData()) {
            mTriangleColorBuffer.setForCompute(
                Base::triangleColorBufferData(),
                Base::triangleNumber(),
                PrimitiveType::UINT);
        }

        // triangle wedge UV buffer
        if (Base::wedgeTexCoordsBufferData()) {
            assert(Base::wedgeTextureIDsBufferData());

            mTriangleTextureIndexBuffer.setForCompute(
                Base::wedgeTextureIDsBufferData(),
                Base::triangleNumber(),
                PrimitiveType::UINT);
        }

        // edge index buffer
        if (Base::edgeBufferData()) {
            mEdgeIndexBuffer.set(
                Base::edgeBufferData(), Base::edgeBufferSize());
        }

        // edge normal buffer
        if (Base::edgeNormalBufferData()) {
            mEdgeNormalBuffer.setForCompute(
                Base::edgeNormalBufferData(),
                Base::edgeNumber() * 3,
                PrimitiveType::FLOAT);
        }

        // edge color buffer
        if (Base::edgeColorBufferData()) {
            mEdgeColorBuffer.setForCompute(
                Base::edgeColorBufferData(),
                Base::edgeNumber(),
                PrimitiveType::UINT);
        }

        // wireframe index buffer
        if (Base::wireframeBufferData()) {
            mWireframeIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::wireframeBufferData(),
                    Base::wireframeBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }

        // textures
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
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
