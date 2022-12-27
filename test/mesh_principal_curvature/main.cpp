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

#include <vclib/algorithms/update/topology.h>
#include <vclib/algorithms/update/curvature.h>
#include <vclib/algorithms/update/scalar.h>
#include <vclib/algorithms/update/color.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/load_obj.h>
#include <vclib/io/save_ply.h>
#include <vclib/tri_mesh.h>
#include <vclib/misc/timer.h>

#ifdef VCLIB_WITH_QGLVIEWER
#include <QApplication>

#include <vclib/ext/opengl2/drawable_mesh.h>
#include <vclib/ext/qglviewer/viewer_main_window.h>
#endif

int main(int argc, char **argv)
{
	vcl::TriMesh m = vcl::io::loadObj<vcl::TriMesh>("/home/alessandro/Repositories/Projects/vclib/test/data/models/bimba.obj");

	m.enablePerVertexAdjacentFaces();
	m.enablePerFaceAdjacentFaces();
	m.enablePerVertexPrincipalCurvature();

	vcl::updatePerFaceAdjacentFaces(m);
	vcl::updatePerVertexAdjacentFaces(m);

	vcl::updatePrincipalCurvature(m);

	vcl::setPerVertexScalarFromGaussianCurvatureDir(m);
	vcl::setPerVertexColorFromScalar(m);

	vcl::io::savePly(m, "/home/alessandro/tmp/bimba_curvature.ply");

#ifdef VCLIB_WITH_QGLVIEWER
	QApplication application(argc, argv);

	vcl::ViewerMainWindow viewer;
	vcl::DrawableMesh<vcl::TriMesh> dm(m);

	std::shared_ptr<vcl::DrawableObjectVector> vector = std::make_shared<vcl::DrawableObjectVector>();
	vector->pushBack(dm);
	viewer.setDrawableObjectVector(vector);

	viewer.show();

	return application.exec();
#else
	(void) argc; // unused
	(void) argv;
	return 0;
#endif
}
