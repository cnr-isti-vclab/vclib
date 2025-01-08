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

#include <vclib/bgfx/context/callback.h>

#include <vclib/space/core/image.h>

#include <bx/debug.h>

#include <iostream>

namespace vcl {

namespace detail {

std::string fatalErrorString(bgfx::Fatal::Enum code)
{
    switch (code) {
    case bgfx::Fatal::DebugCheck: return "DebugCheck";
    case bgfx::Fatal::InvalidShader: return "InvalidShader";
    case bgfx::Fatal::UnableToInitialize: return "UnableToInitialize";
    case bgfx::Fatal::UnableToCreateTexture: return "UnableToCreateTexture";
    case bgfx::Fatal::DeviceLost: return "DeviceLost";
    case bgfx::Fatal::Count:
    default: return "UnrecognizedFatalError";
    }
}

} // namespace detail

void Callback::fatal(
    const char*       filePath,
    uint16_t          line,
    bgfx::Fatal::Enum code,
    const char*       str)
{
    std::cerr << "Fatal Error: " << detail::fatalErrorString(code) << " at "
              << filePath << ":" << line << " - " << str << std::endl;
    abort();
}

void Callback::traceVargs(
    const char* filePath,
    uint16_t    line,
    const char* format,
    va_list     argList)
{
    if (mDebugVerbose) {
        bx::debugPrintf("%s (%d): ", filePath, line);
        bx::debugPrintfVargs(format, argList);
    }
}

void Callback::profilerBegin(
    const char* name,
    uint32_t    abgr,
    const char* filePath,
    uint16_t    line)
{
}

void Callback::profilerBeginLiteral(
    const char* name,
    uint32_t    abgr,
    const char* filePath,
    uint16_t    line)
{
}

void Callback::profilerEnd()
{
}

uint32_t Callback::cacheReadSize(uint64_t id)
{
    return 0;
}

bool Callback::cacheRead(uint64_t id, void* data, uint32_t size)
{
    return false;
}

void Callback::cacheWrite(uint64_t id, const void* data, uint32_t size)
{
}

void Callback::screenShot(
    const char* filePath,
    uint32_t    width,
    uint32_t    height,
    uint32_t    pitch,
    const void* data,
    uint32_t    size,
    bool        yflip)
{
    vcl::Image img(data, width, height, yflip, vcl::Color::Format::ARGB);
    img.save(filePath);
}

void Callback::captureBegin(
    uint32_t                  width,
    uint32_t                  height,
    uint32_t                  pitch,
    bgfx::TextureFormat::Enum format,
    bool                      yflip)
{
}

void Callback::captureEnd()
{
}

void Callback::captureFrame(const void* data, uint32_t size)
{
}

} // namespace vcl
