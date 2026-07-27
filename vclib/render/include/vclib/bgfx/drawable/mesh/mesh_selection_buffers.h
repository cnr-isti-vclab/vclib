// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_SELECTION_BUFFERS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_SELECTION_BUFFERS_H

#include "mesh_render_buffers_macros.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/buffers/boolean_buffer.h>
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
#include <cmath>
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
    BooleanBuffer       mVertexSelection;
    uint                mNumSelectedVertices                = 0;
    std::array<uint, 3> mVertexSelectionWorkgroupSize       = {0, 0, 0};
    std::array<uint, 3> mVertexSelectionAtomicWorkgroupSize = {0, 0, 0};

    // face selection (invalid if mesh has no faces)
    BooleanBuffer       mFaceSelection;
    uint                mNumSelectedFaces                 = 0;
    std::array<uint, 3> mFaceSelectionWorkgroupSize       = {0, 0, 0};
    std::array<uint, 3> mFaceSelectionAtomicWorkgroupSize = {0, 0, 0};

    // Polygon-to-triangle mapping buffers for polygon-level face selection
    IndexBuffer mTriToPolyBuffer;
    IndexBuffer mPolyToTriBeginBuffer;
    IndexBuffer mPolyToTriCountBuffer;

    uint mBufToTexRemainingFrames = UINT_NULL; // NULL means no frames remaining
    SelectionMode mLastReadbackMode;
    bool          mHasPendingReadback = false;
    SelectionMode mPendingReadbackMode;

    // This allows selection for a maximum of 2048^3 = 8_589_934_592
    // vertices/faces per mesh.
    inline static const uint MAX_COMPUTE_WORKGROUP_SIZE = 2048;

public:
    MeshSelectionBuffers() = default;

    void swap(MeshSelectionBuffers& other)
    {
        using std::swap;
        swap(mVertexSelection, other.mVertexSelection);
        swap(mNumSelectedVertices, other.mNumSelectedVertices);
        swap(
            mVertexSelectionWorkgroupSize, other.mVertexSelectionWorkgroupSize);
        swap(
            mVertexSelectionAtomicWorkgroupSize,
            other.mVertexSelectionAtomicWorkgroupSize);
        swap(mFaceSelection, other.mFaceSelection);
        swap(mNumSelectedFaces, other.mNumSelectedFaces);
        swap(mFaceSelectionWorkgroupSize, other.mFaceSelectionWorkgroupSize);
        swap(
            mFaceSelectionAtomicWorkgroupSize,
            other.mFaceSelectionAtomicWorkgroupSize);
        swap(mTriToPolyBuffer, other.mTriToPolyBuffer);
        swap(mPolyToTriBeginBuffer, other.mPolyToTriBeginBuffer);
        swap(mPolyToTriCountBuffer, other.mPolyToTriCountBuffer);
        swap(mBufToTexRemainingFrames, other.mBufToTexRemainingFrames);
        swap(mLastReadbackMode, other.mLastReadbackMode);
        swap(mHasPendingReadback, other.mHasPendingReadback);
        swap(mPendingReadbackMode, other.mPendingReadbackMode);
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
        mVertexSelection.init(numVerts);
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
        mFaceSelection.init(numTris);
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
            mTriToPolyBuffer.create(
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

            mPolyToTriBeginBuffer.create(
                bufBegin,
                numPolys,
                PrimitiveType::UINT,
                bgfx::Access::Read,
                relBegin);
            mPolyToTriCountBuffer.create(
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
    void initReadbackHandler(uint) {}

    // --- Set calls

    template<MeshConcept MeshType>
    void setVertexSelectionFromMesh(const MeshType& mesh)
    {
        const uint numVerts = mesh.vertexCount();
        if (numVerts == 0)
            return;

        // Compute number of bits rounded to 32 needed to store vertex selection
        // We use mesh.vertexCount() which includes duplicated vertices
        uint                 bitNumber = vcl::roundUp(numVerts, 32);
        std::vector<uint8_t> vertexBackup(bitNumber / 8, 0);

        // Build bitfield from mesh vertex selection flags
        // Note: For duplicated vertices (indices >= numVerts), they will
        // remain unselected unless explicitly set elsewhere
        uint                       vidx    = 0;
        uint                       byteIdx = 0;
        vcl::BitSet<uint8_t, true> flags;
        for (const auto& v : mesh.vertices()) {
            flags[vidx % 8] = v.selected();
            ++vidx;

            if (vidx % 8 == 0) {
                vertexBackup[byteIdx] = flags.underlying();
                byteIdx++;
                flags.reset();
            }
        }
        // Handle remaining bits
        if (vidx % 8 != 0)
            vertexBackup[byteIdx] = flags.underlying();

        mVertexSelection.setFromCPUBuffer(vertexBackup);
    }

    template<MeshConcept MeshType>
    void setFaceSelectionFromMesh(
        const MeshType&          mesh,
        const TriPolyIndexBiMap& indexMap)
    {
        const uint numFaces = mesh.faceCount();
        if (numFaces == 0 || !hasFaceSelectionBuffer())
            return;

        // Compute number of bits rounded to 32 needed to store face
        // selection
        const uint wordCount = (indexMap.triangleCount() + 31) / 32;
        uint       bitNumber = vcl::roundUp(indexMap.triangleCount(), 32);
        std::vector<uint8_t> faceBackup(bitNumber / 8, 0);

        // For each face, set selection for all its triangles
        uint                       tIdx    = 0;
        uint                       byteIdx = 0;
        vcl::BitSet<uint8_t, true> flags;
        for (const auto& f : mesh.faces()) {
            const uint faceIdx     = f.index();
            const uint numFaceTris = indexMap.triangleCount(faceIdx);

            // Set selection for all triangles of this face
            for (uint t = 0; t < numFaceTris; ++t) {
                flags[tIdx % 8] = f.selected();
                ++tIdx;

                if (tIdx % 8 == 0) {
                    faceBackup[byteIdx] = flags.underlying();
                    byteIdx++;
                    flags.reset();
                }
            }
        }
        // Handle remaining bits
        if (tIdx % 8 != 0)
            faceBackup[byteIdx] = flags.underlying();

        mFaceSelection.setFromCPUBuffer(faceBackup);
    }

    // ---- Selection operations -------------------------------------------

    void computeSelection(
        const SelectionParameters& params,
        const Matrix44f&           model,
        const VertexBuffer&        vertPosBuf,
        const IndexBuffer&         triIdxBuf)
    {
        bool toCompute = false;

        if (params.mode.isFaceSelection()) {
            if (params.mode.isVisibleSelection()) {
                toCompute =
                    faceSelectionVisible(params, model, vertPosBuf, triIdxBuf);
            }
            else {
                toCompute = faceSelection(params, model, vertPosBuf, triIdxBuf);
            }
        }
        else if (params.mode.isVertexSelection()) {
            toCompute = vertexSelection(params, model, vertPosBuf);
        }

        if (toCompute && !params.isTemporary) {
            // If no readback is in progress, start one. Otherwise queue the
            // most recent mode so both vertex and face backups stay
            // synchronized even when two tools trigger selection of both
            // vertices and faces.
            if (mBufToTexRemainingFrames == UINT_NULL) {
                mLastReadbackMode = params.mode;
                mBufToTexRemainingFrames =
                    requestCPUCopyOfSelectionBuffer(params.mode);
            }
            else {
                // Queue the most recent mode so both vertex and face backups
                // stay synchronized even when two tools trigger selection in
                // sequence.
                mPendingReadbackMode = params.mode;
                mHasPendingReadback  = true;
            }
        }
    }

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
        assert(params.mode.primitive == SelectionPrimitive::VERTEX);
        if (params.isTemporary &&
            (params.mode.action == SelectionAction::ADD ||
             params.mode.action == SelectionAction::SUBTRACT)) {
            mVertexSelection.setFromCPUBuffer(mVertexSelection.cpuBackup());
        }
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
        assert(params.mode.primitive == SelectionPrimitive::FACE);
        if (params.isTemporary &&
            (params.mode.action == SelectionAction::ADD ||
             params.mode.action == SelectionAction::SUBTRACT)) {
            mFaceSelection.setFromCPUBuffer(mFaceSelection.cpuBackup());
        }
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
        assert(params.mode.primitive == SelectionPrimitive::FACE);
        if (params.isTemporary && params.mode.visible &&
            (params.mode.action == SelectionAction::ADD ||
             params.mode.action == SelectionAction::SUBTRACT)) {
            mFaceSelection.setFromCPUBuffer(mFaceSelection.cpuBackup());
        }

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
            params.textureSize[0] * params.textureSize[1]);

        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;
        bgfx::setState(state);
        SelectionUniforms::setMeshIdForSelection(params.meshId);
        SelectionUniforms::setSelectionWorkgroupSize(workGroupSize);
        SelectionUniforms::bind();
        vertPosBuf.bind(VCL_MRB_VERTEX_POSITION_STREAM);
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
        mFaceSelection.bind(
            VCL_MRB_PRIMITIVE_SELECTION_BUFFER, bgfx::Access::ReadWrite);
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

    // called on draw
    template<MeshConcept MeshType>
    bool selectionReadback(MeshType& m, const TriPolyIndexBiMap& indexMap)
    {
        bool readbackCompletedThisFrame = false;
        switch (mBufToTexRemainingFrames) {
        case 0: {
            if (mLastReadbackMode.isVertexSelection()) {
                mVertexSelection.getResultsCopy();
            }
            else if (mLastReadbackMode.isFaceSelection()) {
                mFaceSelection.getResultsCopy();
            }

            // Update CPU-side selection flags from GPU readback
            updateCPUSelectionFromGPU(m, indexMap);
            readbackCompletedThisFrame = true;

            // TODO: right now, only one additional readback can be queued. If
            // multiple selection actions are performed in a single frame.
            // This is fine now because we can select at the same time both
            // vertices and faces. If this changes, we need to implement a queue
            // of pending readbacks.
            if (mHasPendingReadback) {
                mLastReadbackMode   = mPendingReadbackMode;
                mHasPendingReadback = false;
                mBufToTexRemainingFrames =
                    requestCPUCopyOfSelectionBuffer(mLastReadbackMode);
            }
            else {
                mBufToTexRemainingFrames = UINT_NULL;
            }
        } break;
        case UINT_NULL: break;
        default: mBufToTexRemainingFrames--;
        }
        return readbackCompletedThisFrame;
    }

    bool isSelectionReadbackPending() const
    {
        return mBufToTexRemainingFrames != UINT_NULL || mHasPendingReadback;
    }

    /**
     * @brief Updates the CPU-side selection flags (BitFlags component) from the
     * GPU selection buffers that have been read back.
     *
     * This function parses the selection data from mVertexSelectionBackup and
     * mFaceSelectionBackup and updates the selected() flag on each mesh
     * element.
     *
     * @note For vertices, only the original mesh vertices are synchronized.
     * Selection state of duplicated vertices is ignored.
     */
    template<MeshConcept MeshType>
    void updateCPUSelectionFromGPU(
        MeshType&                m,
        const TriPolyIndexBiMap& indexMap)
    {
        // Update vertex selection
        if (mLastReadbackMode.isVertexSelection() &&
            !mVertexSelection.cpuBackup().empty()) {
            const auto& cpuBackup = mVertexSelection.cpuBackup();

            uint numSelectedVertices = 0;
            uint vidx                = 0;

            vcl::BitSet<uint8_t, true> flags;

            for (auto& v : m.vertices()) {
                const uint byteIdx = vidx / 8;
                if (byteIdx < cpuBackup.size()) {
                    if (vidx % 8 == 0)
                        flags.setUnderlying(cpuBackup[byteIdx]);

                    bool selected = flags[vidx % 8];
                    v.selected()  = selected;
                    if (selected)
                        ++numSelectedVertices;
                }
                ++vidx;
            }
            mNumSelectedVertices = numSelectedVertices;
        }

        // Update face selection
        if constexpr (HasFaces<MeshType>) {
            if (mLastReadbackMode.isFaceSelection() &&
                !mFaceSelection.cpuBackup().empty()) {
                const auto& cpuBackup = mFaceSelection.cpuBackup();

                const bool isTriMesh =
                    indexMap.triangleCount() == m.faceCount();

                uint numSelectedFaces = 0;
                // For each face, check if its first triangle is selected
                // (compute shaders ensure all triangles of a face have the same
                // selection state)
                vcl::BitSet<uint8_t, true> flags;
                uint                       lastByteIdx = UINT_NULL;

                for (auto& f : m.faces()) {
                    const uint faceIdx = f.index();
                    const uint firstTriIdx =
                        isTriMesh ? faceIdx : indexMap.triangleBegin(faceIdx);
                    const uint byteIdx = firstTriIdx / 8;

                    if (byteIdx < cpuBackup.size()) {
                        if (byteIdx != lastByteIdx) {
                            flags.setUnderlying(cpuBackup[byteIdx]);
                            lastByteIdx = byteIdx;
                        }

                        bool selected = flags[firstTriIdx % 8];
                        f.selected()  = selected;
                        if (selected)
                            ++numSelectedFaces;
                    }
                }
                mNumSelectedFaces = numSelectedFaces;
            }
        }
    }

    /**
     * @brief Requests an asynchronous GPU→CPU copy of the selection bitfield.
     *
     * @param[in] mode: Selection mode (determines vertex vs face buffer).
     * @return The number of frames the caller must wait before the result is
     * available via getSelectionBufferCopy().
     */
    uint requestCPUCopyOfSelectionBuffer(const SelectionMode& mode)
    {
        if (mode.isFaceSelection()) {
            mFaceSelection.requestCPUCopy();
        }
        else {
            mVertexSelection.requestCPUCopy();
        }
        return 2;
    }

    // ---- State queries --------------------------------------------------

    uint selectedVertexCount() const { return mNumSelectedVertices; }

    uint selectedFaceCount() const { return mNumSelectedFaces; }

    bool hasVertexSelectionBuffer() const { return mVertexSelection.isValid(); }

    bool hasFaceSelectionBuffer() const { return mFaceSelection.isValid(); }

    const BooleanBuffer& vertexSelectionBuffer() const
    {
        return mVertexSelection;
    }

    // ---- Bind -----------------------------------------------------------

    void bindSelectedVerticesBuffer() const { mVertexSelection.bind(4); }

    void bindSelectedFacesBuffer() const
    {
        mFaceSelection.bind(VCL_MRB_PRIMITIVE_SELECTION_BUFFER);
    }

private:
    /**
     * @brief Clears the vertex selection bitfield by dispatching a NONE action.
     *
     * @param[in] drawViewId: View ID for the dispatch.
     */
    void clearVertexSelection(uint drawViewId)
    {
        SelectionParameters params;
        params.drawViewId  = drawViewId;
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
        params.drawViewId  = drawViewId;
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
        SelectionUniforms::setNumPrimitivesForSelection(
            mVertexSelection.size());
        SelectionUniforms::bind();
        mVertexSelection.bind(4, bgfx::Access::ReadWrite);
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
            mVertexSelectionAtomicWorkgroupSize);
        SelectionUniforms::setNumPrimitivesForSelection(
            mVertexSelection.bufferSize());
        SelectionUniforms::bind();
        mVertexSelection.bind(4, bgfx::Access::ReadWrite);
        dispatchAtomicSelection(
            params.drawViewId, prog, mVertexSelectionAtomicWorkgroupSize);
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
        SelectionUniforms::setNumPrimitivesForSelection(mFaceSelection.size());
        SelectionUniforms::bind();
        mFaceSelection.bind(
            VCL_MRB_PRIMITIVE_SELECTION_BUFFER, bgfx::Access::ReadWrite);
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
            mFaceSelectionAtomicWorkgroupSize);
        SelectionUniforms::setNumPrimitivesForSelection(
            mFaceSelection.bufferSize());
        SelectionUniforms::bind();
        mFaceSelection.bind(4, bgfx::Access::ReadWrite);
        dispatchAtomicSelection(
            params.drawViewId, prog, mFaceSelectionAtomicWorkgroupSize);
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

    void dispatchAtomicSelection(
        const uint                 viewId,
        const bgfx::ProgramHandle& handle,
        const std::array<uint, 3>& workGroupSize)
    {
        bgfx::dispatch(
            viewId,
            handle,
            workGroupSize[0],
            workGroupSize[1],
            workGroupSize[2]);
        // ceil(numPrimitives / 32)
        // uint32_t(std::ceil(double(numPrimitives)/sizeof(uint))));
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

        // cut the size considering the compute thread counts
        size = static_cast<uint>(
            std::ceil(double(size) / VCL_COMPUTE_THREAD_COUNT_X));
        // At the moment we assume Y and Z compute thread count == 1

        const uint dim = uint(std::ceil(std::cbrt(double(size))));

        sizes[0] = std::min(dim, MAX_COMPUTE_WORKGROUP_SIZE);

        // Cap at sizes[0] to keep the grid roughly cubic rather than letting
        // one dimension grow much larger than the others.
        const uint remainingWork = uint(std::ceil(double(size) / sizes[0]));
        sizes[1]                 = std::min(remainingWork, sizes[0]);
        sizes[2] = uint(std::ceil(double(size) / (sizes[0] * sizes[1])));

        return sizes;
    }

    void calculateVertexSelectionWorkgroupSize()
    {
        if (mVertexSelection.size() == 0) {
            mVertexSelectionWorkgroupSize       = {0, 0, 0};
            mVertexSelectionAtomicWorkgroupSize = {0, 0, 0};
            return;
        }
        mVertexSelectionWorkgroupSize =
            workGroupSizesFrom1DSize(mVertexSelection.size());
        mVertexSelectionAtomicWorkgroupSize =
            workGroupSizesFrom1DSize(mVertexSelection.bufferSize());
    }

    void calculateFaceSelectionWorkgroupSize()
    {
        if (mFaceSelection.size() == 0) {
            mFaceSelectionWorkgroupSize       = {0, 0, 0};
            mFaceSelectionAtomicWorkgroupSize = {0, 0, 0};
            return;
        }
        mFaceSelectionWorkgroupSize =
            workGroupSizesFrom1DSize(mFaceSelection.size());
        mFaceSelectionAtomicWorkgroupSize =
            workGroupSizesFrom1DSize(mFaceSelection.bufferSize());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_SELECTION_BUFFERS_H
