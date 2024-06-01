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

#include <vclib_bgfx/render/drawable/drawable_axis.h>

#include <vclib/algorithms/mesh/create.h>

namespace vcl {

DrawableAxis::DrawableAxis(double size, bool fromOrigin)
{
    createAxis(fromOrigin);

    updateMatrices(size);
}

void DrawableAxis::setSize(double size)
{
    updateMatrices(size);
}

void DrawableAxis::draw(uint viewId)
{
    if (isVisible()) {
        if (bgfx::isValid(mProgram)) {
            uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                             BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                             BGFX_STATE_MSAA;
            for (uint i = 0; i < 3; i++) {
                for (uint j = 0; j < 2; j++) {
                    if (j == 0) // cylinders
                        mUniforms.setColor(mColors[i]);
                    else // rest (cone, spheres...)
                        mUniforms.setColor(vcl::Color::White);
                    mUniforms.bind();

                    mArrowBuffers[j].bindVertexBuffers(MeshRenderSettings());
                    mArrowBuffers[j].bindIndexBuffers();

                    bgfx::setTransform(mMatrices[i].data());

                    bgfx::submit(viewId, mProgram);
                }
            }
        }
    }
}

void DrawableAxis::updateMatrices(double size)
{
    mMatrices[0](0, 1) = size;
    mMatrices[0](1, 0) = -size;
    mMatrices[0](2, 2) = size;
    mMatrices[0](3, 3) = 1;

    mMatrices[1](0, 0) = size;
    mMatrices[1](1, 1) = size;
    mMatrices[1](2, 2) = size;
    mMatrices[1](3, 3) = 1;

    mMatrices[2](0, 0) = size;
    mMatrices[2](1, 2) = -size;
    mMatrices[2](2, 1) = size;
    mMatrices[2](3, 3) = 1;
}

void DrawableAxis::createAxis(bool fromOrigin)
{
    const double unitLength = 1.0;

    const double cylLength = fromOrigin ? unitLength : unitLength * 2;
    const double cylRadius = cylLength * 0.0025;

    const double coneRadius = cylRadius * 10;
    const double coneLength = cylLength * 0.1;

    const double firstSphereRadius  = unitLength * 0.02;
    const double commonSphereRadius = unitLength * 0.008;

    vcl::TriMesh cylinder =
        vcl::createCylinder<vcl::TriMesh>(cylRadius, cylLength);
    if (fromOrigin) {
        vcl::translate(cylinder, vcl::Point3d(0, unitLength * 0.5, 0));
    }

    vcl::updatePerVertexNormals(cylinder);

    vcl::TriMesh rest =
        vcl::createCone<vcl::TriMesh>(coneRadius, 0, coneLength);
    double transl = unitLength + (coneLength * 0.5);
    vcl::translate(rest, vcl::Point3d(0, transl, 0));

    if (!fromOrigin) {
        vcl::Sphered s(vcl::Point3d(0, -1, 0), firstSphereRadius);
        vcl::TriMesh sp = vcl::createSphere<vcl::TriMesh>(s);
        rest.append(sp);
    }

    for (uint i = 0; i < 9; ++i) {
        const double step = unitLength * 0.1;
        const double x    = step + i * step;
        vcl::Sphered s(vcl::Point3d(0, x, 0), commonSphereRadius);
        vcl::TriMesh sp = vcl::createSphere<vcl::TriMesh>(s);
        rest.append(sp);
        if (!fromOrigin) {
            s.center().y() = -x;
            sp             = vcl::createSphere<vcl::TriMesh>(s);
            rest.append(sp);
        }
    }

    const double rad = fromOrigin ? firstSphereRadius : commonSphereRadius;
    vcl::Sphered s   = vcl::Sphered(vcl::Point3d(0, 0, 0), rad);
    vcl::TriMesh sp  = vcl::createSphere<vcl::TriMesh>(s);
    rest.append(sp);

    vcl::updatePerVertexNormals(rest);

    mArrowBuffers[0] = MeshRenderBuffers<vcl::TriMesh>(
        cylinder,
        MeshRenderBuffers<vcl::TriMesh>::VERT_NORMALS |
            MeshRenderBuffers<vcl::TriMesh>::TRIANGLES);

    mArrowBuffers[1] = MeshRenderBuffers<vcl::TriMesh>(
        rest,
        MeshRenderBuffers<vcl::TriMesh>::VERT_NORMALS |
            MeshRenderBuffers<vcl::TriMesh>::TRIANGLES);
}

} // namespace vcl
