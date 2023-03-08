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
#include <vclib/algorithm.h>

int main()
{
	vcl::TriMesh m = vcl::io::loadObj<vcl::TriMesh>("/home/alessandro/Drive/Research/3DMeshes/cube.obj");

	vcl::updatePerFaceNormals(m);

	vcl::Point3d p(2, 1, 0);
	vcl::Point3d c;

	for (const auto& f : m.faces()) {
		double dist = vcl::pointFaceDistance(p, f, c);

		std::cerr << "Face " << m.index(f) << ": \n";
		std::cerr << "\tdist: " << dist << ";\n";
		std::cerr << "\tclos: " << c << "\n";
	}

	return 0;
}
