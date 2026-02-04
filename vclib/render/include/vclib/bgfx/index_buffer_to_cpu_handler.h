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

#ifndef VCL_BGFX_BUFFER_TO_CPU_HANDLER
#define VCL_BGFX_BUFFER_TO_CPU_HANDLER

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/uniform.h>

#include <bgfx/bgfx.h>

#include <bit>

// Again, i was not really able to figure out what the max size for compute workgroup size is
// except for D3D which i remeber being close to 2^14 per axis (x, y, z) (this may be device dependend though)
// Vulkan has a function that tells you how high it is
// No clue for Metal
//
// In any case, 1024^3 = 1_073_741_824 which is likely enough for these purposes
#define IDXBUF_MAX_COMPUTE_WGP_SIZE uint(1024)

namespace vcl {

// May be worth templating this to allow its use with vertex buffers as well.
// The only difference would be in the used compute shader, since (at least D3D, don't know about the other backends)
// complains in debug mode if you attempt to interpret a float buffer as an uint buffer 
// in the shader (even though it shouldn't matter at all since memory is just memory)

/**
 * @brief Class which handles the copying of IndexBuffers to CPU
 */
class IndexBufferToCpuHandler
{
    vcl::Uniform mBufferToTexUnif = vcl::Uniform(
        "u_workGroupSizeXYTexSizeXAndBufSize",
        bgfx::UniformType::Vec4);

    bgfx::TextureHandle  mGPUTexHandle = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle  mCPUTexHandle = BGFX_INVALID_HANDLE;
    std::vector<uint8_t> mReadResults;
    std::array<uint, 2>  mTextureSize      = {0, 0};
    uint                 mLastCopyByteSize = 0;

public:
    IndexBufferToCpuHandler() = default;

    /**
     * @brief Constructs an object capable of handling the copy of buffers up to a certain size
     * 
     * @param[in] requiredByteSize: The maximum byte size that the handler will be able to copy
     */
    IndexBufferToCpuHandler(uint requiredByteSize)
    {
        uint requiredTexArea = uint(ceil(double(requiredByteSize) / 4.0));
        uint maxTexSize      = bgfx::getCaps()->limits.maxTextureSize;
        mTextureSize[0]      = min(maxTexSize, requiredTexArea);
        mTextureSize[1] =
            uint(ceil(double(requiredTexArea) / double(mTextureSize[0])));
        mGPUTexHandle = bgfx::createTexture2D(
            uint16_t(mTextureSize[0]),
            uint16_t(mTextureSize[1]),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_COMPUTE_WRITE);
        mCPUTexHandle = bgfx::createTexture2D(
            uint16_t(mTextureSize[0]),
            uint16_t(mTextureSize[1]),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK);
        mReadResults = std::vector<uint8_t>(mTextureSize[0] * mTextureSize[1] * 4, 0);
    }

    // Copy not allowed.
    IndexBufferToCpuHandler(IndexBufferToCpuHandler& other)            = delete;
    IndexBufferToCpuHandler& operator=(IndexBufferToCpuHandler& other) = delete;

    void swap(IndexBufferToCpuHandler& other)
    {
        using std::swap;
        swap(mCPUTexHandle, other.mCPUTexHandle);
        swap(mGPUTexHandle, other.mGPUTexHandle);
        swap(mReadResults, other.mReadResults);
        swap(mTextureSize, other.mTextureSize);
        swap(mLastCopyByteSize, other.mLastCopyByteSize);
    }

    friend void swap(IndexBufferToCpuHandler& a, IndexBufferToCpuHandler& b)
    {
        a.swap(b);
    }

    IndexBufferToCpuHandler(IndexBufferToCpuHandler&& other) { swap(other); }

    IndexBufferToCpuHandler& operator=(IndexBufferToCpuHandler&& other)
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Attempts perform a copy of size (bufferElementCount * elementBitSize)/8 (approximated by excess) bytes from the buffer buf
     * 
     * @param[in] buf: The buffer to copy from
     * @param[in] bufferElementCount: The number of buffer elements to be copied
     * @param[in] elementBitSize: The bit size of each element
     * @return: The number of frames after which the result will be ready
     */
    uint copyFromGPU(
        const IndexBuffer& buf,
        uint               bufferElementCount,
        uint               elementBitSize)
    {
        if (!buf.isValid() || !buf.isCompute()) {
            std::cerr << "ERROR: Passed IndexBuffer is either invalid or does "
                         "not support ComputeShaders"
                      << std::endl;
            exit(1);
        }
        if (!bgfx::isValid(mGPUTexHandle) || !bgfx::isValid(mCPUTexHandle)) {
            std::cerr << "ERROR: Attempting to use uninitialized "
                         "IndexBufferToCpuHandler"
                      << std::endl;
            exit(1);
        }
        mLastCopyByteSize        = uint(ceil(double(bufferElementCount) * double(elementBitSize) / 8.0));
        vcl::Context& ctx        = vcl::Context::instance();
        bgfx::ViewId copyViewId = ctx.requestViewId();
        bgfx::ViewId  blitViewId = ctx.requestViewId();
        uint          uintBufferElementCount = uint(
            ceil((double(bufferElementCount) * double(elementBitSize)) / 32.0));
        std::array<uint, 3> workgroupSize;
        workgroupSize[0] =
            min(uintBufferElementCount, IDXBUF_MAX_COMPUTE_WGP_SIZE);
        workgroupSize[1] =
            min(uint(ceil(double(uintBufferElementCount) / double(workgroupSize[0]))),
                IDXBUF_MAX_COMPUTE_WGP_SIZE);
        workgroupSize[2]          = uint(ceil(
            double(uintBufferElementCount) /
            double(workgroupSize[0] * workgroupSize[1])));
        std::array<float, 4> temp = {
            std::bit_cast<float>(workgroupSize[0]),
            std::bit_cast<float>(workgroupSize[1]),
            std::bit_cast<float>(mTextureSize[0]),
            std::bit_cast<float>(uintBufferElementCount)};
        auto& pm = vcl::Context::instance().programManager();
        bgfx::setImage(
            4,
            mGPUTexHandle,
            0,
            bgfx::Access::Write,
            bgfx::TextureFormat::RGBA8);
        mBufferToTexUnif.bind((void*) &(temp[0]));
        buf.bind(5, bgfx::Access::Read);
        bgfx::dispatch(
            copyViewId,
            pm.getComputeProgram<vcl::ComputeProgram::BUFFER_TO_TEX>(),
            workgroupSize[0],
            workgroupSize[1],
            workgroupSize[2]);
        bgfx::blit(
            blitViewId,
            mCPUTexHandle,
            0,
            0,
            mGPUTexHandle,
            0,
            0,
            mTextureSize[0],
            mTextureSize[1]);
        ctx.releaseViewId(blitViewId);
        ctx.releaseViewId(copyViewId);
        bgfx::readTexture(mCPUTexHandle, mReadResults.data());
        return 2;
    }


    /**
     * @return: A copy of the result of the buffer copy (resized to fit the requested size)
     */
    std::vector<uint8_t> getResultsCopy() const
    {
        std::vector<uint8_t> newObj(mReadResults);
        newObj.resize(mLastCopyByteSize);
        return std::move(newObj);
    }

    ~IndexBufferToCpuHandler() {
        if (bgfx::isValid(mGPUTexHandle)) {
            bgfx::destroy(mGPUTexHandle);
        }
        if (bgfx::isValid(mCPUTexHandle)) {
            bgfx::destroy(mCPUTexHandle);
        }
    }
};
} // namespace vcl

#endif
