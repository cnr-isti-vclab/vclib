// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_CREATE_CONE_H
#define VCL_ALGORITHMS_MESH_CREATE_CONE_H

#include <vclib/mesh.h>

namespace vcl {

template<FaceMeshConcept MeshType>
MeshType createCone(
    auto       radiusBottom,
    auto       radiusTop,
    auto       height,
    const uint subdivisions = 36)
{
    using PositionType = MeshType::VertexType::PositionType;
    using ScalarType   = PositionType::ScalarType;

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

    mesh.addVertex(PositionType(0, -height / 2.0, 0));
    mesh.addVertex(PositionType(0, height / 2.0, 0));

    uint b1 = 2;
    uint b2 = 2;

    if (radiusBottom != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            ScalarType a = toRad(i * 360.0 / subdivisions);
            mesh.addVertex(PositionType(
                radiusBottom * std::cos(a),
                -height / 2.0,
                radiusBottom * std::sin(a)));
        }

        b2 += subdivisions;
    }

    if (radiusTop != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            ScalarType a = toRad(i * 360.0 / subdivisions);
            mesh.addVertex(PositionType(
                radiusTop * std::cos(a),
                height / 2.0,
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
            mesh.addFace(1, b1 + (i + 1) % subdivisions, b1 + i);
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
MeshType createCylinder(auto radius, auto height, const uint subdivisions = 36)
{
    return createCone<MeshType>(radius, radius, height, subdivisions);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_CONE_H
