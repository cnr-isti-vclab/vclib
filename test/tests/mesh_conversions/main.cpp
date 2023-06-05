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

#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <catch2/catch_test_macros.hpp>

SCENARIO( "Mesh Conversions" ) {
	GIVEN( "The TextureDouble mesh loaded on TriMesh" ) {
		vcl::TriMesh tm = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/TextureDouble.ply");

		THEN( "The mesh has 8 vertices, 4 triangles, per face wedge texcoords" ) {
			REQUIRE( tm.vertexNumber() == 8 );
			REQUIRE( tm.faceNumber() == 4 );
			REQUIRE( tm.isPerFaceWedgeTexCoordsEnabled() );
		}

		vcl::PolyMesh pm;
		REQUIRE( !pm.isPerFaceWedgeTexCoordsEnabled() );
		pm.enableSameOptionalComponentsOf(tm);
		REQUIRE( pm.isPerFaceWedgeTexCoordsEnabled() );

		pm.importFrom(tm);

		THEN ("The imported PolyMesh has same vertex and face number") {
			REQUIRE( pm.vertexNumber() == 8 );
			REQUIRE( pm.faceNumber() == 4 );
		}

		THEN ("The imported vetices have same coordinates") {
			for (const auto& tv: tm.vertices()) {
				REQUIRE(pm.vertex(tv.index()).coord() == tv.coord());
			}
		}

		THEN ("The imported faces have same vertices") {
			for (const auto& pf: pm.faces()) {
				REQUIRE(pf.vertexNumber() == 3);
				const auto& tf = tm.face(pf.index());

				uint i = 0;
				for (const auto* pv : pf.vertices()) {
					REQUIRE(pv->coord() == tf.vertex(i)->coord());
					++i;
				}

				i = 0;
				for (const auto& pwt : pf.wedgeTexCoords()) {
					REQUIRE(pwt.u() == tf.wedgeTexCoord(i).u());
					REQUIRE(pwt.v() == tf.wedgeTexCoord(i).v());
					++i;
				}
			}
		}
	}

	GIVEN( "The polygonal cube mesh loaded on TriMesh" ) {
		vcl::TriMesh tm = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/cube_poly.ply");

		THEN ("The loaded TriMesh has 8 vertices and 12 faces") {
			REQUIRE( tm.vertexNumber() == 8 );
			REQUIRE( tm.faceNumber() == 12 );
		}
	}

	GIVEN( "The polygonal cube mesh loaded on PolyMesh" ) {
		vcl::PolyMesh pm = vcl::io::loadPly<vcl::PolyMesh>(VCL_TEST_MODELS_PATH "/cube_poly.ply");

		THEN ("The loaded PolyMesh has 8 vertices and 6 faces") {
			REQUIRE( pm.vertexNumber() == 8 );
			REQUIRE( pm.faceNumber() == 6 );
		}

		GIVEN ( "When importing into TriMesh" ) {
			vcl::TriMesh tm;

			tm.importFrom(pm);

			THEN ("The imported trimesh has same vertices") {
				REQUIRE(tm.vertexNumber() == pm.vertexNumber());

				for (uint i = 0; i < tm.vertexNumber(); ++i) {
					REQUIRE(tm.vertex(i).coord() == pm.vertex(i).coord());
				}
			}

			THEN ("The imported trimesh has 12 faces") {
				REQUIRE(tm.faceNumber() == 12);
			}
		}
	}
}

