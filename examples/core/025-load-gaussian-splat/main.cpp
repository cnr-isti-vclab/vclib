// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>
#include <vclib/meshes/point_cloud.h>

#include <vclib/io.h>
#include <vclib/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space.h>

int main()
{
    // left empty for tests

    vcl::LoadSettings lS;
    lS.loadTextureImages = true;

    std::cout << "Loading Bonsai Gaussian Splat Point Cloud..." << std::endl;

    auto [pointCloud, info] = vcl::loadMeshWithInfo<vcl::PointCloud>(
        VCLIB_EXAMPLE_MESHES_PATH "/splat/cactus_142k.ply",lS);

    std::cout << "has gs: " << info.hasVertexGaussianSplat() << "\n"
              << std::endl;

    std::cout << "is gs available: " << pointCloud.isVertexGaussianSplatAvailable() << std::endl;

    std::cout << "Loaded Bonsai Gaussian Splat Point Cloud" << std::endl;

    auto v0 = pointCloud.vertex(0);

    const auto& gs0 = v0.gaussianSplat();

    std::cout << "Gaussian Splat of vertex 0: \n" << std::endl;

    std::cerr << "\t rotation: " << gs0.rotation() << "\n";
    std::cerr << "\t scale: " << gs0.scale() << "\n";
    std::cerr << "\t opacity: " << gs0.opacity() << "\n";
    std::cerr << "\t degree: " << gs0.shDegree() << "\n";
    std::cerr << "\t coeffs: \n";
    for (int i = 0; i < gs0.shCoeffs().size(); ++i)
        std::cerr << "\t\t" << gs0.shCoeffs()[i] << "\n";

    return 0;
}
