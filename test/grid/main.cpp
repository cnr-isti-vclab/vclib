/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
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

#include <vclib/space/spatial_data_structures.h>

#include <vclib/tri_mesh.h>
#include <vclib/algorithms/create.h>

#include <vclib/iterators/pointer_iterator.h>

int main()
{
	vcl::Grid3<double> g(vcl::Point3d(0,0,0), vcl::Point3d(1,1,1), vcl::Point3<uint>(10, 10, 10));

	vcl::Point3<uint> first(2,2,2), last(5, 4, 7);

	for (const auto& c : g.cells(first, last))
		std::cerr << c << "\n";

	std::cerr << "\n";

	vcl::HashTableGrid3<vcl::Point<double, 3>, false> sht(g);

	sht.insert(vcl::Point3d(0.05, 0.15, 0.25));
	sht.insert(vcl::Point3d(0.05, 0.15, 0.25)); // duplicate won't be inserted
	sht.insert(vcl::Point3d(0.02, 0.12, 0.29));
	sht.insert(vcl::Point3d(0.12, 0.09, 0.32));

	sht.insert(vcl::Point3d(0.24, 0.52, 0.29));

	for (const auto& c : sht.nonEmptyCells()) {
		std::cerr << c << "\n";
	}

	std::cerr << "Values in HashTableGrid: \n";

	for (const auto& p : sht) {
		std::cerr << p.key << ": " << p.value << "\n";
	}

	std::cerr << "Values in cell 0, 1, 2: \n";

	auto p = sht.valuesInCell(vcl::Point3<uint>(0,1,2));
	for (auto& it = p.first; it != p.second; ++it) {
		std::cerr << it->value << "; ";
	}
	std::cerr << "\n";

//	sht.erase(vcl::Point3d(0.05, 0.15, 0.25));
//	p = sht.valuesInCell(vcl::Point3<uint>(0,1,2));
//	for (auto& it = p.first; it != p.second; ++it) {
//		std::cerr << it->second.get() << "; ";
//	}
//	std::cerr << "\n\n";

	auto set  = sht.valuesInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});

	std::cerr << "Values in sphere: \n";
	for (auto it : set) {
		std::cerr << it->value << "; ";
	}
	std::cerr << "\n\n";

	sht.eraseInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});

	std::cerr << "Values in HashTableGrid: \n";

	for (const auto& p : sht) {
		std::cerr << p.key << ": " << p.value << "\n";
	}

	vcl::TriMesh m = vcl::createHexahedron<vcl::TriMesh>();

//	vcl::HashTableGrid3<vcl::TriMesh::Vertex*> fsht(
//		vcl::PointerIterator<vcl::TriMesh::VertexIterator>(m.vertexBegin()),
//		vcl::PointerIterator<vcl::TriMesh::VertexIterator>(m.vertexEnd()));

	vcl::HashTableGrid3<vcl::TriMesh::Face&> fsht(m.faceBegin(), m.faceEnd());

	std::cerr << "Values in HashTableGrid: \n";

	for (const auto& p : fsht) {
		std::cerr << p.key << ": " << m.index(p.value) << "\n";
	}

	return 0;
}
