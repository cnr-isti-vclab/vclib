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
#include <vclib/bgfx/drawable/drawable_lines.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/texture_unit.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
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

    IndexBuffer mTriangleIndexBuffer;
    IndexBuffer mTriangleNormalBuffer;
    IndexBuffer mTriangleColorBuffer;

    IndexBuffer mVertexTextureIndexBuffer;
    IndexBuffer mWedgeTextureIndexBuffer;

    // TODO: manage wireframe with proper lines
    IndexBuffer mEdgeIndexBuffer;
    IndexBuffer mEdgeNormalBuffer;
    IndexBuffer mEdgeColorBuffer;

    GPUGeneratedLines mWireframeLines;

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

        mWireframeLines.swap(other.mWireframeLines);
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

        // WAS:
        // const uint nw = Base::numWireframeLines();

        // auto [buffer, releaseFn] =
        //    getAllocatedBufferAndReleaseFn<uint>(nw * 2);

        // Base::fillWireframeIndices(mesh, buffer);

        // mWireframeIndexBuffer.create(buffer, nw * 2, true, releaseFn);
    }

    void setTextureUnits(const MeshType& mesh) // override
    {
        mTextureUnits.clear();
        mTextureUnits.reserve(mesh.textureNumber());
        for (uint i = 0; i < mesh.textureNumber(); ++i) {
            vcl::Image txt;
            if constexpr (vcl::HasTextureImages<MeshType>) {
                if (mesh.texture(i).image().isNull()) {
                    txt = vcl::Image(mesh.meshBasePath() + mesh.texturePath(i));
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
