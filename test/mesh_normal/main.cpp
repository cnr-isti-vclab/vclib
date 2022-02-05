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
#include <vclib/tri_mesh.h>
#include <vclib/poly_mesh.h>

template<typename MeshType>
void updateAndPrintNormals(MeshType& m){
	vcl::updatePerFaceNormals(m);

	std::cerr << "Face Normals:\n\n";

	for (const auto& f : m.faces()) {
		std::cerr << "Face " << m.index(f) << " normal: " << f.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::updatePerVertexNormals(m);

	std::cerr << "Vertex Normals:\n\n";

	for (const auto& v : m.vertices()) {
		std::cerr << "Vertex " << m.index(v) << " normal: " << v.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::updatePerVertexNormalsFromFaceNormals(m);

	std::cerr << "Vertex Normals from Faces:\n\n";

	for (const auto& v : m.vertices()) {
		std::cerr << "Vertex " << m.index(v) << " normal: " << v.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::updatePerVertexNormalsAngleWeighted(m);

	std::cerr << "Vertex Normals angle weighted:\n\n";

	for (const auto& v : m.vertices()) {
		std::cerr << "Vertex " << m.index(v) << " normal: " << v.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::updatePerVertexNormalsNelsonMaxWeighted(m);

	std::cerr << "Vertex Normals Nelson Max weighted:\n\n";

	for (const auto& v : m.vertices()) {
		std::cerr << "Vertex " << m.index(v) << " normal: " << v.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::Matrix44<double> m44;
	m44.setZero();
	m44.diagonal() << -1, -1, -1, -1;
	vcl::multiplyPerFaceNormalsByMatrix(m, m44);

	std::cerr << "Multiplied Face Normals:\n\n";

	for (const auto& f : m.faces()) {
		std::cerr << "Face " << m.index(f) << " normal: " << f.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;

	vcl::multiplyPerVertexNormalsByMatrix(m, m44);

	std::cerr << "Multiplied Vertex Normals:\n\n";

	for (const auto& v : m.vertices()) {
		std::cerr << "Vertex " << m.index(v) << " normal: " << v.normal() << "\n";
		std::cerr << std::endl;
	}

	std::cerr << std::endl << std::endl;
}

int main()
{
	vcl::TriMesh tm;

	vcl::io::loadPly(tm, VCL_TEST_MODELS_PATH "/cube_tri.ply");

	std::cerr << "========= TriMesh =========\n\n";

	updateAndPrintNormals(tm);

	vcl::PolyMesh pm;

	vcl::io::loadPly(pm, VCL_TEST_MODELS_PATH "/cube_poly.ply");

	std::cerr << "========= PolyMesh =========\n\n";

	updateAndPrintNormals(pm);

	return 0;
}
