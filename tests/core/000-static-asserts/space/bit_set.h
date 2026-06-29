// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
