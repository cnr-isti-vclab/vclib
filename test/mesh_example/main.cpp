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

	m.addVertex();
	m.addVertex();
	m.addVertex();

	m.vertex(1)->position() = mgp::Point3d(3,4,5);

	return 0;
}
