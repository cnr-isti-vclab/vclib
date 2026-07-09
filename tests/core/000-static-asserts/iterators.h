// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef ITERATORS_H
#define ITERATORS_H

#include <vclib/base.h>

#include <vector>

void iteratorsStaticAsserts()
{
    using namespace vcl;

    using StdVecInt = std::vector<int>;

    static_assert(
        IteratorConcept<StdVecInt::iterator>,
        "std::vector<int>::iterator does not satisfy the IteratorConcept");
    static_assert(
        IteratorConcept<StdVecInt::const_iterator>,
        "std::vector<int>::const_iterator does not satisfy the "
        "IteratorConcept");

    static_assert(
        InputIterator<StdVecInt::iterator, int>,
        "std::vector<int>::iterator does not satisfy the InputIterator "
        "concept");
    static_assert(
        InputIterator<StdVecInt::const_iterator, int>,
        "std::vector<int>::const_iterator does not satisfy the InputIterator "
        "concept");

    static_assert(
        OutputIterator<StdVecInt::iterator, int>,
        "std::vector<int>::iterator does not satisfy the OutputIterator "
        "concept");
    static_assert(
        !OutputIterator<StdVecInt::const_iterator, int>,
        "std::vector<int>::iterator does satisfy the OutputIterator "
        "concept");
}

#endif // ITERATORS_H
