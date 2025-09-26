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

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_979_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_979_H

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

#include <algorithm>
#include <vclib/algorithms/core/create.h>
#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/texture_unit.h>
#include <vclib/io/image/load.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/render/selection/selection_box.h>
#include <vclib/render/selection/selection_mode.h>
#include <vclib/space/core/image.h>

#include <bgfx/bgfx.h>

// This allows selection for a maximum of 1024^3 = 1_073_741_824 vertices per
// mesh. Still likely enough. It is set to this because i seem to understand
// that Metal has a cap of 1024 (maybe?)
#define MAX_COMPUTE_WORKGROUP_SIZE uint(1024)

namespace vcl {

template<MeshConcept Mesh>
class MeshRenderBuffers979 : public MeshRenderData<MeshRenderBuffers979<Mesh>>
{
    using MeshType = Mesh;
    using Base     = MeshRenderData<MeshRenderBuffers979<MeshType>>;
    using MRI      = MeshRenderInfo;

    friend Base;

    VertexBuffer mVertexPositionsBuffer;
    VertexBuffer mVertexNormalsBuffer;
    VertexBuffer mVertexColorsBuffer;
    VertexBuffer mVertexUVBuffer;
    VertexBuffer mVertexWedgeUVBuffer;

    // vertex selection
    IndexBuffer mSelectedVerticesBuffer;
    Uniform     mSelectionBoxuniform =
        Uniform("u_selectionBox", bgfx::UniformType::Vec4);
    Uniform mVertexSelectionWorkgroupSizeAndVertexCountUniform =
        Uniform("u_workgroupSizeAndVertexCount", bgfx::UniformType::Vec4);
    std::array<uint, 3> mVertexSelectionWorkgroupSize = {0, 0, 0};

    // face selection: Since we do not know whether this mesh has faces or not,
    // we use an optional
    std::optional<IndexBuffer> mSelectedFacesBuffer        = std::nullopt;
    std::array<uint, 3>        mFaceSelectionWorkgroupSize = {0, 0, 0};

    // point splatting
    IndexBuffer         mVertexQuadIndexBuffer;
    DynamicVertexBuffer mVertexQuadBuffer;
    mutable bool        mVertexQuadBufferGenerated = false;

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
    MeshRenderBuffers979() = default;

    MeshRenderBuffers979(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToFill = MRI::BUFFERS_ALL) :
            Base(buffersToFill)
    {
        Base::update(mesh, buffersToFill);
    }

    MeshRenderBuffers979(const MeshRenderBuffers979& other) = delete;

    MeshRenderBuffers979(MeshRenderBuffers979&& other) { swap(other); }

    MeshRenderBuffers979& operator=(const MeshRenderBuffers979& other) = delete;

    MeshRenderBuffers979& operator=(MeshRenderBuffers979&& other)
    {
        swap(other);
        return *this;
    }

    void swap(MeshRenderBuffers979& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mVertexPositionsBuffer, other.mVertexPositionsBuffer);
        swap(mVertexNormalsBuffer, other.mVertexNormalsBuffer);
        swap(mVertexColorsBuffer, other.mVertexColorsBuffer);
        swap(mVertexUVBuffer, other.mVertexUVBuffer);
        swap(mVertexWedgeUVBuffer, other.mVertexWedgeUVBuffer);
        swap(mVertexQuadIndexBuffer, other.mVertexQuadIndexBuffer);
        swap(mVertexQuadBuffer, other.mVertexQuadBuffer);
        swap(mVertexQuadBufferGenerated, other.mVertexQuadBufferGenerated);
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
        swap(
            mVertexSelectionWorkgroupSize, other.mVertexSelectionWorkgroupSize);
        swap(mSelectedVerticesBuffer, other.mSelectedVerticesBuffer);
        swap(mSelectedFacesBuffer, other.mSelectedFacesBuffer);
    }

    friend void swap(MeshRenderBuffers979& a, MeshRenderBuffers979& b)
    {
        a.swap(b);
    }

    void bindVertexBuffers(const MeshRenderSettings& mrs) const
    {
        // bgfx allows a maximum number of 4 vertex streams...
        mVertexPositionsBuffer.bindVertex(VCL_MRB_VERTEX_POSITION_STREAM);
        mVertexNormalsBuffer.bindVertex(VCL_MRB_VERTEX_NORMAL_STREAM);
        mVertexColorsBuffer.bindVertex(VCL_MRB_VERTEX_COLOR_STREAM);

        if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_VERTEX_TEX)) {
            mVertexUVBuffer.bind(VCL_MRB_VERTEX_TEXCOORD_STREAM);
        }
        else if (mrs.isSurface(MeshRenderInfo::Surface::COLOR_WEDGE_TEX)) {
            mVertexWedgeUVBuffer.bind(VCL_MRB_VERTEX_TEXCOORD_STREAM);
        }
    }

    // to generate splats
    void computeQuadVertexBuffers(
        const MeshType&    mesh,
        const bgfx::ViewId viewId) const
    {
        if (!mVertexQuadBuffer.isValid() || mVertexQuadBufferGenerated) {
            return;
        }

        // fill the buffer using compute shader
        mVertexPositionsBuffer.bindCompute(
            VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
        mVertexNormalsBuffer.bindCompute(
            VCL_MRB_VERTEX_NORMAL_STREAM, bgfx::Access::Read);
        mVertexColorsBuffer.bindCompute(
            VCL_MRB_VERTEX_COLOR_STREAM, bgfx::Access::Read);

        mVertexQuadBuffer.bindCompute(4, bgfx::Access::Write);

        auto& pm = Context::instance().programManager();
        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<ComputeProgram::DRAWABLE_MESH_POINTS>(),
            mesh.vertexNumber(),
            1,
            1);

        mVertexQuadBufferGenerated = true;
    }

    // TODO: submit both vertex selection and face selection of the
    // corresponding type when doing face selection
    void calculateSelection(
        const bgfx::ViewId viewId,
        SelectionBox       box,
        SelectionMode      mode) const
    {
        // If one of the box's point is not set and we are not in one of the
        // atomic modes (which ignore the box entirely) we return
        if (box.anyNull() && !mode.isAtomicMode()) {
            return;
        }

        auto&               pm = Context::instance().programManager();
        bgfx::ProgramHandle selectionProgram =
            getComputeProgramFromSelectionMode(pm, mode);

        // If one of them does not have a value then we are clearly in an atomic
        // mode and the values inside are arbitrary
        SelectionBox minMaxBox = box.toMinAndMax();
        Point2d      minPt     = minMaxBox.get1().value_or(Point2d(0.0, 0.0));
        Point2d      maxPt     = minMaxBox.get2().value_or(Point2d(0.0, 0.0));
        mSelectedVerticesBuffer.bind(4, bgfx::Access::ReadWrite);

        float temp[] = {
            float(minPt.x()),
            float(minPt.y()),
            float(maxPt.x()),
            float(maxPt.y())};
        mSelectionBoxuniform.bind((void*) temp);

        std::array<float, 4> temp2;

        if (mode.isVertexSelection()) {
            mVertexPositionsBuffer.bindCompute(
                VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
            temp2 = {
                vcl::Uniform::uintBitsToFloat(mVertexSelectionWorkgroupSize[0]),
                vcl::Uniform::uintBitsToFloat(mVertexSelectionWorkgroupSize[1]),
                vcl::Uniform::uintBitsToFloat(mVertexSelectionWorkgroupSize[2]),
                vcl::Uniform::uintBitsToFloat(Base::numVerts())};
            mVertexSelectionWorkgroupSizeAndVertexCountUniform.bind(
                (void*) temp2.data());
            bgfx::dispatch(
                viewId,
                selectionProgram,
                mVertexSelectionWorkgroupSize[0],
                mVertexSelectionWorkgroupSize[1],
                mVertexSelectionWorkgroupSize[2]);
        }

        // The face selection program calculates which faces(triangles) are
        // selected. Since it uses the vertex selection buffer to do so, it
        // requires a vertex selection beforehand. The way the vertex selection
        // buffer is interpreted depends exclusively on the Face Selection
        // Program. After calling the face selection program, the vertex
        // selection buffer is zeroed
        if (mode.isFaceSelection()) {
            if (!mSelectedFacesBuffer.has_value()) {
                return;
            }
            calculateSelection(viewId, box, SelectionMode::VERTEX_REGULAR);
            temp2 = {
                vcl::Uniform::uintBitsToFloat(mFaceSelectionWorkgroupSize[0]),
                vcl::Uniform::uintBitsToFloat(mFaceSelectionWorkgroupSize[1]),
                vcl::Uniform::uintBitsToFloat(mFaceSelectionWorkgroupSize[2]),
                vcl::Uniform::uintBitsToFloat(Base::numTris() * 3)};
            mTriangleIndexBuffer.bind(5, bgfx::Access::Read);
            mSelectedVerticesBuffer.bind(4, bgfx::Access::ReadWrite),
                mSelectedFacesBuffer.value().bind(6, bgfx::Access::ReadWrite);
            mVertexSelectionWorkgroupSizeAndVertexCountUniform.bind(
                (void*) temp2.data());
            bgfx::dispatch(
                viewId,
                selectionProgram,
                mFaceSelectionWorkgroupSize[0],
                mFaceSelectionWorkgroupSize[1],
                mFaceSelectionWorkgroupSize[2]);
            calculateSelection(viewId, box, SelectionMode::VERTEX_NONE);
        }
    }

    void bindSelectedVerticesBuffer() const { mSelectedVerticesBuffer.bind(4); }

    // to draw splats
    void bindVertexQuadBuffer() const
    {
        mVertexQuadBuffer.bind(VCL_MRB_VERTEX_POSITION_STREAM);
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
    bgfx::ProgramHandle getComputeProgramFromSelectionMode(
        ProgramManager& pm,
        SelectionMode   mode) const
    {
        switch (SelectionMode::Enum(mode)) {
        case SelectionMode::VERTEX_REGULAR:
            return pm.getComputeProgram<ComputeProgram::SELECTION_VERTEX>();
        case SelectionMode::VERTEX_ADD:
            return pm.getComputeProgram<ComputeProgram::SELECTION_VERTEX_ADD>();
        case SelectionMode::VERTEX_SUBTRACT:
            return pm
                .getComputeProgram<ComputeProgram::SELECTION_VERTEX_SUBTRACT>();
        case SelectionMode::VERTEX_ALL:
            return pm.getComputeProgram<ComputeProgram::SELECTION_VERTEX_ALL>();
        case SelectionMode::VERTEX_NONE:
            return pm
                .getComputeProgram<ComputeProgram::SELECTION_VERTEX_NONE>();
        case SelectionMode::VERTEX_INVERT:
            return pm
                .getComputeProgram<ComputeProgram::SELECTION_VERTEX_INVERT>();
        case SelectionMode::FACE_REGULAR:
            return pm.getComputeProgram<ComputeProgram::SELECTION_FACE>();
        default:
            return pm.getComputeProgram<ComputeProgram::SELECTION_VERTEX>();
        }
    }

    // Possibly replace with an algorithm (maybe a compute shader) that
    // calculates the closest shape to a cube for the three dimensions (to
    // reduce the number of eccess computations), since currently if there are
    // 1025 vertices you use 1024*2*1 = 2048 workgroups.
    void calculateVertexSelectionWorkgroupSize()
    {
        mVertexSelectionWorkgroupSize[0] =
            std::min(Base::numVerts(), MAX_COMPUTE_WORKGROUP_SIZE);
        mVertexSelectionWorkgroupSize[1] = std::min(
            uint(
                std::ceil(
                    double(Base::numVerts()) /
                    double(mVertexSelectionWorkgroupSize[0]))),
            MAX_COMPUTE_WORKGROUP_SIZE);
        mVertexSelectionWorkgroupSize[2] = uint(
            std::ceil(
                double(Base::numVerts()) /
                double(
                    mVertexSelectionWorkgroupSize[0] *
                    mVertexSelectionWorkgroupSize[1])));
    }

    void calculateFaceSelectionWorkgroupSize()
    {
        if (Base::numTris() == 0) {
            mFaceSelectionWorkgroupSize = {0, 0, 0};
            return;
        }
        mFaceSelectionWorkgroupSize[0] =
            std::min(Base::numTris(), MAX_COMPUTE_WORKGROUP_SIZE);
        mFaceSelectionWorkgroupSize[1] = std::min(
            uint(
                std::ceil(
                    double(Base::numTris()) /
                    double(mFaceSelectionWorkgroupSize[0]))),
            MAX_COMPUTE_WORKGROUP_SIZE);
        mFaceSelectionWorkgroupSize[2] = uint(
            std::ceil(
                double(Base::numTris()) / double(
                                              mFaceSelectionWorkgroupSize[0] *
                                              mFaceSelectionWorkgroupSize[1])));
    }

    void setVertexPositionsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 3);

        Base::fillVertexPositions(mesh, buffer);

        mVertexPositionsBuffer.createForCompute(
            buffer,
            nv,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT,
            false,
            bgfx::Access::Read,
            releaseFn);

        // Creates the buffers to be used with compute for splatting
        if (Context::instance().supportsCompute()) {
            // create a layout <coordinates, colors, normals, float>
            // 2 X vec4
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
                .end();

            // create the dynamic vertex buffer for splatting
            mVertexQuadBuffer.create(
                mesh.vertexNumber() * 4, layout, BGFX_BUFFER_COMPUTE_WRITE);

            // create the index buffer for splatting
            setVertexQuadIndexBuffer(mesh);

            // record that the vertex quad buffer must be generated
            mVertexQuadBufferGenerated = false;

            // create the vertex selection buffer
            setVertexSelectionBuffer(mesh);

            // create the face selection buffer
            setFaceSelectionBuffer(mesh);
        }
    }

    /**
     * @brief The function allocates and fills a GPU index buffer to render
     * a quad for each vertex of the mesh.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexQuadIndexBuffer(const MeshType& mesh)
    {
        const uint totalIndices = mesh.vertexNumber() * 6;

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(totalIndices);

        Base::fillVertexQuadIndices(mesh, buffer);

        mVertexQuadIndexBuffer.create(buffer, totalIndices, true, releaseFn);

        // if number of vertices is not zero, the index buffer must be valid
        assert(mVertexQuadIndexBuffer.isValid() || totalIndices == 0);
    }

    /**
     *  @brief The function allocates and fills a GPU index buffer which is a
     * bitmap for vertex selection (i.e. bit is 1 if corresponding vertex is
     * selected, otherwise 0). Initialized to all zeroes.
     *
     *  @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexSelectionBuffer(const MeshType& mesh)
    {
        const uint selectionBufferSize =
            uint(ceil(double(mesh.vertexNumber()) / 32.0));

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(selectionBufferSize);

        for (uint i = 0; i < selectionBufferSize; i++) {
            buffer[i] = 0;
        }

        mSelectedVerticesBuffer.createForCompute(
            buffer,
            selectionBufferSize,
            vcl::PrimitiveType::UINT,
            bgfx::Access::ReadWrite,
            releaseFn);

        calculateVertexSelectionWorkgroupSize();
    }

    void setFaceSelectionBuffer(const MeshType& mesh)
    {
        if (Base::numTris() == 0) {
            mSelectedFacesBuffer        = std::nullopt;
            mFaceSelectionWorkgroupSize = {0, 0, 0};
            return;
        }

        const uint selectionBufferSize =
            uint(ceil(double(Base::numTris()) / 32.0));
        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(selectionBufferSize);
        for (uint i = 0; i < selectionBufferSize; i++) {
            buffer[i] = 0;
        }

        mSelectedFacesBuffer = std::make_optional(IndexBuffer());
        mSelectedFacesBuffer->createForCompute(
            buffer,
            selectionBufferSize,
            vcl::PrimitiveType::UINT,
            bgfx::Access::ReadWrite,
            releaseFn);

        calculateFaceSelectionWorkgroupSize();
    }

    void setVertexNormalsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 3);

        Base::fillVertexNormals(mesh, buffer);

        mVertexNormalsBuffer.createForCompute(
            buffer,
            nv,
            bgfx::Attrib::Normal,
            3,
            PrimitiveType::FLOAT,
            false,
            bgfx::Access::Read,
            releaseFn);
    }

    void setVertexColorsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nv);

        Base::fillVertexColors(mesh, buffer, Color::Format::ABGR);

        mVertexColorsBuffer.createForCompute(
            buffer,
            nv,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            bgfx::Access::ReadWrite,
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
