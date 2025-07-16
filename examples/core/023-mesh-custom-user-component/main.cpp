/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
