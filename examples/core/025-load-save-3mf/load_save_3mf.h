// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LOAD_SAVE_3MF_H
#define LOAD_SAVE_3MF_H

#include <vclib/io.h>
#include <vclib/mesh.h>
#include <vclib/meshes.h>

auto load3mf()
{
    vcl::TriMesh box;
    vcl::loadMesh(box, VCLIB_EXAMPLE_MESHES_PATH "box.3mf");

    vcl::TriMesh dodecaChainLoop = vcl::loadMesh<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "dodeca_chain_loop.3mf");

    auto cubeGears =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "cube_gears.3mf");

    return std::make_tuple(box, dodecaChainLoop, cubeGears);
}

void save3mf(const vcl::MeshConcept auto&... meshes)
{
    std::string resultsPath = VCLIB_CORE_RESULTS_PATH;
    size_t i = 0;

    ([&](const auto& m) {
        std::string mName = "/mesh_" + std::to_string(i++) + ".3mf";
        if (!m.name().empty()) {
            mName = "/" + m.name() + ".3mf";
        }
        std::string filename = resultsPath + mName;
        vcl::saveMesh(m, filename);
    }(meshes), ...);
}

#endif // LOAD_SAVE_3MF_H
