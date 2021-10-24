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

	m.enableVertexMutableBitFlags();

	m.vertex(0).position() = mgp::Point3d(0, 0, 0);
	m.vertex(1).position() = mgp::Point3d(0, 1, 0);
	m.vertex(2).position() = mgp::Point3d(1, 0, 0);

	const mgp::TriMesh::Vertex& vv = m.vertex(0);
	vv.setSelectedM();

	assert(m.vertex(0).isSelectedM());
	
	m.vertex(0).normal() = mgp::Point3d(1, 0, 0);

	m.vertex(0).color() = mgp::Color(3, 7, 4);
	m.vertex(1).color() = mgp::Color(3, 63, 44);
	m.vertex(2).color() = mgp::Color(3, 9, 2);
	
	m.enableFaceScalar();
	m.enableFaceColor();

	m.addFace();

	m.face(0).setVertices({&m.vertex(0), &m.vertex(1), &m.vertex(2)});

	m.face(0).color() = mgp::Color(255, 134, 98);

	m.addVertex();
	m.addVertex();
	m.addVertex();

	std::cerr << m.face(0).color() << "\n";

	std::cerr << "VN: " << m.vertexNumber() << "\n";

	//iterate over vertices
	for (const mgp::TriMesh::Vertex& v : m.vertexIterator()){
		std::cerr << "V " << v.id() << "\n";
	}

	return 0;
}
