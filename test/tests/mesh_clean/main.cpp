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

#include <vclib/algorithms.h>
#include <vclib/meshes.h>
#include <catch2/catch_test_macros.hpp>

template<vcl::FaceMeshConcept MeshType>
void populateTriMesh(MeshType& tm)
{
	// note: p3 and p4 have same coords
	const vcl::Point3d p0(0,0,0);
	const vcl::Point3d p1(1,0,0);
	const vcl::Point3d p2(0,1,0);
	const vcl::Point3d p3(1,1,0);
	const vcl::Point3d p4(1,1,0);

	tm.addVertices(p0, p1, p2, p3, p4);

	tm.addFace(0, 1, 2);
	tm.addFace(1, 2, 0); // dup of 0
	tm.addFace(1, 2, 0); // dup of 0
	tm.addFace(3, 1, 0);
	tm.addFace(1, 3, 0); // dup of 3
	tm.addFace(1, 4, 0); // not dup of 3 (different coordinates)
	tm.addFace(2, 1, 0); // dup of 0
	tm.addFace(0, 1, 2); // dup of 0
}

template<vcl::FaceMeshConcept MeshType>
void populatePolyMesh(MeshType& pm)
{
	// note: p3 and p4 have same coords
	const vcl::Point3d p0(0,0,0);
	const vcl::Point3d p1(1,0,0);
	const vcl::Point3d p2(0,1,0);
	const vcl::Point3d p3(1,1,0);
	const vcl::Point3d p4(1,1,0);

	pm.addVertices(p0, p1, p2, p3, p4);

	pm.addFace(0, 1, 2);
	pm.addFace(0, 1, 2, 3);
	pm.addFace(0, 1, 2, 4); // not dup of 1 (different coordinates)
	pm.addFace(0, 2, 1, 3); // dup of 1
	pm.addFace(4, 1, 2, 0); // dup of 2
	pm.addFace(0, 2, 1); // dup of 0

}

TEST_CASE("Clean Duplicated Faces") {

	SECTION( "TriMesh" ) {
		vcl::TriMesh tm;

		populateTriMesh(tm);

		REQUIRE( tm.vertexNumber() == 5 );
		REQUIRE( tm.faceNumber() == 8 );

		uint nr = vcl::removeDuplicatedFaces(tm);
		REQUIRE( nr == 5 );
		REQUIRE( tm.vertexNumber() == 5 );
		REQUIRE( tm.faceNumber() == 3 );
	}

	SECTION( "PolyMesh with triangles" ) {
		vcl::PolyMesh pm;

		populateTriMesh(pm);

		REQUIRE(pm.vertexNumber() == 5 );
		REQUIRE(pm.faceNumber() == 8 );

		uint nr = vcl::removeDuplicatedFaces(pm);
		REQUIRE( nr == 5 );
		REQUIRE(pm.vertexNumber() == 5 );
		REQUIRE(pm.faceNumber() == 3 );
	}

	SECTION( "PolyMesh with polygons" ) {
		vcl::PolyMesh pm;

		populatePolyMesh(pm);

		REQUIRE( pm.vertexNumber() == 5 );
		REQUIRE( pm.faceNumber() == 6 );

		uint nr = vcl::removeDuplicatedFaces(pm);

		REQUIRE( nr == 3 );
		REQUIRE( pm.vertexNumber() == 5 );
		REQUIRE( pm.faceNumber() == 3 );
	}
}


