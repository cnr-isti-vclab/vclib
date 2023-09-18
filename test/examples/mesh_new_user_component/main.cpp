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

#include "foo_mesh.h"

#include <vclib/meshes/tri_mesh.h>

int main(int argc, char **argv)
{

	FooMesh m;

	m.addVertices(10);

	for (auto& v : m.vertices()) {
		v.foo() = m.index(v) + 42;
	}

	vcl::TriMesh tm;

	// when importing in a trimesh, the foo component is not copied
	// because the trimesh does not have the component in its vertices
	tm.importFrom(m);

	// reverse importFrom, to assert that everything builds correctly
	m.importFrom(tm);
}
