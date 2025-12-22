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

#include "mesh_render_buffers_macros.h"

#include <algorithm>
#include <vclib/algorithms/core/create.h>
#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/index_buffer_to_cpu_handler.h>
#include <vclib/bgfx/texture.h>
#include <vclib/bgfx/drawable/uniforms/material_uniforms.h>
#include <vclib/bgfx/primitives/lines.h>
#include <vclib/io/image/load.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/render/selection/selection_box.h>
#include <vclib/render/selection/selection_mode.h>
#include <vclib/render/selection/selection_parameters.h>
#include <vclib/space/core/image.h>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

// This allows selection for a maximum of 1024^3 = 134_217_728 vertices/faces per
// mesh. Still likely enough.
#define MAX_COMPUTE_WORKGROUP_SIZE uint(512)

namespace vcl {

template<MeshConcept Mesh>
class MeshRenderBuffers979 : public MeshRenderData<MeshRenderBuffers979<Mesh>>
{
    using MeshType = Mesh;
    using Base     = MeshRenderData<MeshRenderBuffers979<MeshType>>;
    using MRI      = MeshRenderInfo;

    friend Base;

    inline static const uint N_TEXTURE_TYPES =
        toUnderlying(Material::TextureType::COUNT);

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
    Uniform mVisibleFacesComputeUniform = Uniform("u_meshIdAndDispatchSizeXY", bgfx::UniformType::Vec4);
    Uniform mVisibleFacesVertFragUniform = Uniform("u_meshId", bgfx::UniformType::Vec4);
    std::optional<IndexBuffer> mSelectedFacesBuffer        = std::nullopt;
    std::array<uint, 3>        mFaceSelectionWorkgroupSize = {0, 0, 0};

    // Handler used to copy selection buffers to CPU
    IndexBufferToCpuHandler mSelectionToCPUBufferHandler;
    VertexBuffer mVertexTangentsBuffer;

    // point splatting
    IndexBuffer         mVertexQuadIndexBuffer;
    DynamicVertexBuffer mVertexQuadBuffer;
    mutable bool        mVertexQuadBufferGenerated = false;

    IndexBuffer mTriangleIndexBuffer;
    IndexBuffer mTriangleNormalBuffer;
    IndexBuffer mTriangleColorBuffer;

    Lines mEdgeLines;

    Lines mWireframeLines;
    Color mMeshColor; // todo: find better way to store mesh color

    // map of textures
    // for each texture path of each material, store its texture
    std::map<std::string, Texture> mMaterialTextures;

    mutable DrawableMeshUniforms         mMeshUniforms;
    mutable MaterialUniforms             mMaterialUniforms;
    std::array<Uniform, N_TEXTURE_TYPES> mTextureSamplerUniforms;

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
        swap(mVertexTangentsBuffer, other.mVertexTangentsBuffer);
        swap(mVertexQuadIndexBuffer, other.mVertexQuadIndexBuffer);
        swap(mVertexQuadBuffer, other.mVertexQuadBuffer);
        swap(mVertexQuadBufferGenerated, other.mVertexQuadBufferGenerated);
        swap(mTriangleIndexBuffer, other.mTriangleIndexBuffer);
        swap(mTriangleNormalBuffer, other.mTriangleNormalBuffer);
        swap(mTriangleColorBuffer, other.mTriangleColorBuffer);
        swap(mEdgeLines, other.mEdgeLines);
        swap(mWireframeLines, other.mWireframeLines);
        swap(mMeshUniforms, other.mMeshUniforms);
        swap(
            mVertexSelectionWorkgroupSize, other.mVertexSelectionWorkgroupSize);
        swap(mSelectedVerticesBuffer, other.mSelectedVerticesBuffer);
        swap(mFaceSelectionWorkgroupSize, other.mFaceSelectionWorkgroupSize);
        swap(mSelectedFacesBuffer, other.mSelectedFacesBuffer);
        swap(mSelectionToCPUBufferHandler, other.mSelectionToCPUBufferHandler);
        swap(mMaterialTextures, other.mMaterialTextures);
        swap(mMaterialUniforms, other.mMaterialUniforms);
        swap(mTextureSamplerUniforms, other.mTextureSamplerUniforms);
    }

    friend void swap(MeshRenderBuffers979& a, MeshRenderBuffers979& b) { a.swap(b); }

    uint triangleChunksNumber() const { return Base::mMaterialChunks.size(); }

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

    bool vertexSelection(const SelectionParameters& params)
    {
        if (params.box.anyNull()) {
            return false;
        }
        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(Context::instance().programManager(), params.mode);
        bindSelectionBox(params.box);
        bindVertexWGroupSizeAndCount();
        mSelectedVerticesBuffer.bind(4, bgfx::Access::ReadWrite);
        mVertexPositionsBuffer.bindCompute(VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
        dispatchVertexSelection(params.drawViewId, prog);
        return true;
    }

    bool vertexSelectionAtomic(const SelectionParameters& params)
    {
        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(Context::instance().programManager(), params.mode);
        bindVertexWGroupSizeAndCount();
        mSelectedVerticesBuffer.bind(4, bgfx::Access::ReadWrite);
        dispatchVertexSelection(params.drawViewId, prog);
        return true;
    }

    bool faceSelection(const SelectionParameters& params)
    {
        if (params.box.anyNull()) {
            return false;
        }
        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(Context::instance().programManager(), params.mode);
        bindSelectionBox(params.box);
        bindFaceWGroupSizeAndCount();
        mSelectedFacesBuffer.value().bind(6, bgfx::Access::ReadWrite);
        mVertexPositionsBuffer.bindCompute(VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
        mTriangleIndexBuffer.bind(5, bgfx::Access::Read);
        dispatchFaceSelection(params.drawViewId, prog);
        return true;
    }

    bool faceSelectionAtomic(const SelectionParameters& params)
    {
        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(Context::instance().programManager(), params.mode);
        bindFaceWGroupSizeAndCount();
        mSelectedFacesBuffer.value().bind(4, bgfx::Access::ReadWrite);
        dispatchFaceSelection(params.drawViewId, prog);
        return true;
    }

    bool faceSelectionVisible(const SelectionParameters& params, const Matrix44f& model)
    {
        if(params.mode == SelectionMode::FACE_VISIBLE_REGULAR) {
            SelectionParameters params2(params);
            params2.mode = SelectionMode::FACE_NONE;
            faceSelectionAtomic(params2);
        }
        ProgramManager& pm = Context::instance().programManager();
        bgfx::ProgramHandle passProgram = pm.getProgram<VertFragProgram::SELECTION_FACE_VISIBLE_RENDER_PASS>();
        bgfx::ProgramHandle computeProg = getComputeProgramFromSelectionMode(pm, params.mode);
        std::array<uint, 3> workGroupSize = workGroupSizesFrom1DSize(params.colorAttachmentSize[0] * params.colorAttachmentSize[1]);
        float temp[4] = {
            Uniform::uintBitsToFloat(params.meshId),
            Uniform::uintBitsToFloat(workGroupSize[0]),
            Uniform::uintBitsToFloat(workGroupSize[1]),
            0.f
        };
        mVisibleFacesVertFragUniform.bind(temp);
        mVertexPositionsBuffer.bindVertex(VCL_MRB_VERTEX_POSITION_STREAM);
        mTriangleIndexBuffer.bind();
        bgfx::submit(params.pass1ViewId, passProgram);
        mVisibleFacesComputeUniform.bind(temp);
        bgfx::setImage(0, params.colorAttachmentTex, 0, bgfx::Access::Read);
        mSelectedFacesBuffer.value().bind(6, bgfx::Access::ReadWrite);
        bgfx::dispatch(params.pass2ViewId, computeProg, workGroupSize[0], workGroupSize[1], workGroupSize[2]);
        return true;
    }

    uint requestCPUCopyOfSelectionBuffer(const SelectionMode& mode)
    {
        uint elementBitSize = 1;
        uint elementCount;
        IndexBuffer *idxBuf;
        if (mode.isFaceSelection()) {
            idxBuf = &(mSelectedFacesBuffer.value());
            elementCount = Base::numTris();
        }
        if (mode.isVertexSelection()) {
            idxBuf = &mSelectedVerticesBuffer;
            elementCount =  Base::numVerts();
        }
        mSelectionToCPUBufferHandler.copyFromGPU(*idxBuf, elementCount, elementBitSize); 
        return 2;
    }

    std::vector<uint8_t> getSelectionBufferCopy() const {
        return std::move(mSelectionToCPUBufferHandler.getResultsCopy());
    }

    void bindSelectedVerticesBuffer() const { mSelectedVerticesBuffer.bind(4); }

    void bindSelectedFacesBuffer() const
    {
        mSelectedFacesBuffer.value().bind(6);
    }
    
    void bindVertexBuffers(const MeshRenderSettings& mrs) const
    {
        uint stream = 0;

        // streams MUST be consecutive starting from 0
        mVertexPositionsBuffer.bindVertex(stream++);

        if (mVertexNormalsBuffer.isValid()) {
            mVertexNormalsBuffer.bindVertex(stream++);
        }

        if (mVertexColorsBuffer.isValid()) {
            mVertexColorsBuffer.bindVertex(stream++);
        }

        if (mVertexUVBuffer.isValid()) {
            mVertexUVBuffer.bind(stream++);
        }

        if (mVertexWedgeUVBuffer.isValid()) {
            mVertexWedgeUVBuffer.bind(stream++);
        }

        if (mVertexTangentsBuffer.isValid()) {
            mVertexTangentsBuffer.bind(stream++);
        }
    }

    // to draw splats
    void bindVertexQuadBuffer() const
    {
        mVertexQuadBuffer.bind(VCL_MRB_VERTEX_POSITION_STREAM);
        mVertexQuadIndexBuffer.bind();
    }

    void bindIndexBuffers(
        const MeshRenderSettings& mrs,
        uint                      chunkToBind = UINT_NULL) const
    {
        using enum MRI::Buffers;

        if (chunkToBind == UINT_NULL) {
            mTriangleIndexBuffer.bind();
            mMeshUniforms.updateFirstChunkIndex(0);
        }
        else {
            const auto& chunk = Base::mMaterialChunks[chunkToBind];
            mMeshUniforms.updateFirstChunkIndex(chunk.startIndex);
            mTriangleIndexBuffer.bind(
                chunk.startIndex * 3, chunk.indexCount * 3);
        }

        mTriangleNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

        mTriangleColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);
    }

    void drawEdgeLines(uint viewId) const { mEdgeLines.draw(viewId); }

    void drawWireframeLines(uint viewId) const { mWireframeLines.draw(viewId); }

    void bindTextures(
        const MeshRenderSettings& mrs,
        uint                      chunkNumber,
        const MeshType&           m) const
    {
        uint materialId = Base::materialIndex(mrs, chunkNumber);

        if (materialId != UINT_NULL) {
            for (uint j = 0; j < N_TEXTURE_TYPES; ++j) {
                const auto& td = m.material(materialId).textureDescriptor(j);
                const std::string& path = td.path();
                if (!path.empty()) {
                    const Texture& tex = mMaterialTextures.at(path);
                    if (tex.isValid()) {
                        uint flags = Texture::samplerFlagsFromTexture(td);
                        tex.bind(
                            VCL_MRB_TEXTURE0 + j,
                            mTextureSamplerUniforms[j].handle(),
                            flags);
                    }
                }
            }
        }
    }

    /**
     * @brief Sets and binds the material uniforms for the given triangle chunk,
     * and returns the render state associated to the material that must be set
     * for the draw call.
     *
     * @param mrs
     * @param chunkNumber
     * @param m
     * @return the render state associated to the material
     */
    uint64_t bindMaterials(
        const MeshRenderSettings& mrs,
        uint                      chunkNumber,
        const MeshType&           m) const
    {
        static const Material DEFAULT_MATERIAL;

        uint64_t state = BGFX_STATE_NONE;

        std::array<bool, N_TEXTURE_TYPES> textureAvailable = {false};

        if constexpr (!HasMaterials<MeshType>) {
            // fallback to default material
            mMaterialUniforms.update(
                DEFAULT_MATERIAL,
                isPerVertexColorAvailable(m),
                textureAvailable,
                isPerVertexTangentAvailable(m));
        }
        else {
            using enum Material::AlphaMode;

            uint materialId = Base::materialIndex(mrs, chunkNumber);

            if (materialId == UINT_NULL) {
                // fallback to default material
                mMaterialUniforms.update(
                    DEFAULT_MATERIAL,
                    isPerVertexColorAvailable(m),
                    textureAvailable,
                    isPerVertexTangentAvailable(m));
            }
            else {
                assert(materialId < m.materialsNumber());
                const Material& mat = m.material(materialId);

                for (uint j = 0; j < N_TEXTURE_TYPES; ++j) {
                    const auto& td =
                        m.material(materialId).textureDescriptor(j);
                    const std::string& path = td.path();
                    if (!path.empty()) {
                        const Texture& tex  = mMaterialTextures.at(path);
                        textureAvailable[j] = tex.isValid();
                    }
                }

                mMaterialUniforms.update(
                    m.material(materialId),
                    isPerVertexColorAvailable(m),
                    textureAvailable,
                    isPerVertexTangentAvailable(m));

                // set the state according to the material
                if (!m.material(materialId).doubleSided()) {
                    // backface culling
                    state |= BGFX_STATE_CULL_CW;
                }
                if (m.material(materialId).alphaMode() == ALPHA_BLEND) {
                    state |= BGFX_STATE_BLEND_ALPHA;
                }
            }
        }

        mMaterialUniforms.bind();
        return state;
    }

    void updateEdgeSettings(const MeshRenderSettings& mrs)
    {
        using enum MeshRenderInfo::Edges;
        using enum Lines::ColorToUse;

        mEdgeLines.thickness() = mrs.edgesWidth();
        mEdgeLines.setShading(mrs.isEdges(SHADING_SMOOTH));

        if (mrs.isEdges(COLOR_USER)) {
            mEdgeLines.generalColor() = mrs.edgesUserColor();
            mEdgeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isEdges(COLOR_MESH)) {
            mEdgeLines.generalColor() = mMeshColor;
            mEdgeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isEdges(COLOR_VERTEX)) {
            mEdgeLines.setColorToUse(PER_VERTEX);
        }
        else if (mrs.isEdges(COLOR_EDGE)) {
            mEdgeLines.setColorToUse(PER_EDGE);
        }
    }

    void updateWireframeSettings(const MeshRenderSettings& mrs)
    {
        using enum MeshRenderInfo::Wireframe;
        using enum Lines::ColorToUse;

        mWireframeLines.thickness() = mrs.wireframeWidth();
        mWireframeLines.setShading(mrs.isWireframe(SHADING_VERT));

        if (mrs.isWireframe(COLOR_USER)) {
            mWireframeLines.generalColor() = mrs.wireframeUserColor();
            mWireframeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isWireframe(COLOR_MESH)) {
            mWireframeLines.generalColor() = mMeshColor;
            mWireframeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isWireframe(COLOR_VERTEX)) {
            mWireframeLines.setColorToUse(PER_VERTEX);
        }
    }

    void bindUniforms() const { mMeshUniforms.bind(); }

private:
    void bindSelectionBox(const SelectionBox& box)
    {
        Point2d minPt = box.get1().value();
        Point2d maxPt = box.get2().value();
        float temp[] = {
            float(minPt.x()),
            float(minPt.y()),
            float(maxPt.x()),
            float(maxPt.y())};
        mSelectionBoxuniform.bind((void*) temp);
    }

    void bindVertexWGroupSizeAndCount()
    {
        std::array<float, 4> temp = {
            vcl::Uniform::uintBitsToFloat(mVertexSelectionWorkgroupSize[0]),
            vcl::Uniform::uintBitsToFloat(mVertexSelectionWorkgroupSize[1]),
            vcl::Uniform::uintBitsToFloat(mVertexSelectionWorkgroupSize[2]),
            vcl::Uniform::uintBitsToFloat(Base::numVerts())
        };
        mVertexSelectionWorkgroupSizeAndVertexCountUniform.bind(
            (void*) temp.data());
    }

    void bindFaceWGroupSizeAndCount()
    {
        std::array<float, 4> temp = {
            vcl::Uniform::uintBitsToFloat(mFaceSelectionWorkgroupSize[0]),
            vcl::Uniform::uintBitsToFloat(mFaceSelectionWorkgroupSize[1]),
            vcl::Uniform::uintBitsToFloat(mFaceSelectionWorkgroupSize[2]),
            vcl::Uniform::uintBitsToFloat(Base::numTris())
        };
        mVertexSelectionWorkgroupSizeAndVertexCountUniform.bind(
            (void*) temp.data());
    }

    void dispatchVertexSelection(const uint viewId, const bgfx::ProgramHandle& handle)
    {
        bgfx::dispatch(
            viewId,
            handle,
            mVertexSelectionWorkgroupSize[0],
            mVertexSelectionWorkgroupSize[1],
            mVertexSelectionWorkgroupSize[2]
        );
    }

    void dispatchFaceSelection(const uint viewId, const bgfx::ProgramHandle& handle)
    {
        bgfx::dispatch(
            viewId,
            handle,
            mFaceSelectionWorkgroupSize[0],
            mFaceSelectionWorkgroupSize[1],
            mFaceSelectionWorkgroupSize[2]
        );
    }

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
        case SelectionMode::FACE_REGULAR:
            return pm.getComputeProgram<ComputeProgram::SELECTION_FACE>();
        case SelectionMode::FACE_ADD:
            return pm.getComputeProgram<ComputeProgram::SELECTION_FACE_ADD>();
        case SelectionMode::FACE_SUBTRACT:
            return pm.getComputeProgram<ComputeProgram::SELECTION_FACE_SUBTRACT>();
        case SelectionMode::VERTEX_ALL:
        case SelectionMode::FACE_ALL:
            return pm.getComputeProgram<ComputeProgram::SELECTION_ALL>();
        case SelectionMode::VERTEX_NONE:
        case SelectionMode::FACE_NONE:
            return pm.getComputeProgram<ComputeProgram::SELECTION_NONE>();
        case SelectionMode::VERTEX_INVERT:
        case SelectionMode::FACE_INVERT:
            return pm.getComputeProgram<ComputeProgram::SELECTION_INVERT>();
        case SelectionMode::FACE_VISIBLE_REGULAR:
        case SelectionMode::FACE_VISIBLE_ADD:
            return pm.getComputeProgram<ComputeProgram::SELECTION_FACE_VISIBLE_ADD>();
        case SelectionMode::FACE_VISIBLE_SUBTRACT:
            return pm.getComputeProgram<ComputeProgram::SELECTION_FACE_VISIBLE_SUBTRACT>();
        default:
            return pm.getComputeProgram<ComputeProgram::SELECTION_VERTEX>();
        }
    }

    static std::array<uint, 3> workGroupSizesFrom1DSize(uint size) {
        std::array<uint, 3> sizes;
        sizes[0] =
            std::min(size, MAX_COMPUTE_WORKGROUP_SIZE);
        sizes[1] = std::min(
        uint(
            std::ceil(
                double(size) /
                double(sizes[0]))),
        MAX_COMPUTE_WORKGROUP_SIZE);
        sizes[2] = uint(
            std::ceil(
                double(size) /
                double(
                    sizes[0] *
                    sizes[1])));
        return sizes;
    }

    // Possibly replace with an algorithm (maybe a compute shader) that
    // calculates the closest shape to a cube for the three dimensions (to
    // reduce the number of eccess computations), since currently if there are
    // 1025 vertices you use 1024*2*1 = 2048 workgroups.
    void calculateVertexSelectionWorkgroupSize()
    {
        mVertexSelectionWorkgroupSize = workGroupSizesFrom1DSize(Base::numVerts());
    }

    void calculateFaceSelectionWorkgroupSize()
    {
        if (Base::numTris() == 0) {
            mFaceSelectionWorkgroupSize = {0, 0, 0};
            return;
        }
        mFaceSelectionWorkgroupSize = workGroupSizesFrom1DSize(Base::numTris());
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

        mSelectionToCPUBufferHandler =
            std::move(IndexBufferToCpuHandler(uint(ceil(
                max(double(Base::numVerts()), double(Base::numTris())) /
                8.0))));
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
            bgfx::Access::Read,
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

    void setVertexTangentsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 4);

        Base::fillVertexTangents(mesh, buffer);

        mVertexTangentsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Tangent,
            4,
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

        // Triangle index buffer required in the face selection compute
        mTriangleIndexBuffer.createForCompute(
            buffer,
            nt * 3,
            vcl::PrimitiveType::UINT,
            bgfx::Access::Read,
            releaseFn);
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

    void setEdgeIndicesBuffer(const MeshType& mesh) // override
    {
        computeEdgeLines(mesh);
    }

    void setWireframeIndicesBuffer(const MeshType& mesh) // override
    {
        computeWireframeLines(mesh);
    }

    void setTextures(const MeshType& mesh) // override
    {
        // lambda that sets a texture
        auto setTexture = [&](const Image&       img,
                              const std::string& path,
                              bool               generateMips) {
            const uint size = img.width() * img.height();
            assert(size > 0);

            uint sizeWithMips = bimg::imageGetSize(
                                    nullptr,
                                    img.width(),
                                    img.height(),
                                    1,
                                    false,
                                    generateMips,
                                    1,
                                    bimg::TextureFormat::RGBA8) /
                                4; // in uints
            uint numMips = 1;
            if (generateMips)
                numMips = bimg::imageGetNumMips(
                    bimg::TextureFormat::RGBA8, img.width(), img.height());

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(sizeWithMips);

            const uint* tdata = reinterpret_cast<const uint*>(img.data());

            std::copy(tdata, tdata + size, buffer); // mip level 0

            if (numMips > 1) {
                uint* source = buffer;
                uint* dest;
                uint  offset = size;
                for (uint mip = 1; mip < numMips; mip++) {
                    dest         = source + offset;
                    uint mipSize = (img.width() >> mip) * (img.height() >> mip);
                    bimg::imageRgba8Downsample2x2(
                        dest,                      // output location
                        img.width() >> (mip - 1),  // input width
                        img.height() >> (mip - 1), // input height
                        1, // depth, always 1 for 2D textures
                        (img.width() >> (mip - 1)) * 4, // input pitch
                        (img.width() >> mip) * 4,       // output pitch
                        source                          // input location
                    );
                    source = dest;
                    offset = mipSize;
                }
            }

            uint64_t flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;

            if (img.colorSpace() == Image::ColorSpace::SRGB)
                flags |= BGFX_TEXTURE_SRGB;

            Texture tex;
            tex.set(
                buffer,
                vcl::Point2i(img.width(), img.height()),
                generateMips,
                flags,
                releaseFn);

            // at() does not insert if already present, thus safe in parallel
            mMaterialTextures.at(path) = std::move(tex);
        };

        auto loadImageAndSetTexture =
            [&](const std::pair<std::string, uint>& pathPair) {
                const std::string& path = pathPair.first;

                uint materialId  = pathPair.second / N_TEXTURE_TYPES;
                uint textureType = pathPair.second % N_TEXTURE_TYPES;
                // copy the image because it could be not loaded,
                // and at the end it needs to be mirrored.
                vcl::Image txtImg = mesh.textureImage(path);
                if (txtImg.isNull()) { // try to load it just for rendering
                    try {
                        txtImg = vcl::loadImage(mesh.meshBasePath() + path);
                        txtImg.colorSpace() = Material::textureTypeToColorSpace(
                            static_cast<Material::TextureType>(textureType));
                    }
                    catch (...) {
                        // do nothing
                    }
                    if (txtImg.isNull()) {
                        // still null, use a dummy texture
                        txtImg = createCheckBoardImage(512);
                    }
                }

                // if loading succeeded (or dummy texture has been created)
                if (!txtImg.isNull()) {
                    const TextureDescriptor& tex =
                        mesh.material(materialId)
                            .textureDescriptor(textureType);
                    using enum TextureDescriptor::MinificationFilter;

                    TextureDescriptor::MinificationFilter minFilter =
                        tex.minFilter();

                    bool hasMips =
                        minFilter >= NEAREST_MIPMAP_NEAREST ||
                        minFilter == NONE; // default LINEAR_MIPMAP_LINEAR

                    txtImg.mirror();
                    setTexture(txtImg, path, hasMips);
                }
            };

        mMaterialTextures.clear();

        if constexpr (vcl::HasMaterials<MeshType>) {
            // textures could be missing from the textureImages of the mesh
            // setting the texture paths in a map - key is the path and value is
            // an uint where materialIndex and textureType are encoded
            // map is used to avoid duplicates, then is moved to a vector for
            // parallel processing
            std::map<std::string, uint> texturePaths;
            for (uint i = 0; i < mesh.materialsNumber(); ++i) {
                for (uint j = 0; j < N_TEXTURE_TYPES; ++j) {
                    const vcl::TextureDescriptor& td =
                        mesh.material(i).textureDescriptor(j);
                    if (!td.path().empty()) {
                        texturePaths[td.path()] = i * N_TEXTURE_TYPES + j;

                        // create a null texture in the map
                        // this is crucial to avoid insertions during the
                        // actual creation, that is done in parallel
                        mMaterialTextures[td.path()] = Texture();
                    }
                }
            }

            // move to vector for parallel processing
            std::vector<std::pair<std::string, uint>> texturePathVec;
            texturePathVec.reserve(texturePaths.size());
            for (const auto& tp : texturePaths) {
                texturePathVec.push_back(tp);
            }

            parallelFor(texturePathVec, loadImageAndSetTexture);

            createTextureSamplerUniforms();
        }
    }

    void setMeshUniforms(const MeshType& mesh) // override
    {
        mMeshUniforms.update(mesh);
        if constexpr (HasColor<MeshType>) {
            mMeshColor = mesh.color();
        }
    }

    void computeEdgeLines(const MeshType& mesh)
    {
        // if cpu lines, do this...

        // positions
        const uint         nv = Base::numVerts();
        std::vector<float> positions(nv * 3);
        Base::fillVertexPositions(mesh, positions.data());

        // indices
        const uint        ne = Base::numEdges();
        std::vector<uint> indices(ne * 2);
        Base::fillEdgeIndices(mesh, indices.data());

        // v normals
        std::vector<float> normals;
        if (mVertexNormalsBuffer.isValid()) {
            normals.resize(nv * 3);
            Base::fillVertexNormals(mesh, normals.data());
        }

        // todo - edge normals

        // vcolors
        std::vector<uint> vcolors;
        if (mVertexColorsBuffer.isValid()) {
            vcolors.resize(nv);
            Base::fillVertexColors(mesh, vcolors.data(), Color::Format::ABGR);
        }

        std::vector<uint> ecolors;
        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
            if (vcl::isPerEdgeColorAvailable(mesh)) {
                // if (btu[toUnderlying(EDGE_COLORS)]) {
                //  edge color buffer
                ecolors.resize(ne);
                Base::fillEdgeColors(mesh, ecolors.data(), Color::Format::ABGR);
                //}
            }
        }

        mEdgeLines.setPoints(positions, indices, normals, vcolors, ecolors);

        // otherwise, already computed buffers should do the job
    }

    // to generate wireframe lines
    void computeWireframeLines(const MeshType& mesh)
    {
        // if cpu lines, do this...

        // positions
        const uint         nv = Base::numVerts();
        std::vector<float> positions(nv * 3);
        Base::fillVertexPositions(mesh, positions.data());

        // indices
        const uint        nw = Base::numWireframeLines();
        std::vector<uint> indices(nw * 2);
        Base::fillWireframeIndices(mesh, indices.data());

        // v normals
        std::vector<float> normals;
        if (mVertexNormalsBuffer.isValid()) {
            normals.resize(nv * 3);
            Base::fillVertexNormals(mesh, normals.data());
        }

        // vcolors
        std::vector<uint> vcolors;
        if (mVertexColorsBuffer.isValid()) {
            vcolors.resize(nv);
            Base::fillVertexColors(mesh, vcolors.data(), Color::Format::ABGR);
        }

        mWireframeLines.setPoints(positions, indices, normals, vcolors, {});

        // otherwise, already computed buffers should do the job
    }

    void createTextureSamplerUniforms()
    {
        for (uint i = 0; i < mTextureSamplerUniforms.size(); ++i) {
            mTextureSamplerUniforms[i] = Uniform(
                Material::TEXTURE_TYPE_NAMES[i].c_str(),
                bgfx::UniformType::Sampler);
        }
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
