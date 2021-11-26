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

#include <vclib/polymesh.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/algorithms/update/flag.h>
#include <vclib/algorithms/smooth.h>

int main()
{
	vcl::PolyMesh m;

	m.addVertex();

	m.addFace();

	auto& f = m.face(0);

	f.pushVertex(&m.vertex(0));

	m.enablePerFaceAdjacentFaces();

	f.clearVertices();

	return 0;
}
