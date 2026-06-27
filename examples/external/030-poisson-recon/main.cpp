// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/io.h>
#include <vclib/meshes.h>

#include <vclib/poisson_recon/poisson_reconstruction.h>

int main()
{
    vcl::PointCloud cmln;
    vcl::loadMesh(cmln, VCLIB_EXAMPLE_MESHES_PATH "/chameleon4k.ply");

    // vcl::updatePerVertexAndFaceNormals(cmln);

    auto outMesh = vcl::poiss::poissonReconstruction<vcl::TriMesh>(cmln, 8);

    std::string resultsPath = VCLIB_EXTERNAL_RESULTS_PATH;

    vcl::saveMesh(outMesh, resultsPath + "/030_recon_chameleon.ply");

    return 0;
}
