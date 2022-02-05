/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include <vclib/mesh/mesh.h>

class MyVertex;
class MyFace;
class MyHalfEdge;

class MyVertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,
			vcl::vert::Coordinate3d,
			vcl::vert::HalfEdgeReference<MyHalfEdge>>
{};

class MyHalfEdge :
		public vcl::HalfEdge<
			vcl::hedge::BitFlags,
			vcl::hedge::HalfEdgeReferences<MyHalfEdge, MyVertex, MyFace>>
{};

class MyFace :
			public vcl::Face<
				vcl::face::BitFlags,
				vcl::face::HalfEdgeReference<MyHalfEdge>>
{};

int main()
{
	MyVertex v0, v1, v2;
	MyHalfEdge e0, e1, e2;
	MyFace f;

	v0.coord() = vcl::Point3d(0,0,0);
	v1.coord() = vcl::Point3d(1,1,1);
	v2.coord() = vcl::Point3d(2,2,2);

	e0.fromVertex() = &v0;
	e0.toVertex() = &v1;
	e1.fromVertex() = &v1;
	e1.toVertex() = &v2;
	e2.fromVertex() = &v2;
	e2.toVertex() = &v0;

	e0.next() = &e1;
	e1.next() = &e2;
	e2.next() = &e0;
	e0.prev() = &e2;
	e1.prev() = &e0;
	e2.prev() = &e1;

	f.outerHalfEdge() = &e0;

	for (const MyVertex* v : f.vertices()) {
		std::cerr << v->coord() << "\n";
	}

	return 0;
}
