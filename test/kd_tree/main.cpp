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

#include <vclib/tri_mesh.h>
#include <vclib/io/load_ply.h>
#include <vclib/space/kd_tree.h>

int main()
{
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bone.ply");

	vcl::KDTree<vcl::Point3d> tree(m);

	uint pid = tree.nearestNeighborIndex(vcl::Point3d(0.5, 0.5, 0.5));

	assert(pid == 1558);

	std::vector<uint> res = tree.kNearestNeighborsIndices(vcl::Point3d(0.5, 0.5, 0.5), 5);

	assert(res[0] == 1558);
	assert(res[1] == 1613);
	assert(res[2] == 1720);
	assert(res[3] == 1576);
	assert(res[4] == 163);

	return 0;
}
