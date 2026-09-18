// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SHAPES_UNIFORMS_SHAPE_UNIFORMS_H
#define VCL_BGFX_SHAPES_UNIFORMS_SHAPE_UNIFORMS_H

#include <vclib/bgfx/static_uniform.h>
#include <vclib/space/core/color.h>
#include <bit>

namespace vcl {

/**
 * @brief The ShapeUniforms class is responsible for managing the shader
 * uniforms related to basic shapes.
 *
 * It provides an interface to set the uniform data based on the current shape
 * data and to bind the uniforms to the shader programs.
 */
class ShapeUniforms
{
    inline static std::array<float, 4> sShapeColor = {1.0, 0.0, 0.0, 1.0};
    inline static StaticUniform sShapeColorUniform {
        "u_shapeColor",
        bgfx::UniformType::Vec4};

    inline static std::array<float, 4> sShapeId = {0.0, 0.0, 0.0, 0.0};
    inline static StaticUniform sShapeIdUniform {
        "u_shapeIdPack",
        bgfx::UniformType::Vec4};

public:
    ShapeUniforms() = delete;

    static void setColor(const vcl::Color& color)
    {
        sShapeColor[0] = color.redF();
        sShapeColor[1] = color.greenF();
        sShapeColor[2] = color.blueF();
        sShapeColor[3] = color.alphaF();
    }

    static void setId(uint32_t id)
    {
        sShapeId[0] = std::bit_cast<float>(id);
    }

    static void bind() 
    { 
        sShapeColorUniform.bind(sShapeColor.data()); 
        sShapeIdUniform.bind(sShapeId.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_SHAPES_UNIFORMS_SHAPE_UNIFORMS_H
