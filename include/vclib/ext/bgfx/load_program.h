/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_EXT_BGFX_LOAD_PROGRAM_H
#define VCL_EXT_BGFX_LOAD_PROGRAM_H

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bx/file.h>
#include <bx/readerwriter.h>

#include <iostream>

#ifndef VCLIB_RELATIVE_SHADERS_PATH
#define VCLIB_RELATIVE_SHADERS_PATH ""
#endif

namespace vcl::bgf {

namespace detail {

class BXFileReader
{
    bx::DefaultAllocator allocator;
    bx::FileReaderI* fileReader = BX_NEW(&allocator, bx::FileReader);

public:
    BXFileReader() = default;

    ~BXFileReader() {
        bx::deleteObject(&allocator, fileReader);
    }

    bx::FileReaderI* get() {  return fileReader; }
};

inline const bgfx::Memory* loadMem(
    bx::FileReaderI* reader,
    const std::string& filePath)
{
    if (bx::open(reader, filePath.c_str())) {
        uint32_t size = (uint32_t)bx::getSize(reader);
        const bgfx::Memory* mem = bgfx::alloc(size+1);
        bx::read(reader, mem->data, size, bx::ErrorAssert{});
        bx::close(reader);
        mem->data[mem->size-1] = '\0';
        return mem;
    }

    std::cerr << "Failed to load " << filePath << "\n";
    return nullptr;
}

inline bgfx::ShaderHandle loadShader(
    bx::FileReaderI* reader,
    const std::string& name,
    const std::string& basePath)
{
    std::string shaderPath = "???";

    switch (bgfx::getRendererType())
    {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
    case bgfx::RendererType::Agc:
    case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
    case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
    case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
    case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
    case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
    case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
    case bgfx::RendererType::WebGPU:     shaderPath = "shaders/spirv/"; break;

    case bgfx::RendererType::Count:
        BX_ASSERT(false, "You should not be here!");
        break;
    }

    std::string filePath = basePath + shaderPath + name + ".bin";

    bgfx::ShaderHandle handle = bgfx::createShader(loadMem(reader, filePath));
    bgfx::setName(handle, name.c_str());

    return handle;
}

inline bgfx::ProgramHandle loadProgram(
    bx::FileReaderI* reader,
    const std::string& vsName,
    const std::string& fsName,
    const std::string& basePath)
{
    bgfx::ShaderHandle vsh = loadShader(reader, vsName, basePath);
    bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
    if (!fsName.empty()) {
        fsh = loadShader(reader, fsName, basePath);
    }

    return bgfx::createProgram(vsh, fsh, true);
}

} // namespace vcl::bgf::detail

inline bgfx::ShaderHandle loadShader(
    const std::string& name,
    const std::string& basePath = "")
{
    detail::BXFileReader fr;
    return detail::loadShader(fr.get(), name, basePath);
}

inline bgfx::ProgramHandle loadProgram(
    const std::string& vsName,
    const std::string& fsName,
    const std::string& basePath = "")
{
    detail::BXFileReader fr;
    return detail::loadProgram(fr.get(), vsName, fsName, basePath);
}

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_LOAD_PROGRAM_H
