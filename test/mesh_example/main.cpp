/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/mesh/vertex.h>
#include <mgp/mesh/face.h>

class MyFace;
class MyVertex;

int main()
{
	typedef mgp::Vertex<
		mgp::vert::Position3d,
		mgp::vert::Normal3d,
		mgp::vert::Color> MyVertex;

	typedef mgp::Face<
		mgp::face::VertexRefArray<MyVertex, 3>> MyFace;

	MyVertex v0, v1, v2;

	std::cerr << "Coord : " << v1.position() << "\n";
	std::cerr << "Normal: " << v1.normal() << "\n";
	std::cerr << "Color : " << v1.color() << "\n";

	MyFace f;
	f.setVertexReference(&v0, 0);
	f.setVertexReference(&v1, 1);
	f.setVertexReference(&v2, 2);

	return 0;
}
