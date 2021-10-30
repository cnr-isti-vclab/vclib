/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/trimesh.h>
#include <mgp/io/load_ply.h>
#include <mgp/io/save_ply.h>
#include <mgp/algorithms/update/flag.h>
#include <mgp/algorithms/smooth.h>

int main()
{
	mgp::TriMesh m;

	mgp::requirePerVertexCoordinate(m);
	mgp::requireTriangleMesh(m);
	mgp::requirePerVertexNormal(m);

	mgp::io::loadPly(m, "/home/alessandro/tmp/bunny.ply");

	mgp::vertexCoordLaplacianSmoothing(m, 50);

	mgp::io::savePly(m, "/home/alessandro/tmp/bunny_s.ply");

	return 0;
}
