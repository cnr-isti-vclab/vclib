// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "igl_booleans.h"

#include <vclib/render/mesh_viewer.h>

int main(int argc, char** argv)
{
    auto [tm1, tm2, tUnion, tIntersection] = meshBooleans();

    auto [pm1, pm2, pUnion, pIntersection] = polyMeshBooleans();

    return vcl::showOnMeshViewer(
        argc,
        argv,
        std::move(tm1),
        std::move(tm2),
        std::move(tUnion),
        std::move(tIntersection),
        std::move(pm1),
        std::move(pm2),
        std::move(pUnion),
        std::move(pIntersection));
}
