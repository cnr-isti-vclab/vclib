/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/trimesh.h>

int main()
{

	mgp::TriMesh m;
	mgp::trimesh::Vertex v0, v1, v2;

	std::cerr << "Coord : " << v1.position() << "\n";
	std::cerr << "Normal: " << v1.normal() << "\n";
	std::cerr << "Color : " << v1.color() << "\n";

	mgp::trimesh::Face f;
	f.setVertexReference(&v0, 0);
	f.setVertexReference(&v1, 1);
	f.setVertexReference(&v2, 2);

	v0.addFaceAdjacency(&f);

	return 0;
}
