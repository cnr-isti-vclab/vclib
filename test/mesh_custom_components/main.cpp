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

#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/trimesh.h>
#include <vclib/algorithms/smooth.h>

int main()
{
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bone.ply");

	m.addPerVertexCustomComponent<int>("flag");

	assert(m.hasPerVertexCustomComponent("flag"));

	for (vcl::TriMesh::Vertex& v : m.vertices()){
		v.customComponent<int>("flag") = -4;
	}

	assert(m.vertex(10).customComponent<int>("flag") == -4);

	vcl::TriMesh::CustomComponentVectorHandle<int> v = m.customComponentVectorHandle<int>("flag");

	for (int& l : v){
		l = 8;
	}

	assert(m.vertex(9).customComponent<int>("flag") == 8);

	m.deletePerVertexCustomComponent("flag");

	assert(!m.hasPerVertexCustomComponent("flag"));

	m.addPerVertexCustomComponent<vcl::Point3f>("oldCoords");
	for (vcl::TriMesh::Vertex& v : m.vertices()) {
		v.customComponent<vcl::Point3f>("oldCoords") = v.coord();
	}

	vcl::taubinSmoothing(m, 500, 0.7, -0.73);

	double avgDist = 0;
	for (vcl::TriMesh::Vertex& v : m.vertices()) {
		avgDist += v.coord().dist(v.customComponent<vcl::Point3f>("oldCoords"));
	}
	avgDist /= m.vertexNumber();

	std::cerr << "Avg distance after taubin smoothing: " << avgDist << "\n";

	return 0;
}
