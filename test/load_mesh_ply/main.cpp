/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/polymesh.h>
#include <mgp/io/load_ply.h>
#include <mgp/io/save_ply.h>
#include <mgp/algorithms/update/flag.h>
#include <mgp/algorithms/smooth.h>

int main()
{
	mgp::PolyMesh m;

	m.addVertex();

	m.addFace();

	auto& f = m.face(0);

	f.pushVertex(&m.vertex(0));

	m.enablePerFaceAdjacentFaces();

	f.clearVertices();

	return 0;
}
