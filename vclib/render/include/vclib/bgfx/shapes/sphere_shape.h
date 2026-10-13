// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SHAPES_SPHERE_SHAPE_H
#define VCL_BGFX_SHAPES_SPHERE_SHAPE_H

#include <vclib/bgfx/shapes/shape.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief A basic 3D sphere shape.
 *
 * Creates a sphere mesh centered at the origin with a default radius of 1.
 */
class SphereShape : public Shape
{
public:
    SphereShape() : Shape(vcl::createSphere<vcl::TriMesh>()) {}

    template<SphereConcept S>
    SphereShape(
        const S&                sp,
        const CreateSphereArgs& args = CreateSphereArgs()) :
            Shape(vcl::createSphere<vcl::TriMesh>(sp, args))
    {
    }
};

} // namespace vcl

#endif // VCL_BGFX_SHAPES_SPHERE_SHAPE_H
