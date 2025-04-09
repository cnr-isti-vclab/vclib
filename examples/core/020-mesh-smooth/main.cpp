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
#include <vclib/misc/timer.h>

#include <iostream>

int main()
{
    vcl::TriMesh m = vcl::loadPly<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                                "/bunny_textured.ply");

    vcl::TriMesh mSmooth(m);

    vcl::Timer t1("Laplacian Smoothing");

    vcl::laplacianSmoothing(mSmooth, 30);
    t1.stopAndPrint();

    vcl::savePly(mSmooth, VCLIB_RESULTS_PATH "/bunny_lapl_smooth.ply");

    vcl::Timer t2("Taubin Smoothing");

    vcl::taubinSmoothing(m, 300, 0.5, -0.53);
    t2.stopAndPrint();

    vcl::savePly(m, VCLIB_RESULTS_PATH "/bunny_taub_smooth.ply");

    return 0;
}
