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

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/space/core/array.h>

#include <iostream>

int main()
{
    vcl::TriMesh tm =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    Eigen::MatrixXd v = vcl::vertexCoordsMatrix<Eigen::MatrixXd>(tm);
    Eigen::MatrixXi f = vcl::faceIndicesMatrix<Eigen::MatrixXi>(tm);

    std::cerr << "Vertices:\n" << v << "\n\n";

    std::cerr << "Faces:\n" << f << "\n\n";

    vcl::PolyMesh pm =
        vcl::loadPly<vcl::PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    vcl::Array2<double> va = vcl::vertexCoordsMatrix<vcl::Array2<double>>(pm);
    vcl::Array2<int>    fa = vcl::faceIndicesMatrix<vcl::Array2<int>>(pm);

    std::cerr << "Vertices:\n" << va << "\n\n";

    std::cerr << "Faces:\n" << fa << "\n\n";

    return 0;
}
