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
#include <vclib/load_save.h>

#include <vclib/mesh/iterator.h>
#include <vclib/mesh/views/elements.h>

int main()
{
	vcl::TriMesh m = vcl::load<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/cube_tri.ply");

	// print

	const vcl::TriMesh& cm = m;

	for (auto& v : m | vcl::views::vertices) {
		std::cerr << v.coord() << "\n";
	}

	for (const auto& f : cm | vcl::views::faces) {
		std::cerr << f.normal() << "\n";
	}

	for (const auto& c : vcl::CoordView(cm.vertices())) {
		std::cerr << c << "\n";
	}

	// transform
	std::cerr << "\n\nTransformed:\n\n";

	for (auto& c : vcl::CoordView(m.vertices())) {
		c *= 2;
		std::cerr << c << "\n";
	}

	std::cerr << "\n\nTransform Selection:\n";

	uint i = 0;
//	for (auto& v : m.vertices()) {
//		v.selected() = i % 2 ? true : false;
//		i++;
//	}
	for (auto& sel : vcl::SelectionView(m.vertices())) {
		sel = i % 2 ? true : false;
		std::cerr << sel << "\n";
		++i;
	}

	std::cerr << "\n\nPrint Selection:\n";
	for (const auto& sel : vcl::SelectionView(cm.vertices())) {
		std::cerr << sel << "\n";
	}


	// scalar

	i = 0;
	for (auto& scal : vcl::ScalarView(m.vertices())) {
		scal = ++i;
	}

	std::cerr << "\n\nPrint Scalars:\n";

	for (const auto& scal : vcl::ScalarView(cm.vertices())) {
		std::cerr << scal << "\n";
	}
	return 0;
}
