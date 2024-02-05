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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_CALLBACK_H
#define VCL_EXT_BGFX_CALLBACK_H

#include <bgfx/bgfx.h>

namespace vcl::bgf {

class Callback : public bgfx::CallbackI
{
public:
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

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_CALLBACK_H
