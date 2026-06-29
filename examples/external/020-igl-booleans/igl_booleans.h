// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef IGL_BOOLEANS_H
#define IGL_BOOLEANS_H

#include <vclib/igl/booleans.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

auto meshBooleans()
{
    using namespace vcl;
    using namespace vcl::igl;

    TriMesh m1;
    loadMesh(m1, VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

    TriMesh m2;
    loadMesh(m2, VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    TriMesh mUnion = meshBoolean(m1, m2, MeshBoolean::UNION);
    mUnion.name()  = "union";

    TriMesh mIntersection = meshBoolean(m1, m2, MeshBoolean::INTERSECTION);
    mIntersection.name()  = "intersection";

    return std::make_tuple(m1, m2, mUnion, mIntersection);
}

#endif // IGL_BOOLEANS_H
