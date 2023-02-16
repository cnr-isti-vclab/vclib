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

#include <vclib/algorithms/update/transform.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/tri_mesh.h>
#include <vclib/poly_mesh.h>

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

	return 0;
}
