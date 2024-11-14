/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef RANGES_H
#define RANGES_H

#include <vector>

#include <vclib/concepts/ranges/range.h>

void rangesStaticAsserts()
{
    using namespace vcl;

    // range
    static_assert(Range<std::vector<int>>, "");
    static_assert(Range<const std::vector<int>>, "");
    static_assert(Range<std::vector<int>&>, "");
    static_assert(Range<const std::vector<int>&>, "");
    static_assert(Range<std::vector<int>&&>, "");

    // range of
    static_assert(RangeOf<std::vector<int>, int>, "");
    static_assert(RangeOf<const std::vector<int>, int>, "");
    static_assert(RangeOf<std::vector<int>&, int>, "");
    static_assert(RangeOf<const std::vector<int>&, int>, "");
    static_assert(RangeOf<std::vector<int>&&, int>, "");

    static_assert(!RangeOf<std::vector<int>, uint>, "");
    static_assert(!RangeOf<const std::vector<int>, uint>, "");
    static_assert(!RangeOf<std::vector<int>&, uint>, "");
    static_assert(!RangeOf<const std::vector<int>&, uint>, "");
    static_assert(!RangeOf<std::vector<int>&&, uint>, "");

    // range of convertible to
    static_assert(RangeOfConvertibleTo<std::vector<int>, uint>, "");
    static_assert(RangeOfConvertibleTo<const std::vector<int>, uint>, "");
    static_assert(RangeOfConvertibleTo<std::vector<int>&, uint>, "");
    static_assert(RangeOfConvertibleTo<const std::vector<int>&, uint>, "");
    static_assert(RangeOfConvertibleTo<std::vector<int>&&, uint>, "");
}

#endif // RANGES_H
