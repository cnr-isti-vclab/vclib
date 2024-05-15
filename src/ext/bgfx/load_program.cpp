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

#include <vclib/ext/bgfx/load_program.h>

#include <vclib/exceptions/io_exceptions.h>

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/readerwriter.h>

#include <bgfx/embedded_shader.h>

namespace vcl::bgf {

namespace detail {

class BXFileReader
{
    bx::DefaultAllocator allocator;
    bx::FileReaderI*     fileReader = BX_NEW(&allocator, bx::FileReader);

public:
    BXFileReader() = default;

    ~BXFileReader() { bx::deleteObject(&allocator, fileReader); }

    bx::FileReaderI* get() { return fileReader; }
};

const bgfx::Memory* loadMem(
    bx::FileReaderI*   reader,
    const std::string& filePath)
{
    if (bx::open(reader, filePath.c_str())) {
        uint32_t            size = (uint32_t) bx::getSize(reader);
        const bgfx::Memory* mem  = bgfx::alloc(size + 1);
        bx::read(reader, mem->data, size, bx::ErrorAssert {});
        bx::close(reader);
        mem->data[mem->size - 1] = '\0';
        return mem;
    }

    throw vcl::CannotOpenFileException(filePath);
    return nullptr;
}

bgfx::ShaderHandle loadShader(bx::FileReaderI* reader, std::string name)
{
    std::string shaderPath = "???";

    switch (bgfx::getRendererType()) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/"; break;
    case bgfx::RendererType::Agc:
    case bgfx::RendererType::Gnm: shaderPath = "shaders/pssl/"; break;
    case bgfx::RendererType::Metal: shaderPath = "shaders/metal/"; break;
    case bgfx::RendererType::Nvn: shaderPath = "shaders/nvn/"; break;
    case bgfx::RendererType::OpenGL: shaderPath = "shaders/glsl/"; break;
    case bgfx::RendererType::OpenGLES: shaderPath = "shaders/essl/"; break;
    case bgfx::RendererType::Vulkan: shaderPath = "shaders/spirv/"; break;

    case bgfx::RendererType::Count:
        BX_ASSERT(false, "You should not be here!");
        break;
    }

    // if name starts with "shaders/", remove it
    if (name.find("shaders/") == 0) {
        name = name.substr(8);
    }

    std::string filePath = shaderPath + name + ".bin";

    bgfx::ShaderHandle handle = bgfx::createShader(loadMem(reader, filePath));
    bgfx::setName(handle, name.c_str());

    return handle;
}

bgfx::ProgramHandle loadProgram(
    bx::FileReaderI*   reader,
    const std::string& vsName,
    const std::string& fsName)
{
    bgfx::ShaderHandle vsh = loadShader(reader, vsName);
    bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
    if (!fsName.empty()) {
        fsh = loadShader(reader, fsName);
    }

    return bgfx::createProgram(vsh, fsh, true);
}

} // namespace detail

bgfx::ShaderHandle loadShader(const std::string& name)
{
    detail::BXFileReader fr;
    return detail::loadShader(fr.get(), name);
}

bgfx::ProgramHandle loadProgram(
    const std::string& vsName,
    const std::string& fsName)
{
    detail::BXFileReader fr;
    return detail::loadProgram(fr.get(), vsName, fsName);
}

bgfx::ProgramHandle loadProgram(
    const bgfx::EmbeddedShader& vs,
    const bgfx::EmbeddedShader& fs,
    bgfx::RendererType::Enum type)
{
    auto loadShader = [](const bgfx::EmbeddedShader& s,
                         bgfx::RendererType::Enum type) -> bgfx::ShaderHandle {
        for (const bgfx::EmbeddedShader::Data* esd = s.data;
             bgfx::RendererType::Count != esd->type;
             ++esd)
        {
            if (type == esd->type && 1 < esd->size) {
                bgfx::ShaderHandle handle =
                    bgfx::createShader(bgfx::makeRef(esd->data, esd->size));
                // if (isValid(handle)) {
                //     setName(handle, _name);
                // }
                return handle;
            }
        }
        return BGFX_INVALID_HANDLE;
    };

    bgfx::ShaderHandle vsHandle = loadShader(vs, type);
    bgfx::ShaderHandle fsHandle = loadShader(fs, type);

    return bgfx::createProgram(vsHandle, fsHandle, true);
}

} // namespace vcl::bgf
