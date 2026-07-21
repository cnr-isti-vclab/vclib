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
    auto [m1, m2, mUnion, mIntersection] = meshBooleans();

    vcl::updatePerVertexAndFaceNormals(m1);
    vcl::updatePerVertexAndFaceNormals(m2);
    vcl::updatePerVertexAndFaceNormals(mUnion);
    vcl::updatePerVertexAndFaceNormals(mIntersection);

    return vcl::showOnMeshViewer(
        argc,
        argv,
        std::move(m1),
        std::move(m2),
        std::move(mUnion),
        std::move(mIntersection));
}
