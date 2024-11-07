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
