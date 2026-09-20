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

/**
 * @brief Creates and returns a cone or a frustum of a cone between two points.
 *
 * The function generates a mesh representing a cone or a frustum of a cone,
 * having the base centered in \p p1 and the top (or tip) centered in \p p2.
 * If the provided \p radiusTop is 0, a standard cone with the tip at \p p2
 * is created. If both radii are non-zero, a truncated cone (frustum) is
 * generated. The generated mesh is always composed of triangles.
 *
 * @param[in] p1: The center of the bottom base.
 * @param[in] p2: The center of the top base (or the tip of the cone).
 * @param[in] radiusBottom: The radius of the bottom base.
 * @param[in] radiusTop: The radius of the top base.
 * @param[in] subdivisions: The number of sectors (slices) of the cone.
 *
 * @return A Mesh containing the cone or frustum.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType, Point3Concept PointType>
MeshType createCone(
    const PointType& p1,
    const PointType& p2,
    auto             radiusBottom,
    auto             radiusTop,
    const uint       subdivisions = 36)
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

    mesh.addVertex(p1);
    mesh.addVertex(p2);

    PositionType p1Pos = p1;
    PositionType p2Pos = p2;
    PositionType d     = p2Pos - p1Pos;

    PositionType u(1, 0, 0), v(0, 0, -1);
    ScalarType   length = d.norm();
    if (length > 0) {
        PositionType n = d / length;
        n.orthoBase(u, v);
    }

    uint b1 = 2;
    uint b2 = 2;

    if (radiusBottom != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            ScalarType a = toRad(i * 360.0 / subdivisions);
            mesh.addVertex(
                p1Pos + u * (radiusBottom * std::cos(a)) +
                v * (radiusBottom * std::sin(a)));
        }
        b2 += subdivisions;
    }

    if (radiusTop != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            ScalarType a = toRad(i * 360.0 / subdivisions);
            mesh.addVertex(
                p2Pos + u * (radiusTop * std::cos(a)) +
                v * (radiusTop * std::sin(a)));
        }
    }

    if (radiusBottom != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(0, b1 + (i + 1) % subdivisions, b1 + i);
        }
    }
    else {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(0, b2 + (i + 1) % subdivisions, b2 + i);
        }
    }

    if (radiusTop != 0) {
        for (uint i = 0; i < subdivisions; ++i) {
            mesh.addFace(1, b2 + i, b2 + (i + 1) % subdivisions);
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
            mesh.addFace(b1 + i, b2 + (i + 1) % subdivisions, b2 + i);
            mesh.addFace(
                b1 + i,
                b1 + (i + 1) % subdivisions,
                b2 + (i + 1) % subdivisions);
        }
    }

    return mesh;
}

/**
 * @brief Creates and returns a cone or a frustum of a cone.
 *
 * The function generates a mesh representing a cone or a frustum of a cone
 * aligned with the Y axis and centered at the origin. The bottom base is
 * at ``y = -height/2`` and the top base is at ``y = height/2``.
 * The generated mesh is always composed of triangles.
 *
 * @param[in] radiusBottom: The radius of the bottom base.
 * @param[in] radiusTop: The radius of the top base.
 * @param[in] height: The total height of the cone.
 * @param[in] subdivisions: The number of sectors (slices) of the cone.
 *
 * @return A Mesh containing the cone or frustum.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createCone(
    auto       radiusBottom,
    auto       radiusTop,
    auto       height,
    const uint subdivisions = 36)
{
    using PositionType = MeshType::VertexType::PositionType;
    return createCone<MeshType>(
        PositionType(0, -height / 2.0, 0),
        PositionType(0, height / 2.0, 0),
        radiusBottom,
        radiusTop,
        subdivisions);
}

/**
 * @brief Creates and returns a cylinder between two points.
 *
 * The function generates a mesh representing a cylinder, having its bottom base
 * centered in \p p1 and its top base centered in \p p2.
 * The generated mesh is always composed of triangles.
 *
 * @param[in] p1: The center of the bottom base.
 * @param[in] p2: The center of the top base.
 * @param[in] radius: The radius of the cylinder.
 * @param[in] subdivisions: The number of sectors (slices) of the cylinder.
 *
 * @return A Mesh containing the cylinder.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType, Point3Concept PointType>
MeshType createCylinder(
    const PointType& p1,
    const PointType& p2,
    auto             radius,
    const uint       subdivisions = 36)
{
    return createCone<MeshType>(p1, p2, radius, radius, subdivisions);
}

/**
 * @brief Creates and returns a cylinder.
 *
 * The function generates a mesh representing a cylinder aligned with the Y axis
 * and centered at the origin. The bottom base is at ``y = -height/2`` and the
 * top base is at ``y = height/2``. The generated mesh is always composed of
 * triangles.
 *
 * @param[in] radius: The radius of the cylinder.
 * @param[in] height: The total height of the cylinder.
 * @param[in] subdivisions: The number of sectors (slices) of the cylinder.
 *
 * @return A Mesh containing the cylinder.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createCylinder(auto radius, auto height, const uint subdivisions = 36)
{
    using PositionType = MeshType::VertexType::PositionType;
    return createCylinder<MeshType>(
        PositionType(0, -height / 2.0, 0),
        PositionType(0, height / 2.0, 0),
        radius,
        subdivisions);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_CONE_H
