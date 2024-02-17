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

#include "vclib/algorithms/update/transform.h"
#include <vclib/ext/bgfx/drawable_directional_light.h>

#include <vclib/ext/bgfx/shader_programs/drawable_directional_light_shader_program.h>
#include <vclib/math/transform.h>
#include <vclib/render/matrix.h>

namespace vcl::bgf {

DrawableDirectionalLight::DrawableDirectionalLight()
{
    // create a vertex buffer that will contain the endpoints of the lines that
    // describe the light
    // there are 4 x 4 lines having Z direction in the [-0.75, 0.75] viewport

    vertices.reserve(16 * 2 * 3);

    const float low  = -0.75f;
    const float high = 0.75f;

    const float zlow  = -1.5;
    const float zhigh = -5;
    const uint  n     = 4;
    const float step  = (high - low) / (n - 1);

    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < n; ++j) {
            vertices.push_back(low + i * step);
            vertices.push_back(low + j * step);
            vertices.push_back(zlow);

            vertices.push_back(low + i * step);
            vertices.push_back(low + j * step);
            vertices.push_back(zhigh);
        }
    }

    // vcl::setTransformMatrixRotationDeg(transform, vcl::Point3f(0, 1,
    // 0), 90.0f); transform = transform.transpose().eval();

    updateVertexBuffer();
    setLinesColor(lColor);
}

DrawableDirectionalLight::~DrawableDirectionalLight()
{
    if (bgfx::isValid(vertexCoordBH)) {
        bgfx::destroy(vertexCoordBH);
    }
}

void DrawableDirectionalLight::update(const DirectionalLight<float>& l)
{
    light = l;

    // make the transform matrix such that the lines will be drawn in the
    // direction of the light
    // transform = vcl::lookAtMatrix<vcl::Matrix44f>(
    //     vcl::Point3f(0, 0, 0), light.direction(), vcl::Point3f(0, 0, 1));
}

void DrawableDirectionalLight::setLinesColor(const Color& c)
{
    lColor = c;
    dlUniforms.setColor(lColor);
}

void DrawableDirectionalLight::setShaderProgram(const ShaderProgramI& sp)
{
    const DrawableDirectionalLightShaderProgram* ptr =
        dynamic_cast<const DrawableDirectionalLightShaderProgram*>(&sp);

    if (ptr) {
        program = ptr->program();
    }
}

void DrawableDirectionalLight::draw(uint viewId)
{
    if (isVisible()) {
        if (bgfx::isValid(program)) {
            bgfx::setState(
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                BGFX_STATE_PT_LINES);

            bgfx::setTransform(transform.data());

            dlUniforms.bind();

            bgfx::setVertexBuffer(0, vertexCoordBH);

            bgfx::submit(viewId, program);
        }
    }
}

Point3d DrawableDirectionalLight::center() const
{
    // should not used because this center is in view space
    return Point3d(0, 0, 0);
}

double DrawableDirectionalLight::radius() const
{
    return 1.0;
}

DrawableObjectI* DrawableDirectionalLight::clone() const
{
    return new DrawableDirectionalLight(*this);
}

void DrawableDirectionalLight::updateVertexBuffer()
{
    if (bgfx::isValid(vertexCoordBH)) {
        bgfx::destroy(vertexCoordBH);
    }

    // vertex buffer (positions)
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();

    vertexCoordBH = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices.data(), vertices.size() * sizeof(float)),
        layout);
}

} // namespace vcl::bgf
