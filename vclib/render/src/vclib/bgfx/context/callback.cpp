// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/context/callback.h>

#include <vclib/io/image/save.h>
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
    const char*               filePath,
    uint32_t                  width,
    uint32_t                  height,
    uint32_t                  pitch,
    bgfx::TextureFormat::Enum format,
    const void*               data,
    uint32_t                  size,
    bool                      yflip)
{
    vcl::Color::Format colorFormat = vcl::Color::Format::BGRA;
    switch (format) {
    case bgfx::TextureFormat::BGRA8: break;
    case bgfx::TextureFormat::RGBA8:
        colorFormat = vcl::Color::Format::ARGB;
        break;
    default:
        std::cerr << "Unsupported texture format for screenshot: " << std::endl;
        return;
    }

    vcl::Image img(data, width, height, yflip, colorFormat);
    vcl::saveImage(img, filePath);
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
