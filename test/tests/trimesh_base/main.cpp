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

#include <vclib/meshes.h>
#include <catch2/catch_test_macros.hpp>

SCENARIO( "TriMesh usage" ) {
	GIVEN( "An empty TriMesh" ) {
		using TriMeshPoint = vcl::TriMesh::VertexType::CoordType;
		vcl::TriMesh m;

		THEN( "The size for each container start at 0" ) {
			REQUIRE( m.vertexNumber() == 0 );
			REQUIRE( m.faceNumber() == 0 );
		}

		THEN( "The optional components are all disabled") {
			REQUIRE( m.isPerVertexAdjacentFacesEnabled() == false);
			REQUIRE( m.isPerVertexAdjacentVerticesEnabled() == false);
			REQUIRE( m.isPerVertexPrincipalCurvatureEnabled() == false);
			REQUIRE( m.isPerVertexTexCoordEnabled() == false);
			REQUIRE( m.isPerVertexMarkEnabled() == false);
			REQUIRE( m.perVertexCustomComponentNames().size() == 0);
			REQUIRE( m.isPerFaceQualityEnabled() == false);
			REQUIRE( m.isPerFaceColorEnabled() == false);
			REQUIRE( m.isPerFaceAdjacentFacesEnabled() == false);
			REQUIRE( m.isPerFaceWedgeTexCoordsEnabled() == false);
			REQUIRE( m.isPerFaceMarkEnabled() == false);
			REQUIRE( m.perFaceCustomComponentNames().size() == 0);
		}

		WHEN( "Adding and removing vertices" ) {
			m.addVertex();

			THEN( "The vertex size changes" ) {
				REQUIRE( m.vertexNumber() == 1 );
				REQUIRE( m.faceNumber() == 0 );
			}
			THEN( "The vertex coordinate is 0,0,0" ) {
				REQUIRE( m.vertex(0).coord() == TriMeshPoint(0,0,0) );
			}
		}
	}
}

