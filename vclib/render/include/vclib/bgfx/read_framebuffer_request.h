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

#ifndef VCL_BGFX_READ_FRAMEBUFFER_REQUEST_H
#define VCL_BGFX_READ_FRAMEBUFFER_REQUEST_H

#include <bgfx/bgfx.h>
#include <vclib/render/read_buffer_types.h>
#include <vclib/space/core/color.h>
#include <vclib/space/core/point.h>

namespace vcl {

namespace detail {

class ReadFramebufferRequest
{
public:
    using FloatData          = ReadBufferTypes::FloatData;
    using ByteData           = ReadBufferTypes::ByteData;
    using ReadData           = ReadBufferTypes::ReadData;
    using CallbackReadBuffer = ReadBufferTypes::CallbackReadBuffer;

    // Read depth constructor
    ReadFramebufferRequest(
        Point2i            queryDepthPoint,
        Point2<uint>       framebufferSize,
        CallbackReadBuffer callback,
        const Color&       clearColor = Color::Black);

    // read color constructor
    ReadFramebufferRequest(
        Point2<uint>       framebufferSize,
        CallbackReadBuffer callback,
        const Color&       clearColor = Color::Black);

    ~ReadFramebufferRequest();

    ReadFramebufferRequest& operator=(ReadFramebufferRequest&& right) = default;

    bgfx::ViewId viewId() const;

    bgfx::FrameBufferHandle frameBuffer() const;

    bool submit();

    bool isSubmitted() const;

    bool isAvailable(uint32_t currentFrame) const;

    [[nodiscard]] bool performRead(uint32_t currFrame) const;

private:
    enum Type {
        COLOR = 0, // entire color buffer
        DEPTH = 1, // single pixel depth
        COUNT = 2
    };

    // read back type
    Type type = COUNT;

    // frame # when data will be available for reading
    uint32_t frameAvailable = 0;
    // point to read from
    Point2i point = {-1, -1};

    // frame buffer for offscreen drawing and reading back
    bgfx::FrameBufferHandle offscreenFbh = BGFX_INVALID_HANDLE;
    // view id for offscreen drawing
    bgfx::ViewId viewOffscreenId = 0;

    // blit texture
    bgfx::TextureHandle blitTexture = BGFX_INVALID_HANDLE;
    Point2<uint16_t>    blitSize    = {0, 0};
    // data read from the blit texture
    ReadData readData = {};
    // callback called when the data is available
    CallbackReadBuffer readCallback = nullptr;
    // submitted flag
    bool submitted = false;
};

} // namespace detail

} // namespace vcl

#endif // VCL_BGFX_READ_FRAMEBUFFER_REQUEST_H
