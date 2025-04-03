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

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/read_framebuffer_request.h>

namespace vcl {

namespace detail {

static Point2<uint16_t> getBlitDepthSize(Point2<uint> fbSize)
{
    assert(fbSize.x() != 0 && fbSize.y() != 0);

    // get read depth size
    const auto renderType = Context::renderType();
    switch (renderType) {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        return {uint16_t(fbSize.x()), uint16_t(fbSize.y())};
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::Metal: return {1, 1};
    default: assert(false && "blit depth for untested render type"); break;
    }

    return {0, 0};
}

static bgfx::TextureFormat::Enum getOffscreenColorFormat()
{
    return Context::DEFAULT_COLOR_FORMAT;
}

static bgfx::TextureFormat::Enum getOffscreenDepthFormat()
{
    const auto renderType = Context::renderType();
    switch (renderType) {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
    case bgfx::RendererType::Vulkan: return bgfx::TextureFormat::D32F;
    case bgfx::RendererType::Metal: return bgfx::TextureFormat::D32;
    default:
        assert(false && "offscreen depth untested for current render type");
    }
    return bgfx::TextureFormat::Count;
}

static const uint64_t kBlitFormat =
    0 | BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK |
    BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT |
    BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

// Read depth constructor
ReadFramebufferRequest::ReadFramebufferRequest(
    Point2i            queryDepthPoint,
    Point2<uint>       framebufferSize,
    CallbackReadBuffer callback,
    const Color&       clearColor) :
        mType(DEPTH), mPoint(queryDepthPoint), mReadCallback(callback)
{
    mBlitSize = getBlitDepthSize(framebufferSize);

    auto& ctx       = Context::instance();
    mViewOffscreenId = ctx.requestViewId();

    mOffscreenFbh = ctx.createOffscreenFramebufferAndInitView(
        mViewOffscreenId,
        framebufferSize.x(),
        framebufferSize.y(),
        true,
        clearColor.rgba(),
        Context::DEFAULT_CLEAR_DEPTH,
        Context::DEFAULT_CLEAR_STENCIL,
        getOffscreenColorFormat(),
        getOffscreenDepthFormat());
    assert(bgfx::isValid(mOffscreenFbh));

    // create the blit depth texture
    mBlitTexture = bgfx::createTexture2D(
        uint16_t(mBlitSize.x()),
        uint16_t(mBlitSize.y()),
        false,
        1,
        getOffscreenDepthFormat(),
        kBlitFormat);
    assert(bgfx::isValid(mBlitTexture));
}

// Read color constructor
ReadFramebufferRequest::ReadFramebufferRequest(
    Point2<uint>       framebufferSize,
    CallbackReadBuffer callback,
    const Color& clearColor) : mType(COLOR), mPoint(0, 0), mReadCallback(callback)
{
    // check framebuffer size
    assert(framebufferSize.x() != 0 && framebufferSize.y() != 0);
    // get texture size capabilitites
    const auto maxSize = bgfx::getCaps()->limits.maxTextureSize;
    if (framebufferSize.x() > maxSize || framebufferSize.y() > maxSize) {
        std::cerr << "Framebuffer size " << "(" << framebufferSize.x() << "x"
                    << framebufferSize.y() << ")"
                    << "exceeds the maximum texture size "
                    << maxSize << "x" << maxSize << std::endl;

        const bool isWidthMax  = framebufferSize.x() > framebufferSize.y();
        const auto maxSizeSide =
            isWidthMax ? framebufferSize.x() : framebufferSize.y();

        // adapt the size to the maximum texture size
        const double ratio = double(maxSize) / double(maxSizeSide);
        framebufferSize = Point2<uint>(
            isWidthMax ? maxSize : uint(maxSize * ratio),
            isWidthMax ? uint(maxSize * ratio) : maxSize);

        std::cerr << "Setting size to " << "(" << framebufferSize.x() << "x"
                  << framebufferSize.y() << ")" << std::endl;
    }

    mBlitSize = framebufferSize.cast<uint16_t>();

    auto& ctx       = Context::instance();
    mViewOffscreenId = ctx.requestViewId();

    mOffscreenFbh = ctx.createOffscreenFramebufferAndInitView(
        mViewOffscreenId,
        framebufferSize.x(),
        framebufferSize.y(),
        true,
        clearColor.rgba());
    assert(bgfx::isValid(mOffscreenFbh));

    // create the blit depth texture
    mBlitTexture = bgfx::createTexture2D(
        uint16_t(mBlitSize.x()),
        uint16_t(mBlitSize.y()),
        false,
        1,
        getOffscreenColorFormat(),
        kBlitFormat);
    assert(bgfx::isValid(mBlitTexture));
}

static uint32_t kNullId = UINT_NULL;

// Read ID constructor
ReadFramebufferRequest::ReadFramebufferRequest(
    Point2i            queryIdPoint,
    Point2<uint>       framebufferSize,
    bool               idAsColor, // TODO: implement (now it's always true)
    CallbackReadBuffer callback)
    : mType(ID)
    , mPoint(queryIdPoint)
    , mReadCallback(callback)
{
    mBlitSize = framebufferSize.cast<uint16_t>();

    auto& ctx       = Context::instance();
    mViewOffscreenId = ctx.requestViewId();

    const Color clearColor(kNullId, Color::Format::RGBA);

    mOffscreenFbh = ctx.createOffscreenFramebufferAndInitView(
        mViewOffscreenId,
        framebufferSize.x(),
        framebufferSize.y(),
        true,
        clearColor.abgr());
    assert(bgfx::isValid(mOffscreenFbh));

    // read id as color
    // create the blit color texture
    mBlitTexture = bgfx::createTexture2D(
        uint16_t(mBlitSize.x()),
        uint16_t(mBlitSize.y()),
        false,
        1,
        getOffscreenColorFormat(),
        kBlitFormat);
    assert(bgfx::isValid(mBlitTexture));
}

ReadFramebufferRequest::~ReadFramebufferRequest()
{
    if (bgfx::isValid(mBlitTexture))
        bgfx::destroy(mBlitTexture);

    if (bgfx::isValid(mOffscreenFbh))
        bgfx::destroy(mOffscreenFbh);

    if (mViewOffscreenId != 0) {
        auto& ctx = Context::instance();
        ctx.releaseViewId(mViewOffscreenId);
    }
}

ReadFramebufferRequest::Type ReadFramebufferRequest::type() const
{
    return mType;
}

bgfx::ViewId ReadFramebufferRequest::viewId() const
{
    return mViewOffscreenId;
}

bgfx::FrameBufferHandle ReadFramebufferRequest::frameBuffer() const
{
    return mOffscreenFbh;
}

bool ReadFramebufferRequest::isSubmitted() const
{
    return mSubmitted;
}

bool ReadFramebufferRequest::submit()
{
    if (mSubmitted)
        return false;

    mSubmitted = true;

    // pixel size
    const auto readPixelSize = mBlitSize.x() * mBlitSize.y();

    // source buffer
    uint8_t attachment = mType == ID ? uint8_t(0) : uint8_t(mType);
    const auto srcBuffer = bgfx::getTexture(mOffscreenFbh, attachment);

    switch (mType) {
    case DEPTH: {
        // allocate memory for blit depth data
        mReadData = FloatData(readPixelSize);
        if (readPixelSize == 1) {
            // read a single fragment
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
            // read the entire depth buffer
            bgfx::blit(mViewOffscreenId, mBlitTexture, 0, 0, srcBuffer);
        }
        // submit read from blit CPU texture
        mFrameAvailable = bgfx::readTexture(
            mBlitTexture, std::get<FloatData>(mReadData).data());
    } break;
    case COLOR:
    case ID:{
        // allocate memory for blit color data
        mReadData = ByteData(readPixelSize * 4);

        // read the entire depth buffer
        bgfx::blit(mViewOffscreenId, mBlitTexture, 0, 0, srcBuffer);

        mFrameAvailable = bgfx::readTexture(
            mBlitTexture,
            std::get<ByteData>(mReadData).data());
    } break;
    default: assert(false && "unsupported readback type"); return false;
    }

    return true;
}

bool ReadFramebufferRequest::isAvailable(uint32_t currentFrame) const
{
    return mFrameAvailable != 0 && currentFrame >= mFrameAvailable;
}

bool ReadFramebufferRequest::performRead(uint32_t currFrame) const
{
    if (!isAvailable(currFrame))
        return false;

    switch (mType) {
    case DEPTH: {
        assert(std::holds_alternative<FloatData>(mReadData));
        const auto& data = std::get<FloatData>(mReadData);
        if (data.size() == 1)
            this->mReadCallback(mReadData);
        else {
            this->mReadCallback(
                FloatData({data[mPoint.y() * mBlitSize.x() + mPoint.x()]}));
        }
    } break;
    case COLOR: {
        assert(std::holds_alternative<ByteData>(mReadData));
        this->mReadCallback(mReadData);
    } break;
    case ID: {
        assert(std::holds_alternative<ByteData>(mReadData));
        
        const auto& data = std::get<ByteData>(mReadData);
        if (data.size() == 4)
            this->mReadCallback(mReadData);
        else {
            ByteData idData(4);
            std::copy_n(
                data.begin() + (mPoint.y() * mBlitSize.x() + mPoint.x()) * 4,
                4,
                idData.begin());
            this->mReadCallback(idData);
        }
    } break;
    default: assert(false && "unsupported readback type");
    }
    return true;
}

} // namespace detail

} // namespace vcl
