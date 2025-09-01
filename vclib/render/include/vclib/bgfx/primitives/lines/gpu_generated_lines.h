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

#ifndef VCL_BGFX_PRIMITIVES_LINES_GPU_GENERATED_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_GPU_GENERATED_LINES_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>

#include <vclib/bgfx/uniform.h>

#include <vclib/bgfx/primitives/lines/lines_utils.h>

namespace vcl::detail {

class GPUGeneratedLines
{
    bgfx::ProgramHandle mComputeVerticesPH =
        Context::instance()
            .programManager()
            .getComputeProgram<ComputeProgram::CUSTOM_GPU_LINES>();

    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::CUSTOM_CPU_LINES>();


    bool mCustomIndices = false;
    Uniform mCustomIndicesUH = Uniform("u_CustomIndices", bgfx::UniformType::Vec4);
    
    VertexBuffer mVertexCoords;
    VertexBuffer mVertexNormals;
    VertexBuffer mVertexColors;
    VertexBuffer mLineColors;
    IndexBuffer  mLineIndices;

    VertexBuffer mVertices;
    IndexBuffer  mIndices;

public:
    GPUGeneratedLines() { checkCaps(); }

    GPUGeneratedLines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());

    GPUGeneratedLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());

    void swap(GPUGeneratedLines& other);

    friend void swap(GPUGeneratedLines& a, GPUGeneratedLines& b) { a.swap(b); }
    
    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>& lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());
        
    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());
            
    void draw(uint viewId) const;

private:
    void checkCaps() const
    {
        const bgfx::Caps* caps = bgfx::getCaps();
        const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
        if (!computeSupported) {
            throw std::runtime_error("GPU compute not supported");
        }
    }

    void setActiveBuffers(bool coords, bool indices, bool normals, bool colors, bool lineColors);
    
    void allocateVertexCoords(const std::vector<float>& vertCoords);

    void allocateLineIndices(const std::vector<uint>& lineIndices);
    
    void allocateVertexNormals(const std::vector<float>& vertNormals);
    
    void allocateVertexColors(const std::vector<uint>& vertColors);
    
    void allocateVertexLineColors(const std::vector<uint>& lineColors);
    
    void allocateVertexAndIndexBuffer(const uint pointsSize);

    void generateVertexAndIndexBuffer(const uint pointsSize);
};

} // namespace vcl::detail

#endif // VCL_BGFX_PRIMITIVES_LINES_GPU_GENERATED_LINES_H