// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create Algorithms - Create Tetrahedron")
{
    // Default TriMesh tetrahedron
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    REQUIRE(tet.vertexCount() == 4);
    REQUIRE(tet.faceCount() == 4);

    // PolyMesh tetrahedron
    vcl::PolyMesh tetPoly = vcl::createTetrahedron<vcl::PolyMesh>();
    REQUIRE(tetPoly.vertexCount() == 4);
    REQUIRE(tetPoly.faceCount() == 4);

    // Custom points tetrahedron
    vcl::Point3d p0(0, 0, 0);
    vcl::Point3d p1(1, 0, 0);
    vcl::Point3d p2(0, 1, 0);
    vcl::Point3d p3(0, 0, 1);
    vcl::TriMesh tetCustom =
        vcl::createTetrahedron<vcl::TriMesh>(p0, p1, p2, p3);
    REQUIRE(tetCustom.vertexCount() == 4);
    REQUIRE(tetCustom.faceCount() == 4);
}

TEST_CASE("Create Algorithms - Create Hexahedron")
{
    // TriMesh hexahedron (triangulated, 12 faces)
    vcl::TriMesh hexTri = vcl::createHexahedron<vcl::TriMesh>();
    REQUIRE(hexTri.vertexCount() == 8);
    REQUIRE(hexTri.faceCount() == 12);

    // PolyMesh hexahedron (quads, 6 faces)
    vcl::PolyMesh hexPoly = vcl::createHexahedron<vcl::PolyMesh>();
    REQUIRE(hexPoly.vertexCount() == 8);
    REQUIRE(hexPoly.faceCount() == 6);

    // Custom min/max hexahedron
    vcl::Point3d min(-2, -2, -2);
    vcl::Point3d max(2, 2, 2);
    vcl::TriMesh hexCustom = vcl::createHexahedron<vcl::TriMesh>(min, max);
    REQUIRE(hexCustom.vertexCount() == 8);
    REQUIRE(hexCustom.faceCount() == 12);

    // Default Cube
    vcl::TriMesh cubeTri = vcl::createCube<vcl::TriMesh>();
    REQUIRE(cubeTri.vertexCount() == 8);
    REQUIRE(cubeTri.faceCount() == 12);

    vcl::PolyMesh cubePoly = vcl::createCube<vcl::PolyMesh>();
    REQUIRE(cubePoly.vertexCount() == 8);
    REQUIRE(cubePoly.faceCount() == 6);

    // Parametric Cube
    vcl::TriMesh customCube =
        vcl::createCube<vcl::TriMesh>(vcl::Point3d(0, 0, 0), 2.5);
    REQUIRE(customCube.vertexCount() == 8);
    REQUIRE(customCube.faceCount() == 12);
}

TEST_CASE("Create Algorithms - Create Dodecahedron")
{
    // TriMesh dodecahedron (triangulated pentagons)
    vcl::TriMesh dodTri = vcl::createDodecahedron<vcl::TriMesh>();
    REQUIRE(dodTri.vertexCount() == 20);
    REQUIRE(dodTri.faceCount() == 36);

    // PolyMesh dodecahedron (12 pentagons)
    vcl::PolyMesh dodPoly = vcl::createDodecahedron<vcl::PolyMesh>();
    REQUIRE(dodPoly.vertexCount() == 20);
    REQUIRE(dodPoly.faceCount() == 12);
}

TEST_CASE("Create Algorithms - Create Icosahedron")
{
    // Default icosahedron
    vcl::TriMesh ico = vcl::createIcosahedron<vcl::TriMesh>();
    REQUIRE(ico.vertexCount() == 12);
    REQUIRE(ico.faceCount() == 20);

    // Normalized vertices icosahedron
    vcl::TriMesh icoNorm = vcl::createIcosahedron<vcl::TriMesh>(true);
    REQUIRE(icoNorm.vertexCount() == 12);
    REQUIRE(icoNorm.faceCount() == 20);

    // PolyMesh icosahedron
    vcl::PolyMesh icoPoly = vcl::createIcosahedron<vcl::PolyMesh>();
    REQUIRE(icoPoly.vertexCount() == 12);
    REQUIRE(icoPoly.faceCount() == 20);
}

TEST_CASE("Create Algorithms - Create Sphere")
{
    // Default sphere (UV mode)
    vcl::TriMesh sp = vcl::createSphere<vcl::TriMesh>();
    REQUIRE(sp.vertexCount() > 0);
    REQUIRE(sp.faceCount() > 0);

    vcl::Sphere<double> s(vcl::Point3d(0, 0, 0), 1.0);

    // UV sphere
    vcl::TriMesh spUV = vcl::createSphereUV<vcl::TriMesh>(s, 8, 16);
    REQUIRE(spUV.vertexCount() > 0);
    REQUIRE(spUV.faceCount() > 0);

    // Normalized cube sphere
    vcl::TriMesh spNormCube =
        vcl::createSphereNormalizedCube<vcl::TriMesh>(s, 4);
    REQUIRE(spNormCube.vertexCount() > 0);
    REQUIRE(spNormCube.faceCount() > 0);

    // Spherified cube sphere (TriMesh)
    vcl::TriMesh spCubeTri =
        vcl::createSphereSpherifiedCube<vcl::TriMesh>(s, 4);
    REQUIRE(spCubeTri.vertexCount() > 0);
    REQUIRE(spCubeTri.faceCount() > 0);

    // Spherified cube sphere (PolyMesh - quads)
    vcl::PolyMesh spCubePoly =
        vcl::createSphereSpherifiedCube<vcl::PolyMesh>(s, 4);
    REQUIRE(spCubePoly.vertexCount() > 0);
    REQUIRE(spCubePoly.faceCount() > 0);

    // Icosahedron sphere
    vcl::TriMesh spIco = vcl::createSphereIcosahedron<vcl::TriMesh>(s, 1);
    REQUIRE(spIco.vertexCount() > 0);
    REQUIRE(spIco.faceCount() > 0);
}

TEST_CASE("Create Algorithms - Create Cone And Cylinder")
{
    // Cone centered at origin
    vcl::TriMesh cone = vcl::createCone<vcl::TriMesh>(1.0, 0.0, 2.0, 16);
    REQUIRE(cone.vertexCount() == 18);
    REQUIRE(cone.faceCount() == 32);

    // Frustum cone
    vcl::TriMesh frustum = vcl::createCone<vcl::TriMesh>(1.0, 0.5, 2.0, 16);
    REQUIRE(frustum.vertexCount() == 34);
    REQUIRE(frustum.faceCount() == 64);

    // Cone between two points
    vcl::TriMesh conePts = vcl::createCone<vcl::TriMesh>(
        vcl::Point3d(0, 0, 0), vcl::Point3d(0, 2, 0), 1.0, 0.0, 16);
    REQUIRE(conePts.vertexCount() == 18);
    REQUIRE(conePts.faceCount() == 32);

    // Cylinder centered at origin
    vcl::TriMesh cyl = vcl::createCylinder<vcl::TriMesh>(1.0, 2.0, 16);
    REQUIRE(cyl.vertexCount() == 34);
    REQUIRE(cyl.faceCount() == 64);

    // Cylinder between two points
    vcl::TriMesh cylPts = vcl::createCylinder<vcl::TriMesh>(
        vcl::Point3d(0, 0, 0), vcl::Point3d(0, 2, 0), 1.0, 16);
    REQUIRE(cylPts.vertexCount() == 34);
    REQUIRE(cylPts.faceCount() == 64);
}

TEST_CASE("Create Algorithms - Create Axis")
{
    // Axis mesh (combined)
    vcl::TriMesh axis = vcl::createAxis<vcl::TriMesh>(1.0, true);
    REQUIRE(axis.vertexCount() > 0);
    REQUIRE(axis.faceCount() > 0);

    // Axis disjoint
    auto [cyl, conesph] = vcl::createAxisDisjoint<vcl::TriMesh>(1.0, false);
    REQUIRE(cyl.vertexCount() > 0);
    REQUIRE(conesph.vertexCount() > 0);

    // PolyMesh axis
    vcl::PolyMesh axisPoly = vcl::createAxis<vcl::PolyMesh>(1.0, false);
    REQUIRE(axisPoly.vertexCount() > 0);
    REQUIRE(axisPoly.faceCount() > 0);
}

TEST_CASE("Create Algorithms - Create With Custom And Float Meshes")
{
    // Float scalar meshes
    vcl::TriMeshf tetF = vcl::createTetrahedron<vcl::TriMeshf>();
    REQUIRE(tetF.vertexCount() == 4);

    vcl::TriMeshf hexF = vcl::createHexahedron<vcl::TriMeshf>();
    REQUIRE(hexF.vertexCount() == 8);

    vcl::TriMeshf cubeF = vcl::createCube<vcl::TriMeshf>();
    REQUIRE(cubeF.vertexCount() == 8);

    vcl::TriMeshf coneF = vcl::createCone<vcl::TriMeshf>(1.0f, 0.0f, 2.0f, 16);
    REQUIRE(coneF.vertexCount() == 18);

    vcl::TriMeshf cylF = vcl::createCylinder<vcl::TriMeshf>(1.0f, 2.0f, 16);
    REQUIRE(cylF.vertexCount() == 34);

    vcl::TriMeshf spF = vcl::createSphere<vcl::TriMeshf>();
    REQUIRE(spF.vertexCount() > 0);

    vcl::TriMeshf axisF = vcl::createAxis<vcl::TriMeshf>();
    REQUIRE(axisF.vertexCount() > 0);

    // Custom mesh with optional components (MyMesh from types.h)
    MyMesh myTet = vcl::createTetrahedron<MyMesh>();
    REQUIRE(myTet.vertexCount() == 4);

    MyMesh myHex = vcl::createHexahedron<MyMesh>();
    REQUIRE(myHex.vertexCount() == 8);

    MyMesh myCube = vcl::createCube<MyMesh>();
    REQUIRE(myCube.vertexCount() == 8);

    MyMesh myAxis = vcl::createAxis<MyMesh>();
    REQUIRE(myAxis.vertexCount() > 0);
}
