// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "bar_component.h"
#include "foo_component.h"

#include <iostream>

/**
 * Example 023: Custom User Components
 *
 * This example demonstrates how to create and use custom components in VCLib.
 * Two different approaches are shown:
 *
 * 1. FooComponent: Simple horizontal component (always available)
 * 2. BarComponent: Advanced optional component (runtime-controlled)
 */
int main()
{
    std::cout << "=== Custom User Components Example ===" << std::endl;

    // Demonstrate simple horizontal component
    demonstrateFooComponent();

    // Demonstrate advanced optional component
    demonstrateBarComponent();

    return 0;
}
