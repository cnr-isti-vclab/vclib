// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CONST_CORRECTNESS_H
#define CONST_CORRECTNESS_H

#include <vclib/base.h>

void constCorrectnessStaticAsserts()
{
    using namespace vcl;

    static_assert(!IsConst<int>, "");
    static_assert(!IsConst<int*>, "");
    static_assert(!IsConst<int&>, "");
    static_assert(IsConst<const int>, "");
    static_assert(IsConst<const int*>, "");
    static_assert(IsConst<const int&>, "");
    static_assert(IsConst<const int* const>, "");
    static_assert(IsConst<int* const>, "");
}

#endif // CONST_CORRECTNESS_H
