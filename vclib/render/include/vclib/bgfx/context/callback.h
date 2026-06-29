// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_CONTEXT_CALLBACK_H
#define VCL_BGFX_CONTEXT_CALLBACK_H

#include <bgfx/bgfx.h>

namespace vcl {

class Callback : public bgfx::CallbackI
{
    bool mDebugVerbose = true;

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
        const char*               filePath,
        uint32_t                  width,
        uint32_t                  height,
        uint32_t                  pitch,
        bgfx::TextureFormat::Enum format,
        const void*               data,
        uint32_t                  size,
        bool                      yflip);

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

#endif // VCL_BGFX_CONTEXT_CALLBACK_H
