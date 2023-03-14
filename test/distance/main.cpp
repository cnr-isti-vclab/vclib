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

#include <vclib/mesh.h>
#include <vclib/io/load_obj.h>
#include <vclib/io/load_ply.h>
#include <vclib/algorithm.h>

int main()
{
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/cube_tri.ply");

	vcl::updatePerFaceNormals(m);

	vcl::Point3d p(2, 1, 0);
	vcl::Point3d c;

	for (const auto& f : m.faces()) {
		double dist = vcl::pointFaceDistance(p, f, c);

		std::cerr << "Face " << m.index(f) << ": \n";
		std::cerr << "\tdist: " << dist << ";\n";
		std::cerr << "\tclos: " << c << "\n";
	}

	vcl::TriMesh m1 = vcl::io::loadObj<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bimba.obj");
	vcl::TriMesh m2 = vcl::io::loadObj<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bunny.obj");

	vcl::updateBoundingBox(m1);
	vcl::updateBoundingBox(m2);
	auto res = vcl::hausdorffDistance(m1, m2, 20);

	std::cerr << "\n\nmin: " << res.minDist << ";\n";
	std::cerr << "max: " << res.maxDist << ";\n";
	std::cerr << "mean: " << res.meanDist << ";\n";
	std::cerr << "rms: " << res.RMSDist << ";\n";

	return 0;
}
