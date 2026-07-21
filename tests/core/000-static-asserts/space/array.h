// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef ARRAY_H
#define ARRAY_H

#include <vclib/space.h>

void arrayStaticAsserts()
{
    using namespace vcl;

    using TestArray  = Array<Point3d, 5>;
    using TestArray2 = Array<float, 2>;
    using TestArray3 = Array<double, 3>;
    using TestArray4 = Array<int, 4>;

    // array
    static_assert(
        ArrayConcept<TestArray>, "Array does not satisfy the ArrayConcept");
    static_assert(
        ArrayConcept<const TestArray>,
        "const Array does not satisfy the ArrayConcept");
    static_assert(
        ArrayConcept<TestArray&>, "Array& does not satisfy the ArrayConcept");
    static_assert(
        ArrayConcept<const TestArray&>,
        "const Array& does not satisfy the ArrayConcept");
    static_assert(
        ArrayConcept<TestArray&&>, "Array&& does not satisfy the ArrayConcept");

    static_assert(Serializable<TestArray>, "Array is not serializable");

    // array2
    static_assert(
        Array2Concept<TestArray2>, "Array2 does not satisfy the Array2Concept");
    static_assert(
        Array2Concept<const TestArray2>,
        "const Array2 does not satisfy the Array2Concept");
    static_assert(
        Array2Concept<TestArray2&>,
        "Array2& does not satisfy the Array2Concept");
    static_assert(
        Array2Concept<const TestArray2&>,
        "const Array2& does not satisfy the Array2Concept");
    static_assert(
        Array2Concept<TestArray2&&>,
        "Array2&& does not satisfy the Array2Concept");

    static_assert(Serializable<TestArray2>, "Array2d is not serializable");

    // array3
    static_assert(
        Array3Concept<TestArray3>, "Array3 does not satisfy the Array3Concept");
    static_assert(
        Array3Concept<const TestArray3>,
        "const Array3 does not satisfy the Array3Concept");
    static_assert(
        Array3Concept<TestArray3&>,
        "Array3& does not satisfy the Array3Concept");
    static_assert(
        Array3Concept<const TestArray3&>,
        "const Array3& does not satisfy the Array3Concept");
    static_assert(
        Array3Concept<TestArray3&&>,
        "Array3&& does not satisfy the Array3Concept");

    static_assert(Serializable<TestArray3>, "Array3 is not serializable");

    // array4
    static_assert(
        Array4Concept<TestArray4>, "Array4 does not satisfy the Array4Concept");
    static_assert(
        Array4Concept<TestArray4&>,
        "Array4& does not satisfy the Array4Concept");
    static_assert(
        Array4Concept<const TestArray4&>,
        "const Array4& does not satisfy the Array4Concept");
    static_assert(
        Array4Concept<TestArray4&&>,
        "Array4&& does not satisfy the Array4Concept");

    static_assert(Serializable<TestArray4>, "Array4 is not serializable");
}

#endif // ARRAY_H
