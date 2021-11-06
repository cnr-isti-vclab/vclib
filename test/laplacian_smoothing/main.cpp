/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/algorithms/smooth.h>
#include <mgp/algorithms/update/bounding_box.h>
#include <mgp/algorithms/update/topology.h>
#include <mgp/io/load_ply.h>
#include <mgp/io/save_ply.h>
#include <mgp/mesh/mesh.h>
#include <mgp/misc/timer.h>

namespace mymesh {
class Vertex;
class Face;

class Vertex :
		public mgp::Vertex<
			mgp::vert::BitFlags,
			mgp::vert::Coordinate3d,
			mgp::vert::OptionalAdjacentFaces<Face, Vertex>>
{
};

class Face : public mgp::Face<mgp::face::TriangleBitFlags, mgp::face::TriangleVertexRefs<Vertex>>
{
};
} // namespace mymesh

class MyMesh : public mgp::Mesh<mymesh::Vertex, mymesh::Face, mgp::mesh::BoundingBox3d>
{
public:
	using Vertex = mymesh::Vertex;
	using Face   = mymesh::Face;
};

int main()
{
	MyMesh m;

	mgp::io::loadPly(m, "/home/alessandro/tmp/bunny.ply");

	m.enablePerVertexAdjacentFaces();
	mgp::updatePerVertexAdjacentFaces(m);

	MyMesh m2(m);

	mgp::updateBoundingBox(m2);

	mgp::Timer t("Laplacian Smoothing");
	mgp::vertexCoordLaplacianSmoothing(m2, 30);
	t.stopAndPrint();

	mgp::io::savePly(m2, "/home/alessandro/tmp/bunny_s.ply");

	return 0;
}
