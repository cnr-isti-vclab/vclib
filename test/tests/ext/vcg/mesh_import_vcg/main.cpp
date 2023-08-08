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

#include <vclib/ext/vcg/import.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

#include <wrap/io_trimesh/import_ply.h>

#include "mesh.h"

TEST_CASE( "Import TriMesh from VCG" ) {
	VCGMesh vcgMesh;

	vcg::tri::io::ImporterPLY<VCGMesh>::Open(
		vcgMesh, VCL_TEST_MODELS_PATH "/cube_tri.ply");

	REQUIRE(vcgMesh.VN() == 8);
	REQUIRE(vcgMesh.FN() == 12);

	vcl::TriMesh tm = vcl::meshFromVCGMesh<vcl::TriMesh>(vcgMesh);

	REQUIRE(tm.vertexNumber() == 8);
	REQUIRE(tm.faceNumber() == 12);

	for (uint fi = 0; fi < tm.faceNumber(); ++fi) {
		const auto& f = tm.face(fi);
		const auto& vcgf = vcgMesh.face[fi];
		for (uint vi = 0; vi < 3; ++vi) {
			REQUIRE(
				tm.index(f.vertex(vi)) == vcg::tri::Index(vcgMesh, vcgf.V(vi)));
		}
	}
}

TEST_CASE( "Import PolyMesh from VCG" ) {
	VCGMesh vcgMesh;

	vcg::tri::io::ImporterPLY<VCGMesh>::Open(
		vcgMesh, VCL_TEST_MODELS_PATH "/cube_tri.ply");

	REQUIRE(vcgMesh.VN() == 8);
	REQUIRE(vcgMesh.FN() == 12);

	vcl::PolyMesh pm = vcl::meshFromVCGMesh<vcl::PolyMesh>(vcgMesh);

	REQUIRE(pm.vertexNumber() == 8);
	REQUIRE(pm.faceNumber() == 12);

	for (uint fi = 0; fi < pm.faceNumber(); ++fi) {
		const auto& f = pm.face(fi);
		const auto& vcgf = vcgMesh.face[fi];

		REQUIRE(f.vertexNumber() == 3);
		for (uint vi = 0; vi < 3; ++vi) {
			REQUIRE(
				pm.index(f.vertex(vi)) == vcg::tri::Index(vcgMesh, vcgf.V(vi)));
		}
	}
}
