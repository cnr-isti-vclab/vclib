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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

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
