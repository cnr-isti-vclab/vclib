/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <iostream>

#include <vclib/algorithms.h>
#include <vclib/io/mesh.h>
#include <vclib/meshes.h>

int main()
{
    vcl::MeshInfo info;

    // want to save just these infos in the files, ignore the rest
    info.setVertices();
    info.setFaces();
    info.setVertexCoords(true, vcl::FLOAT);
    info.setFaceVRefs();

    vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();

    assert(m.vertexNumber() == 4);
    assert(m.faceNumber() == 4);

    vcl::savePly(m, VCLIB_TEST_RESULTS_PATH "/tetrahedron.ply", info);

    m = vcl::createIcosahedron<vcl::TriMesh>(true);
    vcl::savePly(m, VCLIB_TEST_RESULTS_PATH "/icosahedron.ply", info);

    m = vcl::createHexahedron<vcl::TriMesh>();
    vcl::savePly(m, VCLIB_TEST_RESULTS_PATH "/hexahedron.ply", info);

    vcl::ConsoleLogger log;
    log.setPrintTimer(true);
    log.startTimer();
    m = vcl::createDodecahedron<vcl::TriMesh>(log);
    vcl::savePly(m, VCLIB_TEST_RESULTS_PATH "/dodecahedron.ply", info);

    vcl::PolyMesh pm = vcl::createHexahedron<vcl::PolyMesh>();
    vcl::savePly(pm, VCLIB_TEST_RESULTS_PATH "/hexahedron_poly.ply", info);

    pm = vcl::createDodecahedron<vcl::PolyMesh>();
    vcl::savePly(pm, VCLIB_TEST_RESULTS_PATH "/dodecahedron_poly.ply", info);

    pm = vcl::createCube<vcl::PolyMesh>(vcl::Point3d(0, 0, 0), 4);
    vcl::savePly(pm, VCLIB_TEST_RESULTS_PATH "/cube_poly.ply", info);

    vcl::Sphere st(vcl::Point3<vcl::TriMesh::ScalarType>(), 1.0);
    m = vcl::createSphereSpherifiedCube<vcl::TriMesh>(st, 50);
    vcl::savePly(m, VCLIB_TEST_RESULTS_PATH "/sphere_tri.ply");

    vcl::Sphere sp = st.cast<vcl::PolyMesh::ScalarType>();
    pm             = vcl::createSphereSpherifiedCube<vcl::PolyMesh>(sp, 50);
    vcl::savePly(pm, VCLIB_TEST_RESULTS_PATH "/sphere_poly.ply");

    m = vcl::createSphereIcosahedron<vcl::TriMesh>(st, 5);
    vcl::savePly(m, VCLIB_TEST_RESULTS_PATH "/sphere_ico.ply");

    return 0;
}
