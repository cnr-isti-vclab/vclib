// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_CREATE_AXIS_H
#define VCL_ALGORITHMS_MESH_CREATE_AXIS_H

#include "cone.h"
#include "sphere.h"

#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/algorithms/mesh/update/transform.h>

namespace vcl {

namespace detail {

template<FaceMeshConcept MeshType>
MeshType createAxisCylinder(double unitLength, bool fromOrigin = false)
{
    using ScalarType = ScalarType<MeshType>;
    using Point3s    = vcl::Point3<ScalarType>;

    const ScalarType cylLength = fromOrigin ? unitLength : unitLength * 2;
    const ScalarType cylRadius = cylLength * 0.0025;

    const ScalarType coneRadius = cylRadius * 10;
    const ScalarType coneLength = cylLength * 0.1;

    const ScalarType firstSphereRadius  = unitLength * 0.02;
    const ScalarType commonSphereRadius = unitLength * 0.008;

    MeshType cylinder = vcl::createCylinder<MeshType>(cylRadius, cylLength);

    if (fromOrigin) {
        vcl::translate(cylinder, Point3s(0, unitLength * 0.5, 0));
    }

    if constexpr (HasVertexNormal<MeshType>) {
        if (cylinder.isVertexNormalAvailable()) {
            vcl::updateVertexNormals(cylinder);
        }
    }

    return cylinder;
}

template<FaceMeshConcept MeshType>
MeshType createAxisConeSpheres(double unitLength, bool fromOrigin = false)
{
    using ScalarType = ScalarType<MeshType>;
    using Point3s    = vcl::Point3<ScalarType>;
    using SSphere    = vcl::Sphere<ScalarType>;

    const ScalarType cylLength = fromOrigin ? unitLength : unitLength * 2;
    const ScalarType cylRadius = cylLength * 0.0025;

    const ScalarType coneRadius = cylRadius * 10;
    const ScalarType coneLength = cylLength * 0.1;

    const ScalarType firstSphereRadius  = unitLength * 0.02;
    const ScalarType commonSphereRadius = unitLength * 0.008;

    MeshType coneSpheres = vcl::createCone<MeshType>(coneRadius, 0, coneLength);
    ScalarType transl    = unitLength + (coneLength * 0.5);
    vcl::translate(coneSpheres, Point3s(0, transl, 0));

    if (!fromOrigin) {
        SSphere  s(Point3s(0, -1, 0), firstSphereRadius);
        MeshType sp = vcl::createSphere<MeshType>(s);
        coneSpheres.append(sp);
    }

    for (uint i = 0; i < 9; ++i) {
        const ScalarType step = unitLength * 0.1;
        const ScalarType x    = step + i * step;
        SSphere          s(Point3s(0, x, 0), commonSphereRadius);
        MeshType         sp = vcl::createSphere<MeshType>(s);
        coneSpheres.append(sp);
        if (!fromOrigin) {
            s.center().y() = -x;
            sp             = vcl::createSphere<MeshType>(s);
            coneSpheres.append(sp);
        }
    }

    const ScalarType rad = fromOrigin ? firstSphereRadius : commonSphereRadius;
    SSphere          s(Point3s(0, 0, 0), rad);
    MeshType         sp = vcl::createSphere<MeshType>(s);
    coneSpheres.append(sp);

    if constexpr (HasVertexNormal<MeshType>) {
        if (coneSpheres.isVertexNormalAvailable()) {
            vcl::updateVertexNormals(coneSpheres);
        }
    }

    return coneSpheres;
}

} // namespace detail

template<FaceMeshConcept MeshType>
std::pair<MeshType, MeshType> createAxisDisjoint(
    double unitLength = 1,
    bool   fromOrigin = false)
{
    return std::make_pair(
        detail::createAxisCylinder<MeshType>(unitLength, fromOrigin),
        detail::createAxisConeSpheres<MeshType>(unitLength, fromOrigin));
}

template<FaceMeshConcept MeshType>
MeshType createAxis(double unitLength = 1, bool fromOrigin = false)
{
    MeshType axis =
        detail::createAxisCylinder<MeshType>(unitLength, fromOrigin);

    axis.append(
        detail::createAxisConeSpheres<MeshType>(unitLength, fromOrigin));

    return axis;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_AXIS_H
