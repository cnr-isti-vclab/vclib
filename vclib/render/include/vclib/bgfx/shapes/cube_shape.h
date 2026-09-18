// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SHAPES_CUBE_SHAPE_H
#define VCL_BGFX_SHAPES_CUBE_SHAPE_H

#include <vclib/algorithms/mesh/create.h>
#include <vclib/bgfx/shapes/shape.h>

namespace vcl {

/**
 * @brief A basic 3D cube shape.
 *
 * Creates a cube mesh extending from -1 to 1 along each axis.
 */
class CubeShape : public Shape
{
public:
    CubeShape() : Shape(vcl::createHexahedron<vcl::TriMesh>()) {}

    template<Point3Concept P1, Point3Concept P2>
    CubeShape(const P1& min, const P2& max) :
            Shape(vcl::createHexahedron<vcl::TriMesh>(min, max))
    {
    }
};

} // namespace vcl

#endif // VCL_BGFX_SHAPES_CUBE_SHAPE_H
