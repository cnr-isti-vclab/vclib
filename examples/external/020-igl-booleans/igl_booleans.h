// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef IGL_BOOLEANS_H
#define IGL_BOOLEANS_H

#include <vclib/algorithms/mesh.h>
#include <vclib/igl/booleans.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

auto meshBooleans()
{
    using namespace vcl;
    using namespace vcl::igl;

    TriMesh m1;
    loadMesh(m1, VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");
    vcl::updatePerVertexAndFaceNormals(m1);
    m1.enablePerFaceColor();
    m1.enablePerVertexColor();
    setPerFaceColor(m1, vcl::Color::LightRed);
    setPerVertexColor(m1, vcl::Color::LightMagenta);

    TriMesh m2;
    loadMesh(m2, VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    vcl::updatePerVertexAndFaceNormals(m2);
    m2.enablePerFaceColor();
    m2.enablePerVertexColor();
    setPerFaceColor(m2, vcl::Color::LightBlue);
    setPerVertexColor(m2, vcl::Color::LightYellow);

    TriMesh mUnion = meshBoolean(m1, m2, MeshBoolean::UNION);
    mUnion.name()  = "union";

    TriMesh mIntersection = meshBoolean(m1, m2, MeshBoolean::INTERSECTION);
    mIntersection.name()  = "intersection";

    return std::make_tuple(m1, m2, mUnion, mIntersection);
}

auto polyMeshBooleans()
{
    using namespace vcl;
    using namespace vcl::igl;

    PolyMesh m1;
    loadMesh(m1, VCLIB_EXAMPLE_MESHES_PATH "/spot/spot_quadrangulated.obj");
    vcl::updatePerVertexAndFaceNormals(m1);

    PolyMesh m2;
    loadMesh(m2, VCLIB_EXAMPLE_MESHES_PATH "/maneki_neko.ply");
    vcl::updatePerVertexAndFaceNormals(m2);

    PolyMesh mUnion = meshBoolean(m1, m2, MeshBoolean::UNION);
    mUnion.name()   = "union";

    PolyMesh mIntersection = meshBoolean(m1, m2, MeshBoolean::INTERSECTION);
    mIntersection.name()   = "intersection";

    return std::make_tuple(m1, m2, mUnion, mIntersection);
}

#endif // IGL_BOOLEANS_H
