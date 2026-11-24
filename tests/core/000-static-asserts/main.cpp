// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "const_correctness.h"
#include "iterators.h"
#include "loggers.h"
#include "mesh.h"
#include "meshes.h"
#include "ranges.h"
#include "space.h"

int main()
{
    constCorrectnessStaticAsserts();
    loggersStaticAsserts();
    iteratorsStaticAsserts();
    meshStaticAsserts();
    meshesStaticAsserts();
    rangesStaticAsserts();
    spaceStaticAsserts();

    return 0;
}
