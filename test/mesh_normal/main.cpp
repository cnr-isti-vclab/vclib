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

#include <vclib/algorithms/update/normal.h>
#include <vclib/io/load_ply.h>
#include <vclib/trimesh.h>

int main()
{
	vcl::TriMesh m;

	vcl::io::loadPly(m, VCL_TEST_MODELS_PATH "/cube_tri.ply");

	vcl::updatePerFaceNormals(m);

	for (const vcl::TriMesh::Face& f : m.faces()) {
		std::cerr << "Face " << m.index(f) << " normal: " << f.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::updatePerVertexNormals(m);

	for (const vcl::TriMesh::Vertex& v : m.vertices()) {
		std::cerr << "Vertex " << m.index(v) << " normal: " << v.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::updatePerVertexNormalsAngleWeighted(m);

	for (const vcl::TriMesh::Vertex& v : m.vertices()) {
		std::cerr << "Vertex " << m.index(v) << " normal: " << v.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	return 0;
}
