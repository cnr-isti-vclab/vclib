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

#ifndef BIT_SET_H
#define BIT_SET_H

#include <vclib/space.h>

void bitSetStaticAsserts()
{
    using namespace vcl;

    using BitSetType = BitSet<uint16_t>;

    static_assert(BitProxyConcept<BitProxy<uint16_t>>, "");

    // bitset
    static_assert(
        BitSetConcept<BitSetType>, "BitSet does not satisfy the BitSetConcept");
    static_assert(
        BitSetConcept<BitSetType&>,
        "BitSet& does not satisfy the BitSetConcept");
    static_assert(
        BitSetConcept<const BitSetType>,
        "const BitSet does not satisfy the BitSetConcept");
    static_assert(
        BitSetConcept<const BitSetType&>,
        "const BitSet& does not satisfy the BitSetConcept");
    static_assert(
        BitSetConcept<BitSetType&&>,
        "BitSet&& does not satisfy the BitSetConcept");

    static_assert(Serializable<BitSetType>, "BitSet is not serializable");
}

#endif // BIT_SET_H
