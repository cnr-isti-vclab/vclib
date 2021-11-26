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
#include <vclib/algorithms/update/bounding_box.h>
#include <vclib/algorithms/update/normal.h>
#include <vclib/algorithms/update/topology.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/mesh/mesh.h>
#include <vclib/misc/timer.h>

namespace mymesh {
class Vertex;
class Face;

class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,
			vcl::vert::Coordinate3d,
			vcl::vert::OptionalAdjacentFaces<Face, Vertex>>
{
};

class Face :
		public vcl::Face<
			vcl::face::TriangleBitFlags,
			vcl::face::TriangleVertexRefs<Vertex>,
			vcl::face::Normal3f>
{
};
} // namespace mymesh

class MyMesh : public vcl::Mesh<mymesh::Vertex, mymesh::Face, vcl::mesh::BoundingBox3d>
{
};

int main()
{
	MyMesh m = vcl::io::loadPly<MyMesh>("/home/alessandro/tmp/bunny.ply");

	m.enablePerVertexAdjacentFaces();
	vcl::updatePerVertexAdjacentFaces(m);
	vcl::updateBoundingBox(m);
	vcl::updatePerFaceNormals(m);

	MyMesh m2;

	vcl::swap(m, m2);

	vcl::updateBoundingBox(m2);

	vcl::Timer t("Laplacian Smoothing");
	vcl::vertexCoordLaplacianSmoothing(m2, 30);
	t.stopAndPrint();

	MyMesh m3;
	m3.addVertex();

	m3 = m2;

	vcl::io::savePly(m2, "/home/alessandro/tmp/bunny_s.ply");
	vcl::io::savePly(m3, "/home/alessandro/tmp/bunny_s3.ply");

	return 0;
}
