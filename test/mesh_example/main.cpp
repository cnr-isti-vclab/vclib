/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/trimesh.h>

int main()
{

	mgp::TriMesh m;

	m.addVertex();
	m.addVertex();
	m.addVertex();

	m.vertex(0).position() = mgp::Point3d(0,0,0);
	m.vertex(1).position() = mgp::Point3d(0,1,0);
	m.vertex(2).position() = mgp::Point3d(1,0,0);

	m.enableVertexColor();
	m.vertex(0).color() = mgp::Color(3, 7, 4);
	m.vertex(1).color() = mgp::Color(3, 63, 44);
	m.vertex(2).color() = mgp::Color(3, 9, 2);

	m.addFace();

	m.face(0).setVertices({&m.vertex(0), &m.vertex(1), &m.vertex(2)});

	m.enableFaceColor();

	m.face(0).color() = mgp::Color(255, 134, 98);

	m.addVertex();
	m.addVertex();
	m.addVertex();

	std::cerr << m.face(0).color() << "\n";

	std::cerr << "VN: " << m.vertexNumber() << "\n";

	return 0;
}
