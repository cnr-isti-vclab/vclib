// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_BUFFERS_BOOLEAN_BUFFER_H
#define VCL_BGFX_BUFFERS_BOOLEAN_BUFFER_H

#include <vclib/bgfx/buffers/index_buffer.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/read_from_gpu_buffer.h>

#include <cmath>
#include <vector>

namespace vcl {

/**
 * @brief Manages a GPU buffer of booleans.
 *
 * This class encapsulates a bitfield buffer stored as uint32 words on the GPU,
 * allowing compute shaders to read and write booleans atomically.
 * It also provides methods for synchronizing with a CPU backup vector.
 */
class BooleanBuffer
{
    IndexBuffer               mGpuBuffer;
    std::vector<uint8_t>      mBackup;
    detail::ReadFromGPUBuffer mReadbackHandler;

    uint mNumElements = 0;
    uint mNumUints    = 0;

public:
    BooleanBuffer() = default;

    void swap(BooleanBuffer& other)
    {
        using std::swap;
        swap(mGpuBuffer, other.mGpuBuffer);
        swap(mBackup, other.mBackup);
        swap(mReadbackHandler, other.mReadbackHandler);
        swap(mNumElements, other.mNumElements);
        swap(mNumUints, other.mNumUints);
    }

    friend void swap(BooleanBuffer& a, BooleanBuffer& b) { a.swap(b); }

    /**
     * @brief Allocates the boolean buffer on the GPU.
     *
     * The bitfield has one bit per element, packed into uint32 words.
     * All bits are initialised to zero.
     *
     * @param[in] elementCount: Number of booleans to store.
     */
    void init(uint elementCount)
    {
        mNumElements = elementCount;

        if (elementCount == 0) {
            mGpuBuffer.destroy();
            mNumUints = 0;
            return;
        }

        mNumUints = uint(std::ceil(double(elementCount) / 32.0));

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(mNumUints);

        // clear the buffer with zeroes
        std::fill(buffer, buffer + mNumUints, 0);

        mGpuBuffer.create(
            buffer,
            mNumUints,
            vcl::PrimitiveType::UINT,
            bgfx::Access::ReadWrite,
            releaseFn);

        // Resize readback handler. We need bytes, so ceil(elementCount / 8.0)
        uint bytes       = uint(std::ceil(double(elementCount) / 8.0));
        mReadbackHandler = std::move(detail::ReadFromGPUBuffer(bytes));
    }

    /**
     * @brief Uploads a CPU bitfield backup to the GPU buffer.
     *
     * @param[in] backup: The CPU bitfield packed in a vector of bytes.
     */
    void setFromCPUBuffer(const std::vector<uint8_t>& backup)
    {
        mBackup = backup;

        if (backup.empty() || !mGpuBuffer.isValid())
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

        mGpuBuffer.destroy();
        mGpuBuffer.create(
            buffer,
            elementCount,
            vcl::PrimitiveType::UINT,
            bgfx::Access::ReadWrite,
            releaseFn);
    }

    /**
     * @brief Requests an asynchronous GPU->CPU copy of the boolean bitfield.
     */
    void requestCPUCopy()
    {
        if (mNumElements > 0 && mGpuBuffer.isValid()) {
            constexpr uint elementBitSize = 1;
            mReadbackHandler.submit(
                mGpuBuffer, mNumElements, elementBitSize, nullptr);
        }
    }

    /**
     * @brief Retrieves the readback result, updates the local backup and
     * returns it.
     */
    std::vector<uint8_t> getResultsCopy()
    {
        mBackup = mReadbackHandler.getResultsCopy();
        return mBackup;
    }

    const std::vector<uint8_t>& cpuBackup() const { return mBackup; }

    std::vector<uint8_t>& cpuBackup() { return mBackup; }

    void bind(uint stage, bgfx::Access::Enum access = bgfx::Access::Read) const
    {
        mGpuBuffer.bind(stage, access);
    }

    uint size() const { return mNumElements; }

    uint bufferSize() const { return mNumUints; }

    bool isValid() const { return mGpuBuffer.isValid(); }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_BOOLEAN_BUFFER_H
