// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SEGMENT_H
#define SEGMENT_H

#include <vclib/space.h>

void segmentStaticAsserts()
{
    using namespace vcl;

    using SegmentType  = Segment<Point<uint16_t, 5>>;
    using Segment2Type = Segment2d;
    using Segment3Type = Segment3f;

    // segment
    static_assert(
        SegmentConcept<SegmentType>,
        "Segment does not satisfy the SegmentConcept");
    static_assert(
        SegmentConcept<const SegmentType>,
        "const Segment does not satisfy the SegmentConcept");
    static_assert(
        SegmentConcept<SegmentType&>,
        "Segment& does not satisfy the SegmentConcept");
    static_assert(
        SegmentConcept<const SegmentType&>,
        "const Segment& does not satisfy the SegmentConcept");
    static_assert(
        SegmentConcept<SegmentType&&>,
        "Segment&& does not satisfy the SegmentConcept");

    // segment2
    static_assert(
        Segment2Concept<Segment2Type>,
        "Segment2 does not satisfy the Segment2Concept");
    static_assert(
        Segment2Concept<const Segment2Type>,
        "const Segment2 does not satisfy the Segment2Concept");
    static_assert(
        Segment2Concept<Segment2Type&>,
        "Segment2& does not satisfy the Segment2Concept");
    static_assert(
        Segment2Concept<const Segment2Type&>,
        "const Segment2& does not satisfy the Segment2Concept");
    static_assert(
        Segment2Concept<Segment2Type&&>,
        "Segment2&& does not satisfy the Segment2Concept");

    // segment3
    static_assert(
        Segment3Concept<Segment3Type>,
        "Segment3 does not satisfy the Segment3Concept");
    static_assert(
        Segment3Concept<const Segment3Type>,
        "const Segment3 does not satisfy the Segment3Concept");
    static_assert(
        Segment3Concept<Segment3Type&>,
        "Segment3& does not satisfy the Segment3Concept");
    static_assert(
        Segment3Concept<const Segment3Type&>,
        "const Segment3& does not satisfy the Segment3Concept");
    static_assert(
        Segment3Concept<Segment3Type&&>,
        "Segment3&& does not satisfy the Segment3Concept");
}

#endif // SEGMENT_H
