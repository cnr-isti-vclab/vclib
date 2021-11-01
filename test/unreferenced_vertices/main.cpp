/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/algorithms/clean.h>
#include <mgp/io/load_ply.h>
#include <mgp/io/save_ply.h>
#include <mgp/trimesh.h>
#include <mgp/misc/timer.h>

int main()
{
	mgp::TriMesh m;

	mgp::io::loadPly(m, "/home/alessandro/tmp/bunny_rem.ply");

	unsigned int nv = mgp::removeUnreferencedVertices(m);

	std::cerr << "Removed Vertices: " << nv << "\n";

	// note: without compacting, ply exporter won't work
	m.compactVertices();
	m.compactFaces();

	mgp::io::savePly(m, "/home/alessandro/tmp/bunny_remrem.ply");

	return 0;
}
