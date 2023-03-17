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
#include <vclib/load_save.h>
#include <vclib/space/array.h>
#include <vclib/mesh.h>

int main()
{
	vcl::TriMesh tm = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/cube_tri.ply");

	Eigen::MatrixXd v = vcl::vertexMatrix<Eigen::MatrixXd>(tm);
	Eigen::MatrixXi f = vcl::faceMatrix<Eigen::MatrixXi>(tm);

	std::cerr << "Vertices:\n" << v << "\n\n";

	std::cerr << "Faces:\n" << f << "\n\n";

	vcl::PolyMesh pm = vcl::io::loadPly<vcl::PolyMesh>(VCL_TEST_MODELS_PATH "/cube_poly.ply");

	vcl::Array2<double> va = vcl::vertexMatrix<vcl::Array2<double>>(pm);
	vcl::Array2<int> fa = vcl::faceMatrix<vcl::Array2<int>>(pm);

	std::cerr << "Vertices:\n" << va << "\n\n";

	std::cerr << "Faces:\n" << fa << "\n\n";

	return 0;
}
