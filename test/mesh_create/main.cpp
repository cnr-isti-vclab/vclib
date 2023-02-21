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

#include <iostream>

#include <vclib/algorithm.h>
#include <vclib/io/save_ply.h>
#include <vclib/mesh.h>

int main()
{
	vcl::io::FileMeshInfo info;

	// want to save just these infos in the files, ignore the rest
	info.setVertices();
	info.setFaces();
	info.setVertexCoords(true, vcl::io::FileMeshInfo::FLOAT);
	info.setFaceVRefs();

	vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();

	assert(m.vertexNumber() == 4);
	assert(m.faceNumber() == 4);

	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/tetrahedron.ply", info);

	m = vcl::createIcosahedron<vcl::TriMesh>(true);
	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/icosahedron.ply", info);

	m = vcl::createHexahedron<vcl::TriMesh>();
	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/hexahedron.ply", info);

	vcl::ConsoleLogger log;
	log.setPrintTimer(true);
	log.startTimer();
	m = vcl::createDodecahedron<vcl::TriMesh>(log);
	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/dodecahedron.ply", info);

	vcl::PolyMesh pm = vcl::createHexahedron<vcl::PolyMesh>();
	vcl::io::savePly(pm, VCL_TEST_RESULTS_PATH "/hexahedron_poly.ply", info);

	pm = vcl::createDodecahedron<vcl::PolyMesh>();
	vcl::io::savePly(pm, VCL_TEST_RESULTS_PATH "/dodecahedron_poly.ply", info);

	pm = vcl::createCube<vcl::PolyMesh>(vcl::Point3d(0,0,0), 4);
	vcl::io::savePly(pm, VCL_TEST_RESULTS_PATH "/cube_poly.ply", info);

	m = vcl::createSphereSpherifiedCube<vcl::TriMesh>({vcl::Point3d(), 1.0}, 50);
	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/sphere_tri.ply");

	pm = vcl::createSphereSpherifiedCube<vcl::PolyMesh>({vcl::Point3d(), 1.0}, 50);
	vcl::io::savePly(pm, VCL_TEST_RESULTS_PATH "/sphere_poly.ply");

	m = vcl::createSphereIcosahedron<vcl::TriMesh>({vcl::Point3d(), 1.0}, 5);
	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/sphere_ico.ply");

	return 0;
}
