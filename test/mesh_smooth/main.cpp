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

#include <vclib/algorithms/smooth.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/io/save_obj.h>
#include <vclib/tri_mesh.h>
#include <vclib/misc/timer.h>

int main()
{
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bunny_textured.ply");

	vcl::TriMesh mSmooth(m);

	vcl::Timer t1("Laplacian Smoothing");

	vcl::laplacianSmoothing(mSmooth, 30);
	t1.stopAndPrint();

	vcl::io::savePly(mSmooth, VCL_TEST_RESULTS_PATH "/bunny_lapl_smooth.ply");

	vcl::Timer t2("Taubin Smoothing");

	vcl::taubinSmoothing(m, 300, 0.5, -0.53);
	t2.stopAndPrint();

	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/bunny_taub_smooth.ply");

	return 0;
}
