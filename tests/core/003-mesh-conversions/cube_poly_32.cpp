// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "helpers.h"

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "Mesh Conversions - The polygonal cube mesh loaded on PolyMesh (32-bit)",
    "",
    Meshes,
    Meshesf)
{
    using TriMesh  = typename TestType::first_type;
    using PolyMesh = typename TestType::second_type;

    testCubePolyConversion<TriMesh, PolyMesh>();
}
