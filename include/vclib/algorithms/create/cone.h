/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_ALGORITHMS_CREATE_CONE_H
#define VCL_ALGORITHMS_CREATE_CONE_H

#include <vclib/math/base.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<FaceMeshConcept MeshType>
MeshType createCone(
    auto radiusBottom,
    auto radiusTop,
    auto height,
    const uint subdivisions = 36)
{
    using CoordType = MeshType::VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;

    uint vn, fn;
    if (radiusBottom == 0 || radiusTop == 0) {
        vn = subdivisions + 2;
        fn = subdivisions * 2;
    }
    else {
        vn = subdivisions * 2 + 2;
        fn = subdivisions * 4;
    }

    MeshType mesh;

    mesh.reserveVertices(vn);
    mesh.reserveFaces(fn);

    mesh.addVertex(CoordType(0, -height/2.0, 0));
    mesh.addVertex(CoordType(0, height/2.0, 0));

    uint b1 = 2;
    uint b2 = 2;

    if (radiusBottom != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            ScalarType a = vcl::toRad(i * 360.0 / subdivisions);
            mesh.addVertex(CoordType(
                radiusBottom * std::cos(a),
                -height/2.0,
                radiusBottom * std::sin(a)));
        }

        b2 += subdivisions;
    }

    if (radiusTop != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            ScalarType a = vcl::toRad(i * 360.0 / subdivisions);
            mesh.addVertex(CoordType(
                radiusTop * std::cos(a),
                height/2.0,
                radiusTop * std::sin(a)));
        }
    }

    if (radiusBottom != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(0, b1 + i, b1 + (i + 1) % subdivisions);
        }
    }
    else {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(0, b2 + i, b2 + (i + 1) % subdivisions);
        }
    }

    if (radiusTop != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(1, b2 + (i + 1) % subdivisions, b2 + i);
        }
    }
    else {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(1, b1 + i, b1 + (i + 1) % subdivisions);
        }
    }

    // side triangles
    if (radiusBottom != 0 && radiusTop != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(b1 + i, b2 + i, b2 + (i + 1) % subdivisions);
            mesh.addFace(
                b1 + i,
                b2 + (i + 1) % subdivisions,
                b1 + (i + 1) % subdivisions);
        }
    }

    return mesh;
}

template<FaceMeshConcept MeshType>
MeshType createCylinder(
    auto radius,
    auto height,
    const uint subdivisions = 36)
{
    return createCone<MeshType>(radius, radius, height, subdivisions);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CREATE_CONE_H
