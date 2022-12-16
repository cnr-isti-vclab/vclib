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

#include <QApplication>

#include <vclib/algorithms/update/normal.h>
#include <vclib/algorithms/update/color.h>
#include <vclib/tri_mesh.h>
#include <vclib/io/load_ply.h>

#include <vclib/ext/opengl2/drawable_mesh.h>
#include <vclib/ext/qglviewer/viewer.h>

int main(int argc, char **argv) {
	// Read command lines arguments.
	QApplication application(argc, argv);

	// Instantiate the viewer.
	vcl::Viewer viewer;

	viewer.setWindowTitle("simpleViewer");

	vcl::io::FileMeshInfo loadedInfo;
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/brain.ply", loadedInfo);

	vcl::updatePerFaceNormals(m);
	vcl::updatePerVertexNormals(m);
	vcl::setPerVertexColor(m, vcl::Color::White);

	vcl::MeshRenderSettings mrs;
	vcl::DrawableMesh<vcl::TriMesh> dtm(m, mrs);

	viewer.pushDrawableObject(dtm);
	viewer.fitScene();

	// Make the viewer window visible on screen.
	viewer.show();

	// Run main loop.
	return application.exec();
}
