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

#ifndef VCL_RENDER_CONTEXT_CALLBACK_H
#define VCL_RENDER_CONTEXT_CALLBACK_H

#include <bgfx/bgfx.h>

namespace vcl {

class Callback : public bgfx::CallbackI
{
    bool mDebugVerbose = false;

public:
    Callback() = default;

    Callback(const Callback&)            = delete;
    Callback& operator=(const Callback&) = delete;
    Callback(Callback&&)                 = delete;
    Callback& operator=(Callback&&)      = delete;

    void setDebugVerbosity(bool verbose) { mDebugVerbose = verbose; }

    // CallbackI interface
    void fatal(
        const char*       filePath,
        uint16_t          line,
        bgfx::Fatal::Enum code,
        const char*       str);

    void traceVargs(
        const char* filePath,
        uint16_t    line,
        const char* format,
        va_list     argList);

    void profilerBegin(
        const char* name,
        uint32_t    abgr,
        const char* filePath,
        uint16_t    line);

    void profilerBeginLiteral(
        const char* name,
        uint32_t    abgr,
        const char* filePath,
        uint16_t    line);

    void profilerEnd();

    uint32_t cacheReadSize(uint64_t id);

    bool cacheRead(uint64_t id, void* data, uint32_t size);

    void cacheWrite(uint64_t id, const void* data, uint32_t size);

    void screenShot(
        const char* filePath,
        uint32_t    width,
        uint32_t    height,
        uint32_t    pitch,
        const void* data,
        uint32_t    size,
        bool        yflip);

    void captureBegin(
        uint32_t                  width,
        uint32_t                  height,
        uint32_t                  pitch,
        bgfx::TextureFormat::Enum format,
        bool                      yflip);

    void captureEnd();

    void captureFrame(const void* data, uint32_t size);
};

} // namespace vcl

#endif // VCL_RENDER_CONTEXT_CALLBACK_H
