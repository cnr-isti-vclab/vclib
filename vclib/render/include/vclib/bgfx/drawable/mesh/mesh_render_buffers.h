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

#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/drawable/lines/lines/gpu_generated_lines.h>


#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers : public vcl::MeshRenderData<MeshType>
{
    using Base = vcl::MeshRenderData<MeshType>;

    bgfx::VertexBufferHandle mVertexCoordBH   = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle mVertexNormalBH  = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle mVertexColorBH   = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle mVertexUVBH      = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle mVertexWedgeUVBH = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mTriangleIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mTriangleNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mTriangleColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mTriangleTextureIndexBH = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mEdgeIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mEdgeNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mEdgeColorBH  = BGFX_INVALID_HANDLE;

    lines::GPUGeneratedLines mWireframeBH;

    std::vector<std::pair<bgfx::TextureHandle, bgfx::UniformHandle>> mTexturesH;

public:
    MeshRenderBuffers() = default;

    MeshRenderBuffers(const MeshType& mesh, uint buffersToFill = Base::ALL) : Base(mesh, buffersToFill)
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
        std::swap((Base&) *this, (Base&) other);
        std::swap(mVertexCoordBH, other.mVertexCoordBH);
        std::swap(mVertexNormalBH, other.mVertexNormalBH);
        std::swap(mVertexColorBH, other.mVertexColorBH);
        std::swap(mVertexUVBH, other.mVertexUVBH);
        std::swap(mVertexWedgeUVBH, other.mVertexWedgeUVBH);
        std::swap(mTriangleIndexBH, other.mTriangleIndexBH);
        std::swap(mTriangleNormalBH, other.mTriangleNormalBH);
        std::swap(mTriangleColorBH, other.mTriangleColorBH);
        std::swap(mTriangleTextureIndexBH, other.mTriangleTextureIndexBH);
        std::swap(mEdgeIndexBH, other.mEdgeIndexBH);
        std::swap(mEdgeNormalBH, other.mEdgeNormalBH);
        std::swap(mEdgeColorBH, other.mEdgeColorBH);
        std::swap(mTexturesH, other.mTexturesH);

        mWireframeBH.swap(other.mWireframeBH);
    }

    void update(const MeshType& mesh)
    {
        Base::update(mesh);
        destroyBGFXBuffers();
        createBGFXBuffers();
    }

    void bindVertexBuffers(const MeshRenderSettings& mrs) const
    {
        // bgfx allows a maximum number of 4 vertex streams...

        bgfx::setVertexBuffer(0, mVertexCoordBH);

        if (bgfx::isValid(mVertexNormalBH)) { // vertex normals
            bgfx::setVertexBuffer(1, mVertexNormalBH);
        }

        if (bgfx::isValid(mVertexColorBH)) { // vertex colors
            bgfx::setVertexBuffer(2, mVertexColorBH);
        }

        if (mrs.isSurfaceColorPerVertexTexcoords()) {
            if (bgfx::isValid(mVertexUVBH)) { // vertex UVs
                bgfx::setVertexBuffer(3, mVertexUVBH);
            }
        }
        else if (mrs.isSurfaceColorPerWedgeTexcoords()) {
            if (bgfx::isValid(mVertexWedgeUVBH)) { // vertex wedge UVs
                bgfx::setVertexBuffer(3, mVertexWedgeUVBH);
            }
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
    }

    void drawWireframe(uint viewId) const 
    {
        mWireframeBH.draw(viewId);
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
        // vertex buffer (positions)
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        mVertexCoordBH = bgfx::createVertexBuffer(
            bgfx::makeRef(
                Base::vertexBufferData(),
                Base::vertexBufferSize() * sizeof(float)),
            layout);

        // vertex buffer (normals)
        if (Base::vertexNormalBufferData()) {
            bgfx::VertexLayout vnlayout;
            vnlayout.begin()
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .end();

            mVertexNormalBH = bgfx::createVertexBuffer(
                bgfx::makeRef(
                    Base::vertexNormalBufferData(),
                    Base::vertexBufferSize() * sizeof(float)),
                vnlayout);
        }

        // vertex buffer (colors)
        if (Base::vertexColorBufferData()) {
            bgfx::VertexLayout vclayout;
            vclayout.begin()
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

            mVertexColorBH = bgfx::createVertexBuffer(
                bgfx::makeRef(
                    Base::vertexColorBufferData(),
                    Base::vertexNumber() * sizeof(uint32_t)),
                vclayout);
        }

        // vertex buffer (UVs)
        if (Base::vertexTexCoordsBufferData()) {
            bgfx::VertexLayout uvlayout;
            uvlayout.begin()
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();

            mVertexUVBH = bgfx::createVertexBuffer(
                bgfx::makeRef(
                    Base::vertexTexCoordsBufferData(),
                    Base::vertexNumber() * 2 * sizeof(float)),
                uvlayout);
        }

        // vertex wedges buffer (duplicated vertices)
        if (Base::wedgeTexCoordsBufferData()) {
            bgfx::VertexLayout uvlayout;
            uvlayout.begin()
                .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
                .end();

            mVertexWedgeUVBH = bgfx::createVertexBuffer(
                bgfx::makeRef(
                    Base::wedgeTexCoordsBufferData(),
                    Base::vertexNumber() * 2 * sizeof(float)),
                uvlayout);
        }

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
            mWireframeBH = lines::GPUGeneratedLines(*Base::wireframeBufferData());
            mWireframeBH.getSettings().setThickness(2);
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
        if (bgfx::isValid(mVertexCoordBH))
            bgfx::destroy(mVertexCoordBH);

        if (bgfx::isValid(mVertexNormalBH))
            bgfx::destroy(mVertexNormalBH);

        if (bgfx::isValid(mVertexColorBH))
            bgfx::destroy(mVertexColorBH);

        if (bgfx::isValid(mVertexUVBH))
            bgfx::destroy(mVertexUVBH);

        if (bgfx::isValid(mVertexWedgeUVBH))
            bgfx::destroy(mVertexWedgeUVBH);

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

        for (auto [th, uh] : mTexturesH) {
            bgfx::destroy(th);
            bgfx::destroy(uh);
        }
        mTexturesH.clear();
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
