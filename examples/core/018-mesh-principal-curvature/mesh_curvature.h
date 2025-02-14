/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef MESH_CURVATURE_H
#define MESH_CURVATURE_H

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/misc/timer.h>

#include <iostream>

template<vcl::MeshConcept MeshType>
void updateCurvature(MeshType& m)
{
    vcl::ConsoleLogger log;
    log.enablePrintTimer();

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
        m, vcl::Color::ColorMap::RedBlue, h.percentile(0.1), h.percentile(0.9));

    std::cout << "Curvature range: " << h.minRangeValue() << " "
              << h.maxRangeValue() << "\n";
    std::cout << "Used 90 percentile: " << h.percentile(0.1) << " "
              << h.percentile(0.9) << "\n";

    m.enablePerFaceColor();
    vcl::setPerFaceColorFromVertexColor(m);
}

#endif // MESH_CURVATURE_H
