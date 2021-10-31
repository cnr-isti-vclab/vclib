/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/trimesh.h>
#include <mgp/io/load_ply.h>

int main()
{
	mgp::TriMesh m;

	m.addVertex();
	m.addVertex();
	m.addVertex();

	m.enablePerVertexMutableFlags();

	m.vertex(0).coordinate() = mgp::Point3d(0, 0, 0);
	m.vertex(1).coordinate() = mgp::Point3d(0, 1, 0);
	m.vertex(2).coordinate() = mgp::Point3d(1, 0, 0);

	const mgp::TriMesh::Vertex& vv = m.vertex(0);
	vv.setVisitedM();

	assert(m.vertex(0).isVisitedM());
	
	m.vertex(0).normal() = mgp::Point3d(1, 0, 0);

	m.vertex(0).color() = mgp::Color(3, 7, 4);
	m.vertex(1).color() = mgp::Color(3, 63, 44);
	m.vertex(2).color() = mgp::Color(3, 9, 2);
	
	m.addPerVertexCustomComponent<char>("prova");
	
	for (auto& v : m.vertexIterator())
		v.customComponent<char>("prova") = 'a';
	
	m.enablePerFaceScalar();

	m.addFace();
	m.addFace();
	m.addFace();
	m.addFace();
	m.addFace();

	m.enablePerFaceColor();

	m.face(0).setVertices({&m.vertex(0), &m.vertex(1), &m.vertex(2)});

	unsigned int i = 17;
	for (mgp::TriMesh::Face& f : m.faceIterator())
		f.color() = mgp::Color(4+2*i++, 134, 98);

	m.addVertex();
	m.addVertex();
	m.addVertex();

	for (const mgp::TriMesh::Face& f : m.faceIterator())
		std::cerr << f.id() << " Color: " << f.color() << "\n";

	std::cerr << "VN: " << m.vertexNumber() << "\n";

	//iterate over vertices
	for (const mgp::TriMesh::Vertex& v : m.vertexIterator()){
		std::cerr << "V " << v.id() << "\n";
	}

	return 0;
}
