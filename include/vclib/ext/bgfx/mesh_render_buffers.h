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

#include <vclib/render/drawable/mesh/mesh_render_data.h>

namespace vcl::bgf {

template<MeshConcept MeshType>
class MeshRenderBuffers : public vcl::MeshRenderData<MeshType>
{
    using Base = vcl::MeshRenderData<MeshType>;

    bgfx::VertexBufferHandle mVertexCoordBH  = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle mVertexNormalBH = BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle mVertexColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mTriangleIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mTriangleNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mTriangleColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mEdgeIndexBH  = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mEdgeNormalBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle mEdgeColorBH  = BGFX_INVALID_HANDLE;

    bgfx::IndexBufferHandle mWireframeIndexBH = BGFX_INVALID_HANDLE;

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
        std::swap(mVertexCoordBH, other.mVertexCoordBH);
        std::swap(mVertexNormalBH, other.mVertexNormalBH);
        std::swap(mVertexColorBH, other.mVertexColorBH);
        std::swap(mTriangleIndexBH, other.mTriangleIndexBH);
        std::swap(mTriangleNormalBH, other.mTriangleNormalBH);
        std::swap(mTriangleColorBH, other.mTriangleColorBH);
        std::swap(mEdgeIndexBH, other.mEdgeIndexBH);
        std::swap(mEdgeNormalBH, other.mEdgeNormalBH);
        std::swap(mEdgeColorBH, other.mEdgeColorBH);
        std::swap(mWireframeIndexBH, other.mWireframeIndexBH);
    }

    void update(const MeshType& mesh)
    {
        Base::update(mesh);
        destroyBGFXBuffers();
        createBGFXBuffers();
    }

    void bindVertexBuffers()
    {
        bgfx::setVertexBuffer(0, mVertexCoordBH);

        if (bgfx::isValid(mVertexNormalBH)) { // vertex normals
            bgfx::setVertexBuffer(1, mVertexNormalBH);
        }

        if (bgfx::isValid(mVertexColorBH)) { // vertex colors
            bgfx::setVertexBuffer(2, mVertexColorBH);
        }
    }

    void bindIndexBuffers(uint indexBufferToBind = Base::TRIANGLES)
    {
        if (indexBufferToBind == Base::TRIANGLES) {
            bgfx::setIndexBuffer(mTriangleIndexBH);

            if (bgfx::isValid(mTriangleColorBH)) { // triangle colors
                bgfx::setBuffer(1, mTriangleColorBH, bgfx::Access::Read);
            }

            if (bgfx::isValid(mTriangleNormalBH)) { // triangle normals
                bgfx::setBuffer(2, mTriangleNormalBH, bgfx::Access::Read);
            }
        }
        else if (indexBufferToBind == Base::EDGES) {
            bgfx::setIndexBuffer(mEdgeIndexBH);

            if (bgfx::isValid(mEdgeColorBH)) { // edge colors
                bgfx::setBuffer(1, mEdgeColorBH, bgfx::Access::Read);
            }

            if (bgfx::isValid(mEdgeNormalBH)) { // edge normals
                bgfx::setBuffer(2, mEdgeNormalBH, bgfx::Access::Read);
            }
        }
        else if (indexBufferToBind == Base::WIREFRAME) {
            bgfx::setIndexBuffer(mWireframeIndexBH);
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
    }

    void destroyBGFXBuffers()
    {
        if (bgfx::isValid(mVertexCoordBH))
            bgfx::destroy(mVertexCoordBH);

        if (bgfx::isValid(mVertexNormalBH))
            bgfx::destroy(mVertexNormalBH);

        if (bgfx::isValid(mVertexColorBH))
            bgfx::destroy(mVertexColorBH);

        if (bgfx::isValid(mTriangleIndexBH))
            bgfx::destroy(mTriangleIndexBH);

        if (bgfx::isValid(mTriangleNormalBH))
            bgfx::destroy(mTriangleNormalBH);

        if (bgfx::isValid(mTriangleColorBH))
            bgfx::destroy(mTriangleColorBH);

        if (bgfx::isValid(mEdgeIndexBH))
            bgfx::destroy(mEdgeIndexBH);

        if (bgfx::isValid(mEdgeNormalBH))
            bgfx::destroy(mEdgeNormalBH);

        if (bgfx::isValid(mEdgeColorBH))
            bgfx::destroy(mEdgeColorBH);

        if (bgfx::isValid(mWireframeIndexBH))
            bgfx::destroy(mWireframeIndexBH);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_MESH_RENDER_BUFFERS_H
