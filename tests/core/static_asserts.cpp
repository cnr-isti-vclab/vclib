// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <catch2/catch_test_macros.hpp>

#include "static_asserts/const_correctness.h"
#include "static_asserts/iterators.h"
#include "static_asserts/loggers.h"
#include "static_asserts/mesh.h"
#include "static_asserts/meshes.h"
#include "static_asserts/ranges.h"
#include "static_asserts/space.h"

TEST_CASE("Static Asserts", "[core][static_asserts]")
{
    constCorrectnessStaticAsserts();
    loggersStaticAsserts();
    iteratorsStaticAsserts();
    meshStaticAsserts();
    meshesStaticAsserts();
    rangesStaticAsserts();
    spaceStaticAsserts();

    SUCCEED();
}
