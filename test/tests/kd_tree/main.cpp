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

#include <vclib/meshes.h>
#include <vclib/load_save.h>
#include <vclib/space/kd_tree.h>

#include <catch2/catch_test_macros.hpp>

using PointType = vcl::TriMesh::VertexType::CoordType;

std::vector<uint> getKNearestNeighbors(const PointType& p, uint k, std::string mesh = "bone.ply")
{
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/" + mesh);

	vcl::KDTree tree(m);

	return tree.kNearestNeighborsIndices(p, k);
}

static const PointType p(0.5, 0.5, 0.5);

TEST_CASE("KD-Tree nearest neighbors to [0.5, 0.5, 0.5] in bone.ply") {
	REQUIRE(getKNearestNeighbors(p, 1)[0] == 1558);
	REQUIRE(getKNearestNeighbors(p, 5) == std::vector<uint>{1558, 1613, 1720, 1576, 163});
}
