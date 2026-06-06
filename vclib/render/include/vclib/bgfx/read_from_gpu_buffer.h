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

#ifndef VCL_BGFX_READ_FROM_GPU_BUFFER_H
#define VCL_BGFX_READ_FROM_GPU_BUFFER_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/render/read_buffer_types.h>
#include <vclib/space/core.h>

#include <bgfx/bgfx.h>

#include <array>
#include <vector>

namespace vcl::detail {

/**
 * @brief Handles GPU-to-CPU data readback for both framebuffers and compute
 * buffers in a unified, reusable interface.
 *
 * Two orthogonal enums describe each instance:
 * - Source: where the data originates (FRAMEBUFFER or COMPUTE_BUFFER).
 * - Target: what kind of data is read (COLOR, DEPTH, ID, or RAW).
 *
 * For FRAMEBUFFER reads (Target = COLOR, DEPTH, or ID), construction
 * pre-allocates an offscreen framebuffer and a CPU-readable blit texture.
 * Reads are submitted in two phases:
 *   1. setPendingRead(): stores per-request parameters (point, callback) and
 *      marks the object as pending. The caller must then render to the
 *      offscreen framebuffer (using viewId() and frameBuffer()) before
 *      proceeding to the next step.
 *   2. submit(): performs the blit from the offscreen FBH to the blit texture
 *      and schedules bgfx readback. Must be called after the offscreen frame
 *      has been rendered.
 *
 * For COMPUTE_BUFFER reads (Target = RAW), construction pre-allocates
 * intermediate GPU and CPU blit textures. A single submit() call dispatches
 * the compute shader, blits the result to the CPU texture, and schedules
 * bgfx readback.
 *
 * In both cases, performRead() delivers results via the stored callback once
 * the GPU data is available, and resets the object so it can be submitted
 * again.
 */
class ReadFromGPUBuffer
{
public:
    enum class Source : uint8_t { FRAMEBUFFER, COMPUTE_BUFFER };
    enum class Target : uint8_t {
        COLOR = 0, ///< Full RGBA8 color buffer (FRAMEBUFFER only)
        DEPTH = 1, ///< Depth value at a single pixel (FRAMEBUFFER only)
        ID  = 2, ///< RGBA8 color interpreted as an object ID (FRAMEBUFFER only)
        RAW = 3, ///< Raw bytes from a compute buffer (COMPUTE_BUFFER only)
    };

private:
    Source mSource = Source::FRAMEBUFFER;
    Target mTarget = Target::COLOR;

    bool     mPending        = false;
    bool     mSubmitted      = false;
    uint32_t mFrameAvailable = 0;

    ReadBufferTypes::ReadData           mReadData     = {};
    ReadBufferTypes::CallbackReadBuffer mReadCallback = nullptr;
    Point2i                             mPoint        = {0, 0};

    // -------------------------------------------------------------------------
    // FRAMEBUFFER-specific state
    // -------------------------------------------------------------------------
    FrameBuffer         mOffscreenFbh;
    bgfx::ViewId        mViewOffscreenId = BGFX_INVALID_VIEW;
    bgfx::TextureHandle mBlitTexture     = BGFX_INVALID_HANDLE;
    Point2<uint16_t>    mBlitSize        = {0, 0};
    Color               mClearColor      = Color::Black;

    // -------------------------------------------------------------------------
    // COMPUTE_BUFFER-specific state
    // -------------------------------------------------------------------------
    bgfx::TextureHandle  mGPUTexHandle     = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle  mCPUTexHandle     = BGFX_INVALID_HANDLE;
    std::array<uint, 2>  mComputeTexSize   = {0, 0};
    uint                 mLastCopyByteSize = 0;
    std::vector<uint8_t> mReadResults;
    Uniform mBufferToTexUnif; // only initialized for COMPUTE_BUFFER

    inline static const uint COMPUTE_MAX_WGP_SIZE = 1024;

public:
    using FloatData          = ReadBufferTypes::FloatData;
    using ByteData           = ReadBufferTypes::ByteData;
    using ReadData           = ReadBufferTypes::ReadData;
    using CallbackReadBuffer = ReadBufferTypes::CallbackReadBuffer;

    /**
     * @brief Default constructor — creates an invalid, empty instance.
     * No GPU resources are allocated.
     */
    ReadFromGPUBuffer() = default;

    /**
     * @brief FRAMEBUFFER constructor. Target must be COLOR, DEPTH, or ID.
     *
     * Pre-allocates an offscreen framebuffer (with both color and depth
     * attachments) and a CPU-readable blit texture sized for @p size.
     * The blit texture format and size are chosen based on @p target and the
     * active bgfx backend.
     *
     * @param[in] target: The kind of data to read back.
     * @param[in] size: The framebuffer dimensions to allocate for.
     * @param[in] clearColor: Clear color used when rendering to the offscreen
     * FBH.
     */
    ReadFromGPUBuffer(
        Target       target,
        Point2<uint> size,
        const Color& clearColor = Color::Black);

    /**
     * @brief COMPUTE_BUFFER constructor. Source is COMPUTE_BUFFER, Target is
     * RAW.
     *
     * Pre-allocates a GPU-writable intermediate texture and a CPU-readable
     * blit texture large enough to hold @p maxByteSize bytes of data.
     *
     * @param[in] maxByteSize: The maximum number of bytes that can be read in a
     * single submit() call.
     */
    explicit ReadFromGPUBuffer(uint maxByteSize);

    // Move-only (GPU resources cannot be copied)
    ReadFromGPUBuffer(const ReadFromGPUBuffer&)            = delete;
    ReadFromGPUBuffer& operator=(const ReadFromGPUBuffer&) = delete;

    ReadFromGPUBuffer(ReadFromGPUBuffer&& other) noexcept;
    ReadFromGPUBuffer& operator=(ReadFromGPUBuffer&& other) noexcept;

    ~ReadFromGPUBuffer();

    void swap(ReadFromGPUBuffer& other) noexcept;

    friend void swap(ReadFromGPUBuffer& a, ReadFromGPUBuffer& b) noexcept
    {
        a.swap(b);
    }

    // -------------------------------------------------------------------------
    // Type and validity queries
    // -------------------------------------------------------------------------

    /**
     * @brief Returns true if GPU resources have been allocated (i.e., a
     * non-default constructor was used and the object has not been moved-from).
     */
    bool isValid() const;

    Source source() const;
    Target target() const;

    // -------------------------------------------------------------------------
    // FRAMEBUFFER-only accessors (for canvas offscreen rendering setup)
    // -------------------------------------------------------------------------

    /**
     * @brief The bgfx view ID associated with the offscreen framebuffer.
     * Only valid for FRAMEBUFFER instances.
     */
    bgfx::ViewId viewId() const;

    /**
     * @brief Handle to the offscreen framebuffer.
     * Only valid for FRAMEBUFFER instances.
     */
    bgfx::FrameBufferHandle frameBuffer() const;

    // -------------------------------------------------------------------------
    // State queries
    // -------------------------------------------------------------------------

    /**
     * @brief Returns true if setPendingRead() has been called but submit() has
     * not yet been called. Only meaningful for FRAMEBUFFER instances.
     */
    bool isPending() const;

    /**
     * @brief Returns true if submit() has been called and the GPU readback is
     * in flight (for both FRAMEBUFFER and COMPUTE_BUFFER).
     */
    bool isSubmitted() const;

    /**
     * @brief Returns true if the GPU readback result is ready for the given
     * frame counter.
     */
    bool isAvailable(uint32_t currentFrame) const;

    // -------------------------------------------------------------------------
    // FRAMEBUFFER: two-phase submission
    // -------------------------------------------------------------------------

    /**
     * @brief Phase 1 of a FRAMEBUFFER read: store per-request parameters and
     * mark the instance as pending.
     *
     * For DEPTH and ID reads @p point selects the pixel of interest.
     * For COLOR reads @p point is ignored.
     *
     * After this call the caller must render its scene to the offscreen
     * framebuffer (using viewId() and frameBuffer()) before calling submit().
     *
     * @return false if the instance is not a valid FRAMEBUFFER instance, or if
     * a read is already pending or submitted.
     */
    bool setPendingRead(Point2i point, CallbackReadBuffer callback);

    /**
     * @brief Convenience overload for COLOR reads (no query point needed).
     * @return false if the instance is not a valid FRAMEBUFFER COLOR instance,
     * or if a read is already pending or submitted.
     */
    bool setPendingRead(CallbackReadBuffer callback);

    /**
     * @brief Phase 2 of a FRAMEBUFFER read: blit from the offscreen FBH to
     * the CPU-readable texture and schedule bgfx readback.
     *
     * Must be called after the offscreen frame has been rendered.
     *
     * @return false if the instance is not pending or is not a valid
     * FRAMEBUFFER instance.
     */
    bool submit();

    // -------------------------------------------------------------------------
    // COMPUTE_BUFFER: single-phase submission
    // -------------------------------------------------------------------------

    /**
     * @brief Dispatch the buffer-to-texture compute shader, blit the result to
     * the CPU-readable texture, and schedule bgfx readback.
     *
     * @param[in] buf: The source index buffer (must be compute-capable).
     * @param[in] elementCount: Number of elements to copy.
     * @param[in] elementBitSize: Bit width of each element.
     * @param[in] callback: Called by performRead() when data is available.
     * May be nullptr if getResultsCopy() is used instead.
     * @return Number of frames the caller must advance before calling
     * performRead() or getResultsCopy(). Returns 0 on failure.
     */
    uint submit(
        const IndexBuffer& buf,
        uint               elementCount,
        uint               elementBitSize,
        CallbackReadBuffer callback);

    // -------------------------------------------------------------------------
    // Common: result delivery
    // -------------------------------------------------------------------------

    /**
     * @brief Check whether GPU data is ready; if so, invoke the stored
     * callback and reset internal state so the object can be reused.
     *
     * @param[in] currFrame: The value returned by the last bgfx::frame() call.
     * @return true when the read completed (callback has been invoked and
     * state has been reset), false if data is not yet available.
     */
    bool performRead(uint32_t currFrame);

    // -------------------------------------------------------------------------
    // COMPUTE_BUFFER: direct result access
    // -------------------------------------------------------------------------

    /**
     * @brief Returns a copy of the last COMPUTE_BUFFER readback result,
     * trimmed to the byte count requested in the last submit() call.
     *
     * Can be called instead of (or alongside) a callback. The data is
     * valid once isAvailable() returns true and remains valid until the next
     * submit() call overwrites the internal buffer.
     */
    std::vector<uint8_t> getResultsCopy() const;

private:
    // Helpers
    void destroyFramebufferResources();
    void destroyComputeResources();
    void submitFramebufferBlit();
    void performFramebufferRead() const;
    void performComputeRead() const;
    void resetSubmissionState();
};

} // namespace vcl::detail

#endif // VCL_BGFX_READ_FROM_GPU_BUFFER_H
