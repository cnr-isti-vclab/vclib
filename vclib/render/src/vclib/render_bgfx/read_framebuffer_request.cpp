/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/render_bgfx/context.h>
#include <vclib/render_bgfx/read_framebuffer_request.h>

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
    return Context::DefaultColorFormat;
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
    CallbackReadBuffer callback) :
        type(DEPTH), point(queryDepthPoint), readCallback(callback)
{
    blitSize = getBlitDepthSize(framebufferSize);

    auto& ctx       = Context::instance();
    viewOffscreenId = ctx.requestViewId();

    offscreenFbh = ctx.createOffscreenFramebufferAndInitView(
        viewOffscreenId,
        framebufferSize.x(),
        framebufferSize.y(),
        true,
        getOffscreenColorFormat(),
        getOffscreenDepthFormat());
    assert(bgfx::isValid(offscreenFbh));

    // create the blit depth texture
    blitTexture = bgfx::createTexture2D(
        uint16_t(blitSize.x()),
        uint16_t(blitSize.y()),
        false,
        1,
        getOffscreenDepthFormat(),
        kBlitFormat);
    assert(bgfx::isValid(blitTexture));
}

// read color constructor
ReadFramebufferRequest::ReadFramebufferRequest(
    Point2<uint>       framebufferSize,
    CallbackReadBuffer callback) :
        type(COLOR), point(0, 0), readCallback(callback)
{
    blitSize = framebufferSize.cast<uint16_t>();

    auto& ctx       = Context::instance();
    viewOffscreenId = ctx.requestViewId();

    offscreenFbh = ctx.createOffscreenFramebufferAndInitView(
        viewOffscreenId, framebufferSize.x(), framebufferSize.y(), true);
    assert(bgfx::isValid(offscreenFbh));

    // create the blit depth texture
    blitTexture = bgfx::createTexture2D(
        uint16_t(blitSize.x()),
        uint16_t(blitSize.y()),
        false,
        1,
        getOffscreenColorFormat(),
        kBlitFormat);
    assert(bgfx::isValid(blitTexture));
}

ReadFramebufferRequest::~ReadFramebufferRequest()
{
    if (bgfx::isValid(blitTexture))
        bgfx::destroy(blitTexture);

    if (bgfx::isValid(offscreenFbh))
        bgfx::destroy(offscreenFbh);

    if (viewOffscreenId != 0) {
        auto& ctx = Context::instance();
        ctx.releaseViewId(viewOffscreenId);
    }
}

bgfx::ViewId ReadFramebufferRequest::viewId() const
{
    return viewOffscreenId;
}

bgfx::FrameBufferHandle ReadFramebufferRequest::frameBuffer() const
{
    return offscreenFbh;
}

bool ReadFramebufferRequest::isSubmitted() const
{
    return submitted;
}

bool ReadFramebufferRequest::submit()
{
    if (submitted)
        return false;

    // pixel size
    const auto readPixelSize = blitSize.x() * blitSize.y();

    // source buffer
    const auto srcBuffer = bgfx::getTexture(offscreenFbh, uint8_t(type));

    switch (type) {
    case DEPTH: {
        // allocate memory for blit depth data
        readData = FloatData(readPixelSize);
        if (readPixelSize == 1) {
            // read a single fragment
            bgfx::blit(
                viewOffscreenId,
                blitTexture,
                0,
                0,
                srcBuffer,
                uint16_t(point.x()),
                uint16_t(point.y()),
                1,
                1);
        }
        else {
            // read the entire depth buffer
            bgfx::blit(viewOffscreenId, blitTexture, 0, 0, srcBuffer);
        }
        // submit read from blit CPU texture
        frameAvailable = bgfx::readTexture(
            blitTexture, std::get<FloatData>(readData).data());
    } break;
    case COLOR: {
        // allocate memory for blit color data
        readData = ByteData(readPixelSize * 4);

        // read the entire depth buffer
        bgfx::blit(viewOffscreenId, blitTexture, 0, 0, srcBuffer);

        frameAvailable =
            bgfx::readTexture(blitTexture, std::get<ByteData>(readData).data());
    } break;
    default: assert(false && "unsupported readback type"); return false;
    }

    submitted = true;
    return true;
}

bool ReadFramebufferRequest::isAvailable(uint32_t currentFrame) const
{
    return frameAvailable != 0 && currentFrame >= frameAvailable;
}

bool ReadFramebufferRequest::performRead(uint32_t currFrame) const
{
    if (!isAvailable(currFrame))
        return false;

    switch (type) {
    case DEPTH: {
        assert(std::holds_alternative<FloatData>(readData));
        const auto& data = std::get<FloatData>(readData);
        if (data.size() == 1)
            this->readCallback(readData);
        else {
            this->readCallback(
                FloatData({data[point.y() * blitSize.x() + point.x()]}));
        }
    } break;
    case COLOR: {
        assert(std::holds_alternative<ByteData>(readData));
        this->readCallback(readData);
    } break;
    default: assert(false && "unsupported readback type");
    }
    return true;
}

} // namespace detail

} // namespace vcl
