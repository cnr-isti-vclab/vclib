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

#include <vclib/bgfx/read_from_gpu_buffer.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/programs/compute_program.h>

#include <bit>
#include <cmath>
#include <iostream>

namespace vcl::detail {

// ============================================================================
// File-local helpers (mirrored from the old read_framebuffer_request.cpp)
// ============================================================================

static Point2<uint16_t> blitDepthSize(Point2<uint> fbSize)
{
    assert(fbSize.x() != 0 && fbSize.y() != 0);

    switch (Context::renderType()) {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        return {uint16_t(fbSize.x()), uint16_t(fbSize.y())};
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::Metal: return {1, 1};
    default: assert(false && "blit depth unsupported for this render type");
    }
    return {0, 0};
}

static bgfx::TextureFormat::Enum offscreenColorFormat()
{
    return bgfx::TextureFormat::RGBA8;
}

static bgfx::TextureFormat::Enum offscreenDepthFormat()
{
    switch (Context::renderType()) {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
    case bgfx::RendererType::Vulkan: return bgfx::TextureFormat::D32F;
    case bgfx::RendererType::Metal: return bgfx::TextureFormat::D32;
    default:
        assert(false && "offscreen depth unsupported for this render type");
    }
    return bgfx::TextureFormat::Count;
}

static constexpr uint64_t kBlitFlags =
    BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK | BGFX_SAMPLER_MIN_POINT |
    BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP |
    BGFX_SAMPLER_V_CLAMP;

// ============================================================================
// Constructors / destructor
// ============================================================================

ReadFromGPUBuffer::ReadFromGPUBuffer(
    Target       target,
    Point2<uint> size,
    const Color& clearColor) :
        mSource(Source::FRAMEBUFFER), mTarget(target), mClearColor(clearColor)
{
    assert(
        target != Target::RAW && "RAW target is only valid for COMPUTE_BUFFER");
    assert(size.x() != 0 && size.y() != 0);

    // Clamp size to hardware limits
    const auto maxTex = bgfx::getCaps()->limits.maxTextureSize;
    if (size.x() > maxTex || size.y() > maxTex) {
        std::cerr << "ReadFromGPUBuffer: requested size (" << size.x() << "x"
                  << size.y() << ") exceeds max texture size " << maxTex
                  << ". Clamping.\n";
        const bool   wideIsLarger = size.x() >= size.y();
        const uint   larger       = wideIsLarger ? size.x() : size.y();
        const double ratio        = double(maxTex) / double(larger);
        size                      = {
            wideIsLarger ? maxTex : uint(double(size.x()) * ratio),
            wideIsLarger ? uint(double(size.y()) * ratio) : maxTex};
    }

    // Determine blit texture size and format
    bgfx::TextureFormat::Enum blitFormat;
    if (target == Target::DEPTH) {
        mBlitSize  = blitDepthSize(size);
        blitFormat = offscreenDepthFormat();
    }
    else { // COLOR or ID
        mBlitSize  = size.cast<uint16_t>();
        blitFormat = offscreenColorFormat();
    }

    // Pre-allocate readback data buffer
    const uint pixelCount = uint(mBlitSize.x()) * uint(mBlitSize.y());
    if (target == Target::DEPTH)
        mReadData = FloatData(pixelCount);
    else
        mReadData = ByteData(pixelCount * 4);

    auto& ctx        = Context::instance();
    mViewOffscreenId = ctx.requestViewId();

    // Determine clear color (ID uses the null-ID as clear)
    const uint32_t clearValue =
        (target == Target::ID) ? Color(UINT_NULL, Color::Format::RGBA).abgr() :
                                 clearColor.rgba();

    // Create the offscreen framebuffer
    mOffscreenFbh.create(
        uint16_t(size.x()),
        uint16_t(size.y()),
        offscreenColorFormat(),
        offscreenDepthFormat());
    assert(mOffscreenFbh.isValid());

    // Initialize the view
    bgfx::setViewFrameBuffer(mViewOffscreenId, mOffscreenFbh.handle());
    bgfx::setViewClear(
        mViewOffscreenId,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL,
        clearValue,
        Context::DEFAULT_CLEAR_DEPTH,
        Context::DEFAULT_CLEAR_STENCIL);
    bgfx::setViewRect(
        mViewOffscreenId, 0, 0, uint16_t(size.x()), uint16_t(size.y()));
    bgfx::touch(mViewOffscreenId);

    mBlitTexture = bgfx::createTexture2D(
        mBlitSize.x(), mBlitSize.y(), false, 1, blitFormat, kBlitFlags);
    assert(bgfx::isValid(mBlitTexture));
}

ReadFromGPUBuffer::ReadFromGPUBuffer(uint maxByteSize) :
        mSource(Source::COMPUTE_BUFFER), mTarget(Target::RAW),
        mBufferToTexUnif(
            "u_workGroupSizeXYTexSizeXAndBufSize",
            bgfx::UniformType::Vec4)
{
    const uint requiredTexArea = uint(std::ceil(double(maxByteSize) / 4.0));
    const uint maxTex          = bgfx::getCaps()->limits.maxTextureSize;

    mComputeTexSize[0] = std::min(maxTex, requiredTexArea);
    mComputeTexSize[1] =
        uint(std::ceil(double(requiredTexArea) / double(mComputeTexSize[0])));

    mGPUTexHandle = bgfx::createTexture2D(
        uint16_t(mComputeTexSize[0]),
        uint16_t(mComputeTexSize[1]),
        false,
        1,
        bgfx::TextureFormat::RGBA8,
        BGFX_TEXTURE_COMPUTE_WRITE);
    assert(bgfx::isValid(mGPUTexHandle));

    mCPUTexHandle = bgfx::createTexture2D(
        uint16_t(mComputeTexSize[0]),
        uint16_t(mComputeTexSize[1]),
        false,
        1,
        bgfx::TextureFormat::RGBA8,
        BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK);
    assert(bgfx::isValid(mCPUTexHandle));

    mReadResults.assign(mComputeTexSize[0] * mComputeTexSize[1] * 4, 0);
}

ReadFromGPUBuffer::ReadFromGPUBuffer(ReadFromGPUBuffer&& other) noexcept
{
    swap(other);
}

ReadFromGPUBuffer& ReadFromGPUBuffer::operator=(
    ReadFromGPUBuffer&& other) noexcept
{
    swap(other);
    return *this;
}

ReadFromGPUBuffer::~ReadFromGPUBuffer()
{
    destroyFramebufferResources();
    destroyComputeResources();
}

void ReadFromGPUBuffer::swap(ReadFromGPUBuffer& other) noexcept
{
    using std::swap;
    swap(mSource, other.mSource);
    swap(mTarget, other.mTarget);
    swap(mPending, other.mPending);
    swap(mSubmitted, other.mSubmitted);
    swap(mFrameAvailable, other.mFrameAvailable);
    swap(mReadData, other.mReadData);
    swap(mReadCallback, other.mReadCallback);
    swap(mPoint, other.mPoint);
    // FRAMEBUFFER
    swap(mOffscreenFbh, other.mOffscreenFbh);
    swap(mViewOffscreenId, other.mViewOffscreenId);
    swap(mBlitTexture, other.mBlitTexture);
    swap(mBlitSize, other.mBlitSize);
    swap(mClearColor, other.mClearColor);
    // COMPUTE_BUFFER
    swap(mGPUTexHandle, other.mGPUTexHandle);
    swap(mCPUTexHandle, other.mCPUTexHandle);
    swap(mComputeTexSize, other.mComputeTexSize);
    swap(mLastCopyByteSize, other.mLastCopyByteSize);
    swap(mReadResults, other.mReadResults);
    swap(mBufferToTexUnif, other.mBufferToTexUnif);
}

// ============================================================================
// Type and validity queries
// ============================================================================

bool ReadFromGPUBuffer::isValid() const
{
    return mOffscreenFbh.isValid() || bgfx::isValid(mGPUTexHandle);
}

ReadFromGPUBuffer::Source ReadFromGPUBuffer::source() const
{
    return mSource;
}

ReadFromGPUBuffer::Target ReadFromGPUBuffer::target() const
{
    return mTarget;
}

// ============================================================================
// FRAMEBUFFER-only accessors
// ============================================================================

bgfx::ViewId ReadFromGPUBuffer::viewId() const
{
    return mViewOffscreenId;
}

bgfx::FrameBufferHandle ReadFromGPUBuffer::frameBuffer() const
{
    return mOffscreenFbh.handle();
}

// ============================================================================
// State queries
// ============================================================================

bool ReadFromGPUBuffer::isPending() const
{
    return mPending;
}

bool ReadFromGPUBuffer::isSubmitted() const
{
    return mSubmitted;
}

bool ReadFromGPUBuffer::isAvailable(uint32_t currentFrame) const
{
    return mSubmitted && mFrameAvailable != 0 &&
           currentFrame >= mFrameAvailable;
}

// ============================================================================
// FRAMEBUFFER: two-phase submission
// ============================================================================

bool ReadFromGPUBuffer::setPendingRead(
    Point2i            point,
    CallbackReadBuffer callback)
{
    if (!isValid() || mSource != Source::FRAMEBUFFER || mPending || mSubmitted)
        return false;

    mPoint        = point;
    mReadCallback = callback;
    mPending      = true;
    return true;
}

bool ReadFromGPUBuffer::setPendingRead(CallbackReadBuffer callback)
{
    return setPendingRead({0, 0}, callback);
}

bool ReadFromGPUBuffer::submit()
{
    if (!mPending || mSubmitted || mSource != Source::FRAMEBUFFER)
        return false;

    mPending   = false;
    mSubmitted = true;
    submitFramebufferBlit();
    return true;
}

// ============================================================================
// COMPUTE_BUFFER: single-phase submission
// ============================================================================

uint ReadFromGPUBuffer::submit(
    const IndexBuffer& buf,
    uint               elementCount,
    uint               elementBitSize,
    CallbackReadBuffer callback)
{
    if (mSource != Source::COMPUTE_BUFFER) {
        std::cerr << "ReadFromGPUBuffer::submit(IndexBuffer): called on a "
                     "non-COMPUTE_BUFFER instance\n";
        return 0;
    }
    if (!buf.isValid() || !buf.isCompute()) {
        std::cerr << "ReadFromGPUBuffer::submit(IndexBuffer): buffer is "
                     "invalid or not compute-capable\n";
        return 0;
    }
    if (!bgfx::isValid(mGPUTexHandle) || !bgfx::isValid(mCPUTexHandle)) {
        std::cerr << "ReadFromGPUBuffer::submit(IndexBuffer): called on an "
                     "uninitialized instance\n";
        return 0;
    }

    mReadCallback = callback;
    mLastCopyByteSize =
        uint(std::ceil(double(elementCount) * double(elementBitSize) / 8.0));

    const uint uintElementCount =
        uint(std::ceil(double(elementCount) * double(elementBitSize) / 32.0));

    std::array<uint, 3> wgpSize;
    wgpSize[0] = std::min(uintElementCount, COMPUTE_MAX_WGP_SIZE);
    wgpSize[1] = std::min(
        uint(std::ceil(double(uintElementCount) / double(wgpSize[0]))),
        COMPUTE_MAX_WGP_SIZE);
    wgpSize[2] = uint(
        std::ceil(double(uintElementCount) / double(wgpSize[0] * wgpSize[1])));

    const std::array<float, 4> unifData = {
        std::bit_cast<float>(wgpSize[0]),
        std::bit_cast<float>(wgpSize[1]),
        std::bit_cast<float>(mComputeTexSize[0]),
        std::bit_cast<float>(uintElementCount)};

    auto&        ctx          = Context::instance();
    auto&        pm           = ctx.programManager();
    bgfx::ViewId dispatchView = ctx.requestViewId();
    bgfx::ViewId blitView     = ctx.requestViewId();

    bgfx::setImage(
        4, mGPUTexHandle, 0, bgfx::Access::Write, bgfx::TextureFormat::RGBA8);
    mBufferToTexUnif.bind(unifData.data());
    buf.bind(5, bgfx::Access::Read);
    bgfx::dispatch(
        dispatchView,
        pm.getComputeProgram<vcl::ComputeProgram::BUFFER_TO_TEX>(),
        wgpSize[0],
        wgpSize[1],
        wgpSize[2]);

    bgfx::blit(
        blitView,
        mCPUTexHandle,
        0,
        0,
        mGPUTexHandle,
        0,
        0,
        uint16_t(mComputeTexSize[0]),
        uint16_t(mComputeTexSize[1]));

    ctx.releaseViewId(blitView);
    ctx.releaseViewId(dispatchView);

    mFrameAvailable = bgfx::readTexture(mCPUTexHandle, mReadResults.data());
    mSubmitted      = true;
    return 2;
}

// ============================================================================
// Common: result delivery
// ============================================================================

bool ReadFromGPUBuffer::performRead(uint32_t currFrame)
{
    if (!isAvailable(currFrame))
        return false;

    if (mReadCallback != nullptr) {
        if (mSource == Source::FRAMEBUFFER)
            performFramebufferRead();
        else
            performComputeRead();
    }

    resetSubmissionState();
    return true;
}

// ============================================================================
// COMPUTE_BUFFER: direct result access
// ============================================================================

std::vector<uint8_t> ReadFromGPUBuffer::getResultsCopy() const
{
    std::vector<uint8_t> result(mReadResults);
    result.resize(mLastCopyByteSize);
    return result;
}

// ============================================================================
// Private helpers
// ============================================================================

void ReadFromGPUBuffer::destroyFramebufferResources()
{
    if (bgfx::isValid(mBlitTexture)) {
        bgfx::destroy(mBlitTexture);
        mBlitTexture = BGFX_INVALID_HANDLE;
    }
    // FrameBuffer handles its own destruction
    mOffscreenFbh.destroy();
    if (mViewOffscreenId != BGFX_INVALID_VIEW) {
        Context::instance().releaseViewId(mViewOffscreenId);
        mViewOffscreenId = BGFX_INVALID_VIEW;
    }
}

void ReadFromGPUBuffer::destroyComputeResources()
{
    if (bgfx::isValid(mGPUTexHandle)) {
        bgfx::destroy(mGPUTexHandle);
        mGPUTexHandle = BGFX_INVALID_HANDLE;
    }
    if (bgfx::isValid(mCPUTexHandle)) {
        bgfx::destroy(mCPUTexHandle);
        mCPUTexHandle = BGFX_INVALID_HANDLE;
    }
}

void ReadFromGPUBuffer::submitFramebufferBlit()
{
    // Determine which framebuffer attachment to read from:
    // COLOR → attachment 0, DEPTH → attachment 1, ID → attachment 0
    const uint8_t attachment =
        (mTarget == Target::DEPTH) ? uint8_t(1) : uint8_t(0);
    const bgfx::TextureHandle srcBuffer =
        bgfx::getTexture(mOffscreenFbh, attachment);

    switch (mTarget) {
    case Target::DEPTH: {
        assert(std::holds_alternative<FloatData>(mReadData));
        const uint pixelCount = uint(mBlitSize.x()) * uint(mBlitSize.y());
        if (pixelCount == 1) {
            // Vulkan/Metal: blit only the queried pixel
            bgfx::blit(
                mViewOffscreenId,
                mBlitTexture,
                0,
                0,
                srcBuffer,
                uint16_t(mPoint.x()),
                uint16_t(mPoint.y()),
                1,
                1);
        }
        else {
            // D3D: blit the full depth buffer; pixel extracted in performRead
            bgfx::blit(mViewOffscreenId, mBlitTexture, 0, 0, srcBuffer);
        }
        mFrameAvailable = bgfx::readTexture(
            mBlitTexture, std::get<FloatData>(mReadData).data());
    } break;

    case Target::COLOR:
    case Target::ID: {
        assert(std::holds_alternative<ByteData>(mReadData));
        bgfx::blit(mViewOffscreenId, mBlitTexture, 0, 0, srcBuffer);
        mFrameAvailable = bgfx::readTexture(
            mBlitTexture, std::get<ByteData>(mReadData).data());
    } break;

    default: assert(false && "FRAMEBUFFER submit called with RAW target");
    }
}

void ReadFromGPUBuffer::performFramebufferRead() const
{
    switch (mTarget) {
    case Target::DEPTH: {
        assert(std::holds_alternative<FloatData>(mReadData));
        const auto& data = std::get<FloatData>(mReadData);
        if (data.size() == 1) {
            mReadCallback(mReadData);
        }
        else {
            mReadCallback(FloatData(
                {data[uint(mPoint.y()) * mBlitSize.x() + uint(mPoint.x())]}));
        }
    } break;

    case Target::COLOR: {
        assert(std::holds_alternative<ByteData>(mReadData));
        mReadCallback(mReadData);
    } break;

    case Target::ID: {
        assert(std::holds_alternative<ByteData>(mReadData));
        const auto& data = std::get<ByteData>(mReadData);
        if (data.size() == 4) {
            mReadCallback(mReadData);
        }
        else {
            ByteData   idPixel(4);
            const auto offset =
                (uint(mPoint.y()) * mBlitSize.x() + uint(mPoint.x())) * 4;
            std::copy_n(data.begin() + offset, 4, idPixel.begin());
            mReadCallback(idPixel);
        }
    } break;

    default: assert(false && "performFramebufferRead called with RAW target");
    }
}

void ReadFromGPUBuffer::performComputeRead() const
{
    assert(mSource == Source::COMPUTE_BUFFER);
    ByteData result(
        mReadResults.begin(), mReadResults.begin() + mLastCopyByteSize);
    mReadCallback(result);
}

void ReadFromGPUBuffer::resetSubmissionState()
{
    mPending        = false;
    mSubmitted      = false;
    mFrameAvailable = 0;
    mReadCallback   = nullptr;
    mPoint          = {0, 0};
}

} // namespace vcl::detail
