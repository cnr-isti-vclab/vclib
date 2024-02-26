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

#ifndef VCL_EXT_BGFX_MESH_RENDER_BUFFERS_H
#define VCL_EXT_BGFX_MESH_RENDER_BUFFERS_H

#include <bgfx/bgfx.h>

#include <vclib/render/mesh_render_buffers.h>

namespace vcl::bgf {

template<MeshConcept MeshType>
class MeshRenderBuffers : public vcl::MeshRenderBuffers<MeshType>
{
    using Base = vcl::MeshRenderBuffers<MeshType>;

    bgfx::VertexBufferHandle vertexCoordBH  = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexNormalBH = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle vertexColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle triangleIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle triangleNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle triangleColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle wireframeIndexBH = BGFX_INVALID_HANDLE;

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
        std::swap((Base&) *this, (Base&) other);
        std::swap(vertexCoordBH, other.vertexCoordBH);
        std::swap(vertexNormalBH, other.vertexNormalBH);
        std::swap(vertexColorBH, other.vertexColorBH);
        std::swap(triangleIndexBH, other.triangleIndexBH);
        std::swap(triangleNormalBH, other.triangleNormalBH);
        std::swap(triangleColorBH, other.triangleColorBH);
        std::swap(wireframeIndexBH, other.wireframeIndexBH);
    }

    void update(const MeshType& mesh)
    {
        Base::update(mesh);
        destroyBGFXBuffers();
        createBGFXBuffers();
    }

    void bindVertexBuffers()
    {
        bgfx::setVertexBuffer(0, vertexCoordBH);

        if (bgfx::isValid(vertexNormalBH)) { // vertex normals
            bgfx::setVertexBuffer(1, vertexNormalBH);
        }

        if (bgfx::isValid(vertexColorBH)) { // vertex colors
            bgfx::setVertexBuffer(2, vertexColorBH);
        }
    }

    void bindIndexBuffers(bool triangles = true)
    {
        if (triangles) {
            bgfx::setIndexBuffer(triangleIndexBH);

            if (bgfx::isValid(triangleColorBH)) { // triangle colors
                bgfx::setBuffer(1, triangleColorBH, bgfx::Access::Read);
            }

            if (bgfx::isValid(triangleNormalBH)) { // triangle normals
                bgfx::setBuffer(2, triangleNormalBH, bgfx::Access::Read);
            }
        }
        else {
            bgfx::setIndexBuffer(wireframeIndexBH);
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

        vertexCoordBH = bgfx::createVertexBuffer(
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

            vertexNormalBH = bgfx::createVertexBuffer(
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

            vertexColorBH = bgfx::createVertexBuffer(
                bgfx::makeRef(
                    Base::vertexColorBufferData(),
                    Base::vertexNumber() * sizeof(uint32_t)),
                vclayout);
        }

        // triangle index buffer
        if (Base::triangleBufferData()) {
            triangleIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::triangleBufferData(),
                    Base::triangleBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }

        // triangle normal buffer
        if (Base::triangleNormalBufferData()) {
            triangleNormalBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::triangleNormalBufferData(),
                    Base::triangleNumber() * 3 * sizeof(float)),
                BGFX_BUFFER_COMPUTE_FORMAT_32X1 | BGFX_BUFFER_COMPUTE_READ |
                    BGFX_BUFFER_COMPUTE_TYPE_FLOAT);
        }

        // triangle color buffer
        if (Base::triangleColorBufferData()) {
            triangleColorBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::triangleColorBufferData(),
                    Base::triangleNumber() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32 | BGFX_BUFFER_COMPUTE_READ);
        }

        // edge index buffer
        if (Base::wireframeBufferData()) {
            wireframeIndexBH = bgfx::createIndexBuffer(
                bgfx::makeRef(
                    Base::wireframeBufferData(),
                    Base::wireframeBufferSize() * sizeof(uint32_t)),
                BGFX_BUFFER_INDEX32);
        }
    }

    void destroyBGFXBuffers()
    {
        if (bgfx::isValid(vertexCoordBH))
            bgfx::destroy(vertexCoordBH);

        if (bgfx::isValid(vertexNormalBH))
            bgfx::destroy(vertexNormalBH);

        if (bgfx::isValid(vertexColorBH))
            bgfx::destroy(vertexColorBH);

        if (bgfx::isValid(triangleIndexBH))
            bgfx::destroy(triangleIndexBH);

        if (bgfx::isValid(triangleNormalBH))
            bgfx::destroy(triangleNormalBH);

        if (bgfx::isValid(triangleColorBH))
            bgfx::destroy(triangleColorBH);

        if (bgfx::isValid(wireframeIndexBH))
            bgfx::destroy(wireframeIndexBH);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_MESH_RENDER_BUFFERS_H
