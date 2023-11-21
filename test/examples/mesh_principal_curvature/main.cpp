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

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/misc/timer.h>

#ifdef VCLIB_WITH_QGLVIEWER
#include <QApplication>

#include <vclib/ext/opengl2/drawable_mesh.h>
#include <vclib/ext/qglviewer/viewer_main_window.h>
#endif

int main(int argc, char** argv)
{
    vcl::ConsoleLogger log;
    log.setPrintTimer(true);

    vcl::TriMesh m =
        vcl::loadObj<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bimba.obj");

    m.enablePerVertexAdjacentFaces();
    m.enablePerFaceAdjacentFaces();
    m.enablePerVertexPrincipalCurvature();
    m.enablePerVertexColor();
    m.enablePerVertexQuality();

    vcl::updatePerFaceNormals(m);
    vcl::updatePerFaceAdjacentFaces(m);
    vcl::updatePerVertexAdjacentFaces(m);

    double radius = vcl::boundingBox(m).diagonal() * 0.1;
    log.startTimer();
    vcl::updatePrincipalCurvaturePCA(m, radius, true, log);

    vcl::setPerVertexQualityFromPrincipalCurvatureMean(m);
    vcl::Histogramd h = vcl::vertexQualityHistogram(m);

    vcl::setPerVertexColorFromQuality(
        m, vcl::Color::RedBlue, h.percentile(0.1), h.percentile(0.9));

    std::cout << "Curvature range: " << h.minRangeValue() << " "
              << h.maxRangeValue() << "\n";
    std::cout << "Used 90 percentile: " << h.percentile(0.1) << " "
              << h.percentile(0.9) << "\n";

    m.enablePerFaceColor();
    vcl::setPerFaceColorFromVertexColor(m);

    vcl::saveStl(m, VCL_TEST_RESULTS_PATH "/bimba_curvature.stl");

#ifdef VCLIB_WITH_QGLVIEWER
    QApplication application(argc, argv);

    vcl::qgl::ViewerMainWindow         viewer;
    vcl::gl2::DrawableMesh<vcl::TriMesh> dm(m);

    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();
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
