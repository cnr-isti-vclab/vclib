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

    auto pointCloud = vcl::loadMesh<vcl::PointCloud>(
        VCLIB_EXAMPLE_MESHES_PATH "/bonsai_gaussian_splat.ply", lS);

    std::cout << "Loaded Bonsai Gaussian Splat Point Cloud" << std::endl;

    vcl::pointcloud::Vertex<double>& v0 = pointCloud.vertex(0);
    vcl::MeshInfo info = vcl::MeshInfo(pointCloud);

    std::cout << "has gs: " << info.hasPerVertexGaussianSplat() << "\n" << std::endl;
    std::cout << "is gs available: " << vcl::comp::isGaussianSplatAvailableOn(v0) << "\n" << std::endl;

    auto& gs0 = v0.gaussianSplat();

    //gs0.serialize(std::cout);

    //std::cout << ... << std::endl;

    return 0;
}
