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

#include <vclib/algorithms/clean.h>
#include <vclib/algorithms/update/topology.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/trimesh.h>
#include <vclib/misc/timer.h>

int main()
{
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/brain.ply");

	bool isWaterTight = vcl::isWaterTight(m);

	assert(!isWaterTight);

	std::cerr << "Is Water Tight: " << isWaterTight << "\n";

	m.enablePerFaceAdjacentFaces();
	vcl::updatePerFaceAdjacentFaces(m);

	uint nm = vcl::countNonManifoldVertices(m);

	assert(nm == 4);

	std::cerr << "Non Manifold Vertices: " << nm << "\n";

	uint nv = vcl::removeUnreferencedVertices(m);

	assert(nv == 0);

	std::cerr << "Removed Unreferenced Vertices: " << nv << "\n";

	nv = vcl::removeDuplicatedVertices(m);

	assert(nv == 453);

	std::cerr << "Removed Duplicated Vertices: " << nv << "\n";

	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/brain_clean.ply");

	m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bunny_textured.ply");

	m.enablePerFaceAdjacentFaces();
	vcl::updatePerFaceAdjacentFaces(m);
	uint nHoles = vcl::numberHoles(m);

	assert(nHoles == 5);

	std::cerr << "Bunny number holes: " << nHoles << "\n";

	return 0;
}
