/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_ALGORITHMS_MESH_CREATE_AXIS_H
#define VCL_ALGORITHMS_MESH_CREATE_AXIS_H

#include "cone.h"
#include "sphere.h"

namespace vcl {

namespace detail {

template<FaceMeshConcept MeshType>
MeshType createAxisCylinder(double unitLength, bool fromOrigin = false)
{
    const double cylLength = fromOrigin ? unitLength : unitLength * 2;
    const double cylRadius = cylLength * 0.0025;

    const double coneRadius = cylRadius * 10;
    const double coneLength = cylLength * 0.1;

    const double firstSphereRadius  = unitLength * 0.02;
    const double commonSphereRadius = unitLength * 0.008;

    MeshType cylinder = vcl::createCylinder<MeshType>(cylRadius, cylLength);

    if (fromOrigin) {
        vcl::translate(cylinder, vcl::Point3d(0, unitLength * 0.5, 0));
    }

    vcl::updatePerVertexNormals(cylinder);

    return cylinder;
}

template<FaceMeshConcept MeshType>
MeshType createAxisConeSpheres(double unitLength, bool fromOrigin = false)
{
    const double cylLength = fromOrigin ? unitLength : unitLength * 2;
    const double cylRadius = cylLength * 0.0025;

    const double coneRadius = cylRadius * 10;
    const double coneLength = cylLength * 0.1;

    const double firstSphereRadius  = unitLength * 0.02;
    const double commonSphereRadius = unitLength * 0.008;

    MeshType coneSpheres = vcl::createCone<MeshType>(coneRadius, 0, coneLength);
    double   transl      = unitLength + (coneLength * 0.5);
    vcl::translate(coneSpheres, vcl::Point3d(0, transl, 0));

    if (!fromOrigin) {
        vcl::Sphered s(vcl::Point3d(0, -1, 0), firstSphereRadius);
        MeshType     sp = vcl::createSphere<MeshType>(s);
        coneSpheres.append(sp);
    }

    for (uint i = 0; i < 9; ++i) {
        const double step = unitLength * 0.1;
        const double x    = step + i * step;
        vcl::Sphered s(vcl::Point3d(0, x, 0), commonSphereRadius);
        MeshType     sp = vcl::createSphere<MeshType>(s);
        coneSpheres.append(sp);
        if (!fromOrigin) {
            s.center().y() = -x;
            sp             = vcl::createSphere<MeshType>(s);
            coneSpheres.append(sp);
        }
    }

    const double rad = fromOrigin ? firstSphereRadius : commonSphereRadius;
    vcl::Sphered s   = vcl::Sphered(vcl::Point3d(0, 0, 0), rad);
    MeshType     sp  = vcl::createSphere<MeshType>(s);
    coneSpheres.append(sp);

    vcl::updatePerVertexNormals(coneSpheres);

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
