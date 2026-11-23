// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef RANGES_H
#define RANGES_H

#include <vclib/base.h>

#include <vector>

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

    // input ranges
    static_assert(InputRange<std::vector<int>, int>, "");
    static_assert(InputRange<const std::vector<int>, int>, "");
    static_assert(InputRange<std::vector<int>&, int>, "");
    static_assert(InputRange<const std::vector<int>&, int>, "");
    static_assert(InputRange<std::vector<int>&&, int>, "");
    static_assert(InputRange<std::vector<int>, uint>, "");
    static_assert(InputRange<const std::vector<int>, uint>, "");
    static_assert(InputRange<std::vector<int>&, uint>, "");
    static_assert(InputRange<const std::vector<int>&, uint>, "");
    static_assert(InputRange<std::vector<int>&&, uint>, "");

    // output ranges
    static_assert(OutputRange<std::vector<int>, int>, "");
    static_assert(!OutputRange<const std::vector<int>, int>, "");
    static_assert(OutputRange<std::vector<int>&, int>, "");
    static_assert(!OutputRange<const std::vector<int>&, int>, "");
    static_assert(OutputRange<std::vector<int>&&, int>, "");
    static_assert(!OutputRange<std::vector<int>, uint>, "");
    static_assert(!OutputRange<const std::vector<int>, uint>, "");
    static_assert(!OutputRange<std::vector<int>&, uint>, "");
    static_assert(!OutputRange<const std::vector<int>&, uint>, "");
    static_assert(!OutputRange<std::vector<int>&&, uint>, "");
}

#endif // RANGES_H
