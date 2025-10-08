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

#define IDXBUF_MAX_COMPUTE_WGP_SIZE uint(1024)

namespace vcl {
class IndexBufferToCpuHandler
{
    vcl::Uniform mBufferToTexUnif = vcl::Uniform(
        "u_workGroupSizeXYTexSizeAndBufSize",
        bgfx::UniformType::Vec4);

    bgfx::TextureHandle  mGPUTexHandle = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle  mCPUTexHandle = BGFX_INVALID_HANDLE;
    std::vector<uint8_t> mReadResults;
    std::array<uint, 2>  mTextureSize      = {0, 0};
    uint                 mLastCopyByteSize = 0;

public:
    IndexBufferToCpuHandler() = default;

    IndexBufferToCpuHandler(uint requiredByteSize) :
            mReadResults(requiredByteSize, 0)
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
        bgfx::ViewId  copyViewId = ctx.requestViewId();
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
            Uniform::uintBitsToFloat(workgroupSize[0]),
            Uniform::uintBitsToFloat(workgroupSize[1]),
            Uniform::uintBitsToFloat(mTextureSize[0]),
            Uniform::uintBitsToFloat(uintBufferElementCount)};
        auto& pm = vcl::Context::instance().programManager();
        mBufferToTexUnif.bind(temp.data());
        buf.bind(5, bgfx::Access::Read);
        bgfx::setImage(
            4,
            mGPUTexHandle,
            0,
            bgfx::Access::Write,
            bgfx::TextureFormat::RGBA8);
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
        ctx.releaseViewId(copyViewId);
        ctx.releaseViewId(blitViewId);
        bgfx::readTexture(mCPUTexHandle, mReadResults.data());
        return 2;
    }

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
