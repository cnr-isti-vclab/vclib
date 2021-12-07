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

#include <vclib/algorithms/update/transform.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/trimesh.h>
#include <vclib/polymesh.h>

int main()
{
	vcl::TriMesh tm;

	vcl::io::loadPly(tm, VCL_TEST_MODELS_PATH "/bunny_textured.ply");

	vcl::TriMesh m = tm;

	vcl::Matrix44<double> m44;
	m44.setZero();
	m44.diagonal() << -1, -1, -1, 1;

	vcl::applyTransformMatrix(tm, m44);

	vcl::io::savePly(tm, VCL_TEST_RESULTS_PATH "/rotated_bunny.ply");

	m44.setZero();
	m44(0,0) = 1;
	m44(0,1) = 0.5;
	m44(0,3) = 1;

	m44(1,0) = 0.1;
	m44(1,1) = 1;
	m44(1,2) = -0.3;
	m44(1,3) = 2;

	m44(2,0) = 0.2;
	m44(2,2) = 1;

	m44(3,3) = 1;

	vcl::applyTransformMatrix(m, m44);

	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/obbrobrio.ply");

//	vcl::PolyMesh pm;

//	vcl::io::loadPly(pm, VCL_TEST_MODELS_PATH "/cube_poly.ply");

//	std::cerr << "========= PolyMesh =========\n\n";


	return 0;
}
