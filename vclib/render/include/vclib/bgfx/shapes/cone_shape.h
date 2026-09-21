// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SHAPES_CONE_SHAPE_H
#define VCL_BGFX_SHAPES_CONE_SHAPE_H

#include <vclib/bgfx/shapes/shape.h>

#include <vclib/algorithms/mesh.h>

namespace vcl {

/**
 * @brief A basic 3D cone shape.
 *
 * Creates a cone mesh with the base at Y=-height/2 and apex at Y=height/2,
 * with a default base radius of 1.
 */
class ConeShape : public Shape
{
public:
    ConeShape() : ConeShape(1.0, 0.0, 1.0) {}

    ConeShape(
        double radiusBottom,
        double radiusTop,
        double height,
        uint   subdivisions = 36) :
            Shape(
                vcl::createCone<vcl::TriMesh>(
                    radiusBottom,
                    radiusTop,
                    height,
                    subdivisions))
    {
    }

    template<Point3Concept PointType>
    ConeShape(
        const PointType& p1,
        const PointType& p2,
        double           radiusBottom,
        double           radiusTop,
        uint             subdivisions = 36) :
            Shape(
                vcl::createCone<vcl::TriMesh>(
                    p1,
                    p2,
                    radiusBottom,
                    radiusTop,
                    subdivisions))
    {
    }
};

} // namespace vcl

#endif // VCL_BGFX_SHAPES_CONE_SHAPE_H
