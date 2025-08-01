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
