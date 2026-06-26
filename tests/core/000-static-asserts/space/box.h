// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BOX_H
#define BOX_H

#include <vclib/space.h>

void boxStaticAsserts()
{
    using namespace vcl;

    using BoxType  = Box<Point<uint16_t, 5>>;
    using Box2Type = Box2i;
    using Box3Type = Box3f;

    // box
    static_assert(BoxConcept<BoxType>, "Box does not satisfy the BoxConcept");
    static_assert(
        BoxConcept<const BoxType>, "const Box does not satisfy the BoxConcept");
    static_assert(BoxConcept<BoxType&>, "Box& does not satisfy the BoxConcept");
    static_assert(
        BoxConcept<const BoxType&>,
        "const Box& does not satisfy the BoxConcept");
    static_assert(
        BoxConcept<BoxType&&>, "Box&& does not satisfy the BoxConcept");
    static_assert(Serializable<BoxType>, "Box is not serializable");

    // box2
    static_assert(
        Box2Concept<Box2Type>, "Box2 does not satisfy the Box2Concept");
    static_assert(
        Box2Concept<const Box2Type>,
        "const Box2 does not satisfy the Box2Concept");
    static_assert(
        Box2Concept<Box2Type&>, "Box2& does not satisfy the Box2Concept");
    static_assert(
        Box2Concept<const Box2Type&>,
        "const Box2& does not satisfy the Box2Concept");
    static_assert(
        Box2Concept<Box2Type&&>, "Box2&& does not satisfy the Box2Concept");
    static_assert(Serializable<Box2Type>, "Box2 is not serializable");

    // box3
    static_assert(
        Box3Concept<Box3Type>, "Box3 does not satisfy the Box3Concept");
    static_assert(
        Box3Concept<const Box3Type>,
        "const Box3 does not satisfy the Box3Concept");
    static_assert(
        Box3Concept<Box3Type&>, "Box3& does not satisfy the Box3Concept");
    static_assert(
        Box3Concept<const Box3Type&>,
        "const Box3& does not satisfy the Box3Concept");
    static_assert(
        Box3Concept<Box3Type&&>, "Box3&& does not satisfy the Box3Concept");
    static_assert(Serializable<Box3Type>, "Box3 is not serializable");
}

#endif // BOX_H
