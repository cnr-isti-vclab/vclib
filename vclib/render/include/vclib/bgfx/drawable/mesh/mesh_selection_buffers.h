/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_SELECTION_BUFFERS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_SELECTION_BUFFERS_H

#include "mesh_render_buffers_macros.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/selection_uniforms.h>
#include <vclib/bgfx/read_from_gpu_buffer.h>
#include <vclib/bgfx/uniform.h>

#include <vclib/render/selection/selection_mode.h>
#include <vclib/render/selection/selection_parameters.h>

#include <vclib/space/complex.h>
#include <vclib/space/core.h>

#include <bgfx/bgfx.h>

#include <array>
#include <bit>
#include <cmath>
#include <optional>
#include <vector>

namespace vcl {

/**
 * @brief Manages all GPU-side selection state for a mesh rendered with bgfx.
 *
 * This class encapsulates:
 * - Vertex and face selection bitfield buffers (GPU compute read/write)
 * - Polygon-to-triangle mapping buffers used by face-selection compute shaders
 * - Workgroup size bookkeeping for compute dispatch
 * - Asynchronous GPU→CPU readback of selection results
 *
 * It is non-templated on the mesh type; callers pass primitive counts and
 * geometry buffer handles explicitly.
 *
 * Typical lifecycle (driven by MeshRenderBuffers):
 * 1. initVertexSelectionBitfield() when vertex positions are uploaded
 * 2. initFaceSelectionBitfield() at the same time (face count is known)
 * 3. initPolyMapping() after triangle indices are uploaded
 * 4. initReadbackHandler() once, after both counts are known
 */
class MeshSelectionBuffers
{
    // vertex selection
    IndexBuffer mSelectedVerticesBuffer;

    std::array<uint, 3> mVertexSelectionWorkgroupSize = {0, 0, 0};
    uint                mNumVerts                     = 0;

    // face selection (invalid if mesh has no faces)
    IndexBuffer         mSelectedFacesBuffer;
    std::array<uint, 3> mFaceSelectionWorkgroupSize = {0, 0, 0};
    uint                mNumTris                    = 0;

    // Polygon-to-triangle mapping buffers for polygon-level face selection
    IndexBuffer mTriToPolyBuffer;
    IndexBuffer mPolyToTriBeginBuffer;
    IndexBuffer mPolyToTriCountBuffer;

    // Handler used to copy selection buffers to CPU
    detail::ReadFromGPUBuffer mSelectionToCPUBufferHandler;

    // This allows selection for a maximum of 2048^3 = 8_589_934_592
    // vertices/faces per mesh.
    inline static const uint MAX_COMPUTE_WORKGROUP_SIZE = 2048;

public:
    MeshSelectionBuffers() = default;

    void swap(MeshSelectionBuffers& other)
    {
        using std::swap;
        swap(mSelectedVerticesBuffer, other.mSelectedVerticesBuffer);
        swap(
            mVertexSelectionWorkgroupSize, other.mVertexSelectionWorkgroupSize);
        swap(mNumVerts, other.mNumVerts);
        swap(mSelectedFacesBuffer, other.mSelectedFacesBuffer);
        swap(mFaceSelectionWorkgroupSize, other.mFaceSelectionWorkgroupSize);
        swap(mNumTris, other.mNumTris);
        swap(mTriToPolyBuffer, other.mTriToPolyBuffer);
        swap(mPolyToTriBeginBuffer, other.mPolyToTriBeginBuffer);
        swap(mPolyToTriCountBuffer, other.mPolyToTriCountBuffer);
        swap(mSelectionToCPUBufferHandler, other.mSelectionToCPUBufferHandler);
    }

    friend void swap(MeshSelectionBuffers& a, MeshSelectionBuffers& b)
    {
        a.swap(b);
    }

    // ---- Init (called from MeshRenderBuffers CRTP hooks) ----------------

    /**
     * @brief Allocates the vertex selection bitfield on the GPU.
     *
     * The bitfield has one bit per vertex, packed into uint32 words.
     * All bits are initialised to zero (no vertex selected).
     *
     * @param[in] numVerts: Number of (possibly duplicated) vertices.
     */
    void initVertexSelectionBitfield(uint numVerts)
    {
        mNumVerts = numVerts;

        const uint selectionBufferSize =
            uint(std::ceil(double(numVerts) / 32.0));

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(selectionBufferSize);

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

    /**
     * @brief Allocates the face selection bitfield on the GPU.
     *
     * The bitfield has one bit per triangle, packed into uint32 words.
     * All bits are initialised to zero (no face selected).
     * If @p numTris is zero the optional buffer is set to nullopt.
     *
     * @param[in] numTris: Number of triangles after triangulation.
     */
    void initFaceSelectionBitfield(uint numTris)
    {
        mNumTris = numTris;

        if (numTris == 0) {
            mSelectedFacesBuffer.destroy();
            mFaceSelectionWorkgroupSize = {0, 0, 0};
            return;
        }

        const uint selectionBufferSize =
            uint(std::ceil(double(numTris) / 32.0));

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(selectionBufferSize);

        for (uint i = 0; i < selectionBufferSize; i++) {
            buffer[i] = 0;
        }

        mSelectedFacesBuffer.createForCompute(
            buffer,
            selectionBufferSize,
            vcl::PrimitiveType::UINT,
            bgfx::Access::ReadWrite,
            releaseFn);

        calculateFaceSelectionWorkgroupSize();
    }

    /**
     * @brief Uploads the polygon-to-triangle mapping buffers to the GPU.
     *
     * These three buffers allow compute shaders to convert from triangle
     * indices back to the original polygon indices, which is required for
     * polygon-level face selection on triangulated meshes.
     *
     * Call this after the triangle index buffer has been built (and therefore
     * after @p indexMap has been populated by fillTriangleIndices).
     *
     * @param[in] indexMap: Bidirectional triangle ↔ polygon index map.
     * @param[in] numTris: Number of triangles (after triangulation).
     */
    void initPolyMapping(const TriPolyIndexBiMap& indexMap, uint numTris)
    {
        if (numTris == 0)
            return;

        const uint numPolys = indexMap.polygonCount();

        // tri_to_poly[triIdx] = polygon index
        {
            auto [buf, rel] =
                Context::getAllocatedBufferAndReleaseFn<uint>(numTris);
            for (uint i = 0; i < numTris; i++) {
                buf[i] = indexMap.polygon(i);
            }
            mTriToPolyBuffer.createForCompute(
                buf, numTris, PrimitiveType::UINT, bgfx::Access::Read, rel);
        }

        // poly_to_tri_begin[polyIdx] and poly_to_tri_count[polyIdx]
        {
            auto [bufBegin, relBegin] =
                Context::getAllocatedBufferAndReleaseFn<uint>(numPolys);
            auto [bufCount, relCount] =
                Context::getAllocatedBufferAndReleaseFn<uint>(numPolys);

            for (uint i = 0; i < numPolys; i++) {
                uint begin = indexMap.triangleBegin(i);
                if (begin != UINT_NULL) {
                    bufBegin[i] = begin;
                    bufCount[i] = indexMap.triangleCount(i);
                }
                else {
                    // deleted polygon — will never be accessed by a valid
                    // triangle thread
                    bufBegin[i] = 0;
                    bufCount[i] = 0;
                }
            }

            mPolyToTriBeginBuffer.createForCompute(
                bufBegin,
                numPolys,
                PrimitiveType::UINT,
                bgfx::Access::Read,
                relBegin);
            mPolyToTriCountBuffer.createForCompute(
                bufCount,
                numPolys,
                PrimitiveType::UINT,
                bgfx::Access::Read,
                relCount);
        }
    }

    /**
     * @brief Allocates the GPU→CPU readback handler.
     *
     * The handler is sized to accommodate reading either the vertex or the
     * face selection bitfield (whichever is larger).
     *
     * @param[in] maxElements: max(numVerts, numTris).
     */
    void initReadbackHandler(uint maxElements)
    {
        mSelectionToCPUBufferHandler = std::move(
            detail::ReadFromGPUBuffer(
                uint(std::ceil(double(maxElements) / 8.0))));
    }

    // ---- Selection operations -------------------------------------------

    /**
     * @brief Vertex selection dispatch (atomic or box-based).
     *
     * Checks whether the action is atomic (ALL / NONE / INVERT) and delegates
     * to the appropriate private helper. For non-atomic actions the vertex
     * positions buffer is required.
     *
     * @param[in] params: Selection parameters (box, mode, view id, …).
     * @param[in] model: Mesh model matrix (used to build modelViewProj in
     * shader).
     * @param[in] vertPosBuf: Vertex positions buffer (only used for
     * box-based selection).
     */
    bool vertexSelection(
        const SelectionParameters& params,
        const Matrix44f&           model,
        const VertexBuffer&        vertPosBuf)
    {
        if (params.mode.isAtomicAction())
            return vertexSelectionAtomic(params);
        else
            return vertexSelectionNonAtomic(params, model, vertPosBuf);
    }

    /**
     * @brief Face selection dispatch (atomic or box-based).
     *
     * Checks whether the action is atomic (ALL / NONE / INVERT) and delegates
     * to the appropriate private helper. For non-atomic actions the vertex
     * positions and triangle index buffers are required.
     *
     * @param[in] params: Selection parameters (box, mode, view id, …).
     * @param[in] model: Mesh model matrix (used to build modelViewProj in
     * shader).
     * @param[in] vertPosBuf: Vertex positions buffer (only used for
     * box-based selection).
     * @param[in] triIdxBuf: Triangle index buffer (only used for
     * box-based selection).
     */
    bool faceSelection(
        const SelectionParameters& params,
        const Matrix44f&           model,
        const VertexBuffer&        vertPosBuf,
        const IndexBuffer&         triIdxBuf)
    {
        if (params.mode.isAtomicAction())
            return faceSelectionAtomic(params);
        else
            return faceSelectionNonAtomic(params, model, vertPosBuf, triIdxBuf);
    }

    /**
     * @brief Visible-face selection via a two-pass render + compute pipeline.
     *
     * Pass 1 (vert+frag): renders primitive IDs and mesh IDs into two colour
     * attachments.  Pass 2 (compute): reads those attachments and updates the
     * face selection bitfield.
     *
     * @param[in] params: Selection parameters (includes texture handles,
     * sizes…)
     * @param[in] model: Mesh model matrix.
     * @param[in] vertPosBuf: Vertex positions buffer.
     * @param[in] triIdxBuf: Triangle index buffer.
     */
    bool faceSelectionVisible(
        const SelectionParameters& params,
        const Matrix44f&           model,
        const VertexBuffer&        vertPosBuf,
        const IndexBuffer&         triIdxBuf)
    {
        if (params.mode.primitive == SelectionPrimitive::FACE &&
            params.mode.action == SelectionAction::REGULAR) {
            SelectionParameters params2(params);
            params2.drawViewId  = params2.pass1ViewId;
            params2.mode.action = SelectionAction::NONE;
            faceSelectionAtomic(params2);
        }

        ProgramManager& pm = Context::instance().programManager();

        bgfx::ProgramHandle passProgram = pm.getProgram<
            VertFragProgram::SELECTION_FACE_VISIBLE_RENDER_PASS>();
        bgfx::ProgramHandle computeProg =
            getComputeProgramFromSelectionMode(pm, params.mode);

        std::array<uint, 3> workGroupSize = workGroupSizesFrom1DSize(
            params.texAttachsSize[0] * params.texAttachsSize[1]);

        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;
        bgfx::setState(state);
        SelectionUniforms::setMeshIdForSelection(params.meshId);
        SelectionUniforms::setSelectionWorkgroupSize(workGroupSize);
        SelectionUniforms::bind();
        vertPosBuf.bindVertex(VCL_MRB_VERTEX_POSITION_STREAM);
        triIdxBuf.bind();
        bgfx::setTransform(model.data());
        bgfx::submit(params.pass1ViewId, passProgram);

        SelectionUniforms::setSelectionAction(params.mode.action);
        SelectionUniforms::setMeshIdForSelection(params.meshId);
        SelectionUniforms::setSelectionWorkgroupSize(workGroupSize);
        SelectionUniforms::bind();
        bgfx::setImage(
            0,
            params.primIdTex,
            0,
            bgfx::Access::Read,
            bgfx::TextureFormat::RGBA8);
        bgfx::setImage(
            1,
            params.meshIdTex,
            0,
            bgfx::Access::Read,
            bgfx::TextureFormat::RGBA8);
        mSelectedFacesBuffer.bind(6, bgfx::Access::ReadWrite);
        mTriToPolyBuffer.bind(7, bgfx::Access::Read);
        mPolyToTriBeginBuffer.bind(8, bgfx::Access::Read);
        mPolyToTriCountBuffer.bind(9, bgfx::Access::Read);
        bgfx::setTransform(model.data());
        bgfx::dispatch(
            params.pass2ViewId,
            computeProg,
            workGroupSize[0],
            workGroupSize[1],
            workGroupSize[2]);
        return true;
    }

    // ---- CPU readback ---------------------------------------------------

    /**
     * @brief Requests an asynchronous GPU→CPU copy of the selection bitfield.
     *
     * @param[in] mode: Selection mode (determines vertex vs face buffer).
     * @return The number of frames the caller must wait before the result is
     * available via getSelectionBufferCopy().
     */
    uint requestCPUCopyOfSelectionBuffer(const SelectionMode& mode)
    {
        constexpr uint elementBitSize = 1;
        uint           elementCount;
        IndexBuffer*   idxBuf;

        if (mode.isFaceSelection()) {
            idxBuf       = &(mSelectedFacesBuffer);
            elementCount = mNumTris;
        }
        else {
            idxBuf       = &mSelectedVerticesBuffer;
            elementCount = mNumVerts;
        }

        mSelectionToCPUBufferHandler.submit(
            *idxBuf, elementCount, elementBitSize, nullptr);
        return 2;
    }

    std::vector<uint8_t> getSelectionBufferCopy() const
    {
        return std::move(mSelectionToCPUBufferHandler.getResultsCopy());
    }

    void setVertexSelectionFromCPUBuffer(const std::vector<uint8_t>& backup)
    {
        setSelectionBufferFromCPUBuffer(backup, mSelectedVerticesBuffer);
    }

    void setFaceSelectionFromCPUBuffer(const std::vector<uint8_t>& backup)
    {
        setSelectionBufferFromCPUBuffer(backup, mSelectedFacesBuffer);
    }

    // ---- State queries --------------------------------------------------

    bool hasVertexSelectionBuffer() const
    {
        return mSelectedVerticesBuffer.isValid();
    }

    bool hasFaceSelectionBuffer() const
    {
        return mSelectedFacesBuffer.isValid();
    }

    // ---- Bind -----------------------------------------------------------

    void bindSelectedVerticesBuffer() const { mSelectedVerticesBuffer.bind(4); }

    void bindSelectedFacesBuffer() const { mSelectedFacesBuffer.bind(6); }

private:
    /**
     * @brief Clears the vertex selection bitfield by dispatching a NONE action.
     *
     * @param[in] drawViewId: View ID for the dispatch.
     */
    void clearVertexSelection(uint drawViewId)
    {
        SelectionParameters params;
        params.drawViewId = drawViewId;
        params.mode.action = SelectionAction::NONE;
        vertexSelectionAtomic(params);
    }

    /**
     * @brief Clears the face selection bitfield by dispatching a NONE action.
     *
     * @param[in] drawViewId: View ID for the dispatch.
     */
    void clearFaceSelection(uint drawViewId)
    {
        SelectionParameters params;
        params.drawViewId = drawViewId;
        params.mode.action = SelectionAction::NONE;
        faceSelectionAtomic(params);
    }

    /**
     * @brief Box-based vertex selection (non-atomic).
     *
     * Returns false if the selection box is not yet fully defined.
     *
     * @param[in] params: Selection parameters (box, mode, view id, …).
     * @param[in] model: Mesh model matrix (used to build modelViewProj in
     * shader).
     * @param[in] vertPosBuf: Vertex positions buffer (read-only in the shader).
     */
    bool vertexSelectionNonAtomic(
        const SelectionParameters& params,
        const Matrix44f&           model,
        const VertexBuffer&        vertPosBuf)
    {
        if (params.box.isNull()) {
            return false;
        }

        // For REGULAR mode, first clear the entire vertex selection buffer
        if (params.mode.action == SelectionAction::REGULAR) {
            clearVertexSelection(params.drawViewId);
        }
        SelectionUniforms::setSelectionAction(params.mode.action);

        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(
            Context::instance().programManager(), params.mode);

        SelectionUniforms::setSelectionBox(params.box);
        SelectionUniforms::setSelectionWorkgroupSize(
            mVertexSelectionWorkgroupSize);
        SelectionUniforms::setNumPrimitivesForSelection(mNumVerts);
        SelectionUniforms::bind();
        mSelectedVerticesBuffer.bind(4, bgfx::Access::ReadWrite);
        vertPosBuf.bindCompute(
            VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
        bgfx::setTransform(model.data());
        dispatchVertexSelection(params.drawViewId, prog);
        return true;
    }

    /**
     * @brief Atomic vertex selection (ALL / NONE / INVERT — no box needed).
     *
     * @param[in] params: Selection parameters (mode, view id, …).
     */
    bool vertexSelectionAtomic(const SelectionParameters& params)
    {
        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(
            Context::instance().programManager(), params.mode);
        SelectionUniforms::setSelectionWorkgroupSize(
            mVertexSelectionWorkgroupSize);
        SelectionUniforms::setNumPrimitivesForSelection(mNumVerts);
        SelectionUniforms::bind();
        mSelectedVerticesBuffer.bind(4, bgfx::Access::ReadWrite);
        dispatchVertexSelection(params.drawViewId, prog);
        return true;
    }

    /**
     * @brief Box-based face selection (non-atomic).
     *
     * Returns false if the selection box is not yet fully defined.
     *
     * @param[in] params: Selection parameters.
     * @param[in] model: Mesh model matrix (used to build modelViewProj in
     * shader).
     * @param[in] vertPosBuf: Vertex positions buffer.
     * @param[in] triIdxBuf: Triangle index buffer.
     */
    bool faceSelectionNonAtomic(
        const SelectionParameters& params,
        const Matrix44f&           model,
        const VertexBuffer&        vertPosBuf,
        const IndexBuffer&         triIdxBuf)
    {
        if (params.box.isNull()) {
            return false;
        }

        // For REGULAR mode, first clear the entire face selection buffer
        if (params.mode.action == SelectionAction::REGULAR) {
            clearFaceSelection(params.drawViewId);
        }

        SelectionUniforms::setSelectionAction(params.mode.action);

        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(
            Context::instance().programManager(), params.mode);

        SelectionUniforms::setSelectionBox(params.box);
        SelectionUniforms::setSelectionWorkgroupSize(
            mFaceSelectionWorkgroupSize);
        SelectionUniforms::setNumPrimitivesForSelection(mNumTris);
        SelectionUniforms::bind();
        mSelectedFacesBuffer.bind(6, bgfx::Access::ReadWrite);
        vertPosBuf.bindCompute(
            VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
        triIdxBuf.bind(5, bgfx::Access::Read);
        mTriToPolyBuffer.bind(7, bgfx::Access::Read);
        mPolyToTriBeginBuffer.bind(8, bgfx::Access::Read);
        mPolyToTriCountBuffer.bind(9, bgfx::Access::Read);
        bgfx::setTransform(model.data());
        dispatchFaceSelection(params.drawViewId, prog);
        return true;
    }

    /**
     * @brief Atomic face selection (ALL / NONE / INVERT).
     *
     * @param[in] params: Selection parameters.
     */
    bool faceSelectionAtomic(const SelectionParameters& params)
    {
        bgfx::ProgramHandle prog = getComputeProgramFromSelectionMode(
            Context::instance().programManager(), params.mode);
        SelectionUniforms::setSelectionWorkgroupSize(
            mFaceSelectionWorkgroupSize);
        SelectionUniforms::setNumPrimitivesForSelection(mNumTris);
        SelectionUniforms::bind();
        mSelectedFacesBuffer.bind(4, bgfx::Access::ReadWrite);
        dispatchFaceSelection(params.drawViewId, prog);
        return true;
    }

    void dispatchVertexSelection(
        const uint                 viewId,
        const bgfx::ProgramHandle& handle)
    {
        bgfx::dispatch(
            viewId,
            handle,
            mVertexSelectionWorkgroupSize[0],
            mVertexSelectionWorkgroupSize[1],
            mVertexSelectionWorkgroupSize[2]);
    }

    void dispatchFaceSelection(
        const uint                 viewId,
        const bgfx::ProgramHandle& handle)
    {
        bgfx::dispatch(
            viewId,
            handle,
            mFaceSelectionWorkgroupSize[0],
            mFaceSelectionWorkgroupSize[1],
            mFaceSelectionWorkgroupSize[2]);
    }

    static void setSelectionBufferFromCPUBuffer(
        const std::vector<uint8_t>& backup,
        IndexBuffer&                sBuffer)
    {
        if (backup.empty() || !sBuffer.isValid())
            return;

        const uint elementCount =
            (uint(backup.size()) + uint(sizeof(uint32_t)) - 1u) /
            uint(sizeof(uint32_t));
        if (elementCount == 0)
            return;

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint32_t>(elementCount);

        // The backup bytes are big-endian (uintRGBAToVec4Color stores MSB
        // in the R channel). Reconstruct each uint32 with proper byte order.
        for (uint i = 0; i < elementCount; i++) {
            const uint base = 4u * i;
            uint32_t   val  = 0;
            if (base + 0u < uint(backup.size()))
                val |= uint32_t(backup[base + 0u]) << 24;
            if (base + 1u < uint(backup.size()))
                val |= uint32_t(backup[base + 1u]) << 16;
            if (base + 2u < uint(backup.size()))
                val |= uint32_t(backup[base + 2u]) << 8;
            if (base + 3u < uint(backup.size()))
                val |= uint32_t(backup[base + 3u]);
            buffer[i] = val;
        }

        sBuffer.destroy();
        sBuffer.createForCompute(
            buffer,
            elementCount,
            vcl::PrimitiveType::UINT,
            bgfx::Access::ReadWrite,
            releaseFn);
    }

    bgfx::ProgramHandle getComputeProgramFromSelectionMode(
        ProgramManager& pm,
        SelectionMode   mode) const
    {
        using enum SelectionAction;
        using enum SelectionPrimitive;
        using enum ComputeProgram;

        switch (mode.primitive) {
        case VERTEX:
            switch (mode.action) {
            case REGULAR:
            case ADD:
            case SUBTRACT: return pm.getComputeProgram<SELECTION_VERTEX>();
            case ALL: return pm.getComputeProgram<SELECTION_ALL>();
            case NONE: return pm.getComputeProgram<SELECTION_NONE>();
            case INVERT: return pm.getComputeProgram<SELECTION_INVERT>();
            }

        case FACE:
            switch (mode.action) {
            case REGULAR:
            case ADD:
            case SUBTRACT:
                if (mode.visible)
                    return pm.getComputeProgram<SELECTION_FACE_VISIBLE>();
                else
                    return pm.getComputeProgram<SELECTION_FACE>();
            case ALL: return pm.getComputeProgram<SELECTION_ALL>();
            case NONE: return pm.getComputeProgram<SELECTION_NONE>();
            case INVERT: return pm.getComputeProgram<SELECTION_INVERT>();
            }
        default:
            // Invalid mode — should never happen
            assert(false && "Invalid selection mode");
            return BGFX_INVALID_HANDLE;
        }
    }

    /**
     * @brief Converts a 1-D element count into a 3-D compute dispatch shape.
     *
     * Distribute the load across as cubic a grid as possible so no dimension
     * exceeds MAX_COMPUTE_WORKGROUP_SIZE (2048).
     *
     * @param[in] size: Total number of elements.
     * @return {x, y, z} workgroup counts.
     */
    static std::array<uint, 3> workGroupSizesFrom1DSize(uint size)
    {
        std::array<uint, 3> sizes = {1, 1, 1};
        if (size == 0)
            return sizes;

        uint dim = uint(std::ceil(std::cbrt(size)));

        sizes[0] = std::min(dim, MAX_COMPUTE_WORKGROUP_SIZE);

        // Cap at sizes[0] to keep the grid roughly cubic rather than letting
        // one dimension grow much larger than the others.
        const uint remainingWork = uint(std::ceil(double(size) / sizes[0]));
        sizes[1] = std::min(remainingWork, sizes[0]);
        sizes[2] = uint(std::ceil(double(size) / (sizes[0] * sizes[1])));

        return sizes;
    }

    void calculateVertexSelectionWorkgroupSize()
    {
        mVertexSelectionWorkgroupSize = workGroupSizesFrom1DSize(mNumVerts);
    }

    void calculateFaceSelectionWorkgroupSize()
    {
        if (mNumTris == 0) {
            mFaceSelectionWorkgroupSize = {0, 0, 0};
            return;
        }
        mFaceSelectionWorkgroupSize = workGroupSizesFrom1DSize(mNumTris);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_SELECTION_BUFFERS_H
