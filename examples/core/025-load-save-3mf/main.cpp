// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "load_save_3mf.h"

int main()
{
    auto [mesh0, mesh1, mesh2] = load3mf();

    save3mf(mesh0, mesh1, mesh2);

    return 0;
}
