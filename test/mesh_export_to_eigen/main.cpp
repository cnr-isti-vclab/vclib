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

#include <vclib/algorithms/export_to_matrix.h>
#include <vclib/io/load_ply.h>
#include <vclib/trimesh.h>
#include <vclib/polymesh.h>

int main()
{
	vcl::TriMesh tm = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/cube_tri.ply");

	Eigen::MatrixXd v = vcl::vertexMatrix<Eigen::MatrixXd>(tm);
	Eigen::MatrixXi f = vcl::faceMatrix<Eigen::MatrixXi>(tm);

	std::cerr << "Vertices:\n" << v << "\n\n";

	std::cerr << "Faces:\n" << f << "\n\n";

	vcl::PolyMesh pm = vcl::io::loadPly<vcl::PolyMesh>(VCL_TEST_MODELS_PATH "/cube_poly.ply");

	v = vcl::vertexMatrix<Eigen::MatrixXd>(pm);
	f = vcl::faceMatrix<Eigen::MatrixXi>(pm);

	std::cerr << "Vertices:\n" << v << "\n\n";

	std::cerr << "Faces:\n" << f << "\n\n";

	return 0;
}
