/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include <vclib/dcel_mesh.h>
#include <vclib/algorithms/create/tetrahedron.h>
#include <vclib/io/save.h>

int main()
{
	vcl::DcelMesh m;

	m.addVertices(vcl::Point3d(0,0,0), vcl::Point3d(1,1,1), vcl::Point3d(2,2,2));
	m.addFace(0, 1, 2);

	for (const vcl::DcelMesh::Vertex* v : m.face(0).vertices()) {
		std::cerr << v->coord() << "\n";
	}

	vcl::DcelMesh t = vcl::createTetrahedron<vcl::DcelMesh>();
	vcl::io::save(t, VCL_TEST_RESULTS_PATH "/tet_dcel.ply");

	return 0;
}
