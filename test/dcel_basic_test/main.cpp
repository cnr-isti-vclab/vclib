/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <iostream>

#include <vclib/dcel_mesh.h>
#include <vclib/tri_mesh.h>
#include <vclib/algorithms/create/tetrahedron.h>
#include <vclib/io/save.h>
#include "vclib/io/load.h"

int main()
{
	vcl::DcelMesh m1;
	{
		vcl::DcelMesh m;

		m.addVertices(vcl::Point3d(0,0,0), vcl::Point3d(1,1,1), vcl::Point3d(2,2,2));
		m.addFace(0, 1, 2);

		m.enablePerFaceWedgeColors();
		m.face(0).wedgeColor(0) = vcl::Color::Red;
		m.face(0).wedgeColor(1) = vcl::Color::Green;
		m.face(0).wedgeColor(2) = vcl::Color::Blue;

		assert(m.face(0).wedgeColor(0) == vcl::Color::Red);
		assert(m.face(0).wedgeColor(0) != vcl::Color::Cyan);
		assert(m.face(0).wedgeColor(1) == vcl::Color::Green);
		assert(m.face(0).wedgeColor(2) == vcl::Color::Blue);
		assert(m.face(0).wedgeColor(0) == m.halfEdge(0).color());
		assert(m.face(0).wedgeColor(1) == m.halfEdge(1).color());
		assert(m.face(0).wedgeColor(2) == m.halfEdge(2).color());

		m.enablePerFaceWedgeTexCoords();
		m.face(0).wedgeTexCoord(0) = vcl::TexCoordd(1, 3);

		assert(m.face(0).wedgeTexCoord(0).u() == 1);
		assert(m.face(0).wedgeTexCoord(0).v() == 3);

		for (const vcl::DcelMesh::Vertex* v : m.face(0).vertices()) {
			std::cerr << v->coord() << "\n";
		}

		vcl::DcelMesh t = vcl::createTetrahedron<vcl::DcelMesh>();
		vcl::io::save(t, VCL_TEST_RESULTS_PATH "/tet_dcel.ply");

		vcl::DcelMesh l = vcl::io::loadPly<vcl::DcelMesh>(VCL_TEST_DATA_PATH "/models/cube_tri.ply");

		for (const vcl::DcelMesh::Face& f : l.faces()) {
			std::cerr << "Face " << l.index(f) << ":\n\t";
			for (const vcl::DcelMesh::Vertex* v : f.vertices()) {
				std::cerr << l.index(v) << "; ";
			}
			std::cerr << "\n";
		}


		m1.importFrom(l);
	}

	std::cerr << "----- Imported dcel -----\n";
	for (const vcl::DcelMesh::Face& f : m1.faces()) {
		std::cerr << "Face " << m1.index(f) << ":\n\t";
		for (const vcl::DcelMesh::Vertex* v : f.vertices()) {
			std::cerr << m1.index(v) << "; ";
		}
		std::cerr << "\n";
	}

	vcl::TriMesh t;
	t.importFrom(m1);

	std::cerr << "----- Imported trimesh -----\n";
	for (const vcl::TriMesh::Face& f : t.faces()) {
		std::cerr << "Face " << t.index(f) << ":\n\t";
		for (const vcl::TriMesh::Vertex* v : f.vertices()) {
			std::cerr << t.index(v) << "; ";
		}
		std::cerr << "\n";
	}

	vcl::DcelMesh m2;
	m2.importFrom(t);

	std::cerr << "----- Imported dcel -----\n";
	for (const vcl::DcelMesh::Face& f : m2.faces()) {
		std::cerr << "Face " << m2.index(f) << ":\n\t";
		for (const vcl::DcelMesh::Vertex* v : f.vertices()) {
			std::cerr << m2.index(v) << "; ";
		}
		std::cerr << "\n";
	}

	return 0;
}
