/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/algorithms/smooth.h>
#include <mgp/io/load_ply.h>
#include <mgp/io/save_ply.h>
#include <mgp/mesh/mesh.h>
#include <mgp/misc/timer.h>

namespace mymesh {
class Vertex;
class Face;

class Vertex : public mgp::Vertex<mgp::vert::BitFlags, mgp::vert::Coordinate3d>
{
};

class Face :
		public mgp::Face<
			mgp::face::TriangleBitFlags,
			mgp::face::TriangleVertexRefs<Vertex>>
{
};
} // namespace mymesh

class MyMesh : public mgp::Mesh<mymesh::Vertex, mymesh::Face>
{
public:
	using Vertex = mymesh::Vertex;
	using Face   = mymesh::Face;
};

int main()
{
	MyMesh m;

	mgp::io::loadPly(m, "/home/alessandro/tmp/bunny.ply");

	mgp::Timer t("Laplacian Smoothing");
	mgp::vertexCoordLaplacianSmoothing(m, 500);
	t.stopAndPrint();

	mgp::io::savePly(m, "/home/alessandro/tmp/bunny_s.ply");

	return 0;
}
