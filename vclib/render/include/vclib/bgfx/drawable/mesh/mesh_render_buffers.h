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

    VertexBuffer mVertexCoordsBuffer;
    VertexBuffer mVertexNormalsBuffer;
    VertexBuffer mVertexColorsBuffer;
    VertexBuffer mVertexUVBuffer;
    VertexBuffer mVertexWedgeUVBuffer;

    bgfx::IndexBufferHandle mTriangleIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mTriangleNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mTriangleColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mTriangleTextureIndexBH = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mEdgeIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mEdgeNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mEdgeColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mWireframeIndexBH = BGFX_INVALID_HANDLE;

    std::vector<std::pair<bgfx::TextureHandle, bgfx::UniformHandle>> mTexturesH;

public:
    MeshRenderBuffers() = default;

    MeshRenderBuffers(const MeshType& mesh, uint buffersToFill = Base::ALL) :
            Base(mesh, buffersToFill)
    {
        createBGFXBuffers();
    }

    MeshRenderBuffers(const MeshRenderBuffers& other) : Base(other)
    {
        // each object has its own bgfx buffers
        createBGFXBuffers();
    }

    MeshRenderBuffers(MeshRenderBuffers&& other) { swap(other); }

    ~MeshRenderBuffers() { destroyBGFXBuffers(); }

    MeshRenderBuffers& operator=(MeshRenderBuffers other)
    {
        swap(other);
        return *this;
    }

    void swap(MeshRenderBuffers& other)
    {
        using std::swap;
        swap((Base&) *this, (Base&) other);
        swap(mVertexCoordsBuffer, other.mVertexCoordsBuffer);
        swap(mVertexNormalsBuffer, other.mVertexNormalsBuffer);
        swap(mVertexColorsBuffer, other.mVertexColorsBuffer);
        swap(mVertexUVBuffer, other.mVertexUVBuffer);
        swap(mVertexWedgeUVBuffer, other.mVertexWedgeUVBuffer);
        swap(mTriangleIndexBH, other.mTriangleIndexBH);
        swap(mTriangleNormalBH, other.mTriangleNormalBH);
        swap(mTriangleColorBH, other.mTriangleColorBH);
        swap(mTriangleTextureIndexBH, other.mTriangleTextureIndexBH);
        swap(mEdgeIndexBH, other.mEdgeIndexBH);
        swap(mEdgeNormalBH, other.mEdgeNormalBH);
        swap(mEdgeColorBH, other.mEdgeColorBH);
        swap(mWireframeIndexBH, other.mWireframeIndexBH);
        swap(mTexturesH, other.mTexturesH);
    }

    friend void swap(MeshRenderBuffers& a, MeshRenderBuffers& b) { a.swap(b); }

    void update(const MeshType& mesh)
    {
        Base::update(mesh);
        destroyBGFXBuffers();
        createBGFXBuffers();
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

    void bindIndexBuffers(uint indexBufferToBind = Base::TRIANGLES) const
    {
        if (indexBufferToBind == Base::TRIANGLES) {
            bgfx::setIndexBuffer(mTriangleIndexBH);

            if (bgfx::isValid(mTriangleColorBH)) { // triangle colors
                bgfx::setBuffer(
                    VCL_MRB_PRIMITIVE_COLOR_BUFFER,
                    mTriangleColorBH,
                    bgfx::Access::Read);
            }

            if (bgfx::isValid(mTriangleNormalBH)) { // triangle normals
                bgfx::setBuffer(
                    VCL_MRB_PRIMITIVE_NORMAL_BUFFER,
                    mTriangleNormalBH,
                    bgfx::Access::Read);
            }

            if (bgfx::isValid(mTriangleTextureIndexBH)) { // tri texture indices
                bgfx::setBuffer(
                    VCL_MRB_TRIANGLE_TEXTURE_ID_BUFFER,
                    mTriangleTextureIndexBH,
                    bgfx::Access::Read);
            }
        }
        else if (indexBufferToBind == Base::EDGES) {
            bgfx::setIndexBuffer(mEdgeIndexBH);

            if (bgfx::isValid(mEdgeColorBH)) { // edge colors
                bgfx::setBuffer(
                    VCL_MRB_PRIMITIVE_COLOR_BUFFER,
                    mEdgeColorBH,
                    bgfx::Access::Read);
            }

            if (bgfx::isValid(mEdgeNormalBH)) { // edge normals
                bgfx::setBuffer(
                    VCL_MRB_PRIMITIVE_NORMAL_BUFFER,
                    mEdgeNormalBH,
                    bgfx::Access::Read);
            }
        }
        else if (indexBufferToBind == Base::WIREFRAME) {
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
    void createBGFXBuffers()
    {
        mVertexCoordsBuffer.set(
            Base::vertexBufferData(),
            Base::vertexNumber() * 3,
            bgfx::Attrib::Position,
            3,
            bgfx::AttribType::Float);

        mVertexNormalsBuffer.set(
            Base::vertexNormalBufferData(),
            Base::vertexNumber() * 3,
            bgfx::Attrib::Normal,
            3,
            bgfx::AttribType::Float);

        mVertexColorsBuffer.set(
            Base::vertexColorBufferData(),
            Base::vertexNumber() * 4,
            bgfx::Attrib::Color0,
            4,
            bgfx::AttribType::Uint8,
            true);

        mVertexUVBuffer.set(
            Base::vertexTexCoordsBufferData(),
            Base::vertexNumber() * 2,
            bgfx::Attrib::TexCoord0,
            2,
            bgfx::AttribType::Float);

        // vertex wedges buffer (duplicated vertices)
        mVertexWedgeUVBuffer.set(
            Base::wedgeTexCoordsBufferData(),
            Base::vertexNumber() * 2,
            bgfx::Attrib::TexCoord1,
            2,
            bgfx::AttribType::Float);

        // triangle index buffer
        if (Base::triangleBufferData()) {
            mTriangleIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::triangleBufferData(),
                    Base::triangleBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }

        // triangle normal buffer
        if (Base::triangleNormalBufferData()) {
            mTriangleNormalBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::triangleNormalBufferData(),
                    Base::triangleNumber() * 3 * sizeof(float)),
                BGFX_BUFFER_COMPUTE_FORMAT_32X1 | BGFX_BUFFER_COMPUTE_READ |
                    BGFX_BUFFER_COMPUTE_TYPE_FLOAT);
        }

        // triangle color buffer
        if (Base::triangleColorBufferData()) {
            mTriangleColorBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::triangleColorBufferData(),
                    Base::triangleNumber() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32 | BGFX_BUFFER_COMPUTE_READ);
        }

        // triangle wedge UV buffer
        if (Base::wedgeTexCoordsBufferData()) {
            assert(Base::wedgeTextureIDsBufferData());

            mTriangleTextureIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::wedgeTextureIDsBufferData(),
                    Base::triangleNumber() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32 | BGFX_BUFFER_COMPUTE_READ);
        }

        // edge index buffer
        if (Base::edgeBufferData()) {
            mEdgeIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::edgeBufferData(),
                    Base::edgeBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }

        // edge normal buffer
        if (Base::edgeNormalBufferData()) {
            mEdgeNormalBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::edgeNormalBufferData(),
                    Base::edgeNumber() * 3 * sizeof(float)),
                BGFX_BUFFER_COMPUTE_FORMAT_32X1 | BGFX_BUFFER_COMPUTE_READ |
                    BGFX_BUFFER_COMPUTE_TYPE_FLOAT);
        }

        // edge color buffer
        if (Base::edgeColorBufferData()) {
            mEdgeColorBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::edgeColorBufferData(),
                    Base::edgeNumber() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32 | BGFX_BUFFER_COMPUTE_READ);
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
        if (bgfx::isValid(mTriangleIndexBH))
            bgfx::destroy(mTriangleIndexBH);

        if (bgfx::isValid(mTriangleNormalBH))
            bgfx::destroy(mTriangleNormalBH);

        if (bgfx::isValid(mTriangleColorBH))
            bgfx::destroy(mTriangleColorBH);

        if (bgfx::isValid(mTriangleTextureIndexBH))
            bgfx::destroy(mTriangleTextureIndexBH);

        if (bgfx::isValid(mEdgeIndexBH))
            bgfx::destroy(mEdgeIndexBH);

        if (bgfx::isValid(mEdgeNormalBH))
            bgfx::destroy(mEdgeNormalBH);

        if (bgfx::isValid(mEdgeColorBH))
            bgfx::destroy(mEdgeColorBH);

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
