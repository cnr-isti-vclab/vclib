/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include <iostream>

#include <mgp/trimesh.h>
#include <mgp/io/load_ply.h>
#include <mgp/io/save_ply.h>
#include <mgp/algorithms/create/tetrahedron.h>

int main()
{
	mgp::TriMesh m;

	// adding some vertices
	m.addVertex();
	m.addVertex();
	m.addVertex();

	// setting coordinate
	m.vertex(0).coordinate() = mgp::Point3d(0, 0, 0);
	m.vertex(1).coordinate() = mgp::Point3d(0, 1, 0);
	m.vertex(2).coordinate() = mgp::Point3d(1, 0, 0);

	// test mutable flags
	// flags that can be modified also on const meshes, like the "visited" flag
	m.enablePerVertexMutableBitFlags(); // mutable flags on this mesh is an optional component

	const mgp::TriMesh::Vertex& vv = m.vertex(0);
	vv.setVisitedM(); // vertex 0 is now visited

	assert(m.vertex(0).isVisitedM());

	m.enablePerVertexTexCoord();

	m.vertex(0).texCoord().set(0.1, 0.2);
	
	// setting vertex normals
	// in this mesh vertex normaks are not optional, I don't need to enable them
	m.vertex(0).normal() = mgp::Point3d(1, 0, 0);

	// setting vertex colors, also colors here are not optional
	m.vertex(0).color() = mgp::Color(3, 7, 4);
	m.vertex(1).color() = mgp::Color(3, 63, 44);
	m.vertex(2).color() = mgp::Color(3, 9, 2);
	
	// adding a per vertex custom component of chars, called 'prova'
	m.addPerVertexCustomComponent<char>("prova");
	
	for (auto& v : m.vertexIterator())
		v.customComponent<char>("prova") = 'a'; // set the custom component for each vertex
	
	// enable per face scalar, which is optional
	m.enablePerFaceScalar();

	// add some faces
	m.addFace();
	m.addFace();
	m.addFace();
	m.addFace();
	m.addFace();

	// enable face colors after faces are added
	m.enablePerFaceColor();

	// set vertices of f0
	m.face(0).setVertices<mgp::trimesh::Vertex>({&m.vertex(0), &m.vertex(1), &m.vertex(2)});
	m.face(0).scalar() = 4;

	// setting some colors to all the faces of the mesh
	unsigned int i = 17;
	for (mgp::TriMesh::Face& f : m.faceIterator()) // iterates over all the non-deleted faces of m
		f.color() = mgp::Color(4+2*i++, 134, 98);

	// add some other vertices
	m.addVertex();
	m.addVertex();
	m.addVertex();

	std::cerr << "f0 scalar: " << m.face(0).scalar() << "\n";
	for (const mgp::TriMesh::Face& f : m.faceIterator())
		std::cerr << f.id() << " Color: " << f.color() << "\n";

	std::cerr << "VN: " << m.vertexNumber() << "\n";

	//iterate over vertices
	for (const mgp::TriMesh::Vertex& v : m.vertexIterator()){
		std::cerr << "V " << v.id() << "\n";
	}

	mgp::TriMesh tet = mgp::createTetrahedron<mgp::TriMesh>();

	mgp::io::savePly(tet, "/home/alessandro/tmp/tet.ply");

	return 0;
}
