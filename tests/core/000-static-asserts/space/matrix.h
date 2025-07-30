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

#ifndef MATRIX_H
#define MATRIX_H

#include <vclib/space.h>

void matrixStaticAsserts()
{
    using namespace vcl;

    using TestMatrix  = Matrix<uint16_t, 3, 3>;
    using TestRowMatrix = Eigen::Matrix<uint8_t, 2, 4, Eigen::RowMajor>;
    using TestMatrix3 = Eigen::Matrix3d;
    using TestVector  = Eigen::VectorXi;

    using TestArray = Array2<float>;

    // matrix
    static_assert(
        EigenMatrixConcept<TestMatrix>,
        "Eigen::Matrix does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestMatrix&>,
        "Eigen::Matrix& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestMatrix>,
        "const Eigen::Matrix does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestMatrix&>,
        "const Eigen::Matrix& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestMatrix&&>,
        "Eigen::Matrix&& does not satisfy the EigenMatrixConcept");

    static_assert(
        EigenMatrixConcept<TestRowMatrix>,
        "RowMajor Eigen::Matrix does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestRowMatrix&>,
        "RowMajor Eigen::Matrix& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestRowMatrix>,
        "RowMajor const Eigen::Matrix does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestRowMatrix&>,
        "RowMajor const Eigen::Matrix& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestRowMatrix&&>,
        "RowMajor Eigen::Matrix&& does not satisfy the EigenMatrixConcept");

    static_assert(
        EigenMatrixConcept<TestMatrix3>,
        "Eigen::Matrix3 does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestMatrix3&>,
        "Eigen::Matrix3& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestMatrix3>,
        "const Eigen::Matrix3 does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestMatrix3&>,
        "const Eigen::Matrix3& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestMatrix3&&>,
        "Eigen::Matrix3&& does not satisfy the EigenMatrixConcept");

    static_assert(
        EigenMatrixConcept<TestVector>,
        "Eigen::Vector does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestVector&>,
        "Eigen::Vector& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestVector>,
        "const Eigen::Vector does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<const TestVector&>,
        "const Eigen::Vector& does not satisfy the EigenMatrixConcept");
    static_assert(
        EigenMatrixConcept<TestVector&&>,
        "Eigen::Vector&& does not satisfy the EigenMatrixConcept");

    static_assert(
        !EigenMatrixConcept<TestArray>,
        "Array2 satisfies the EigenMatrixConcept??");
    static_assert(
        !EigenMatrixConcept<TestArray&>,
        "Array2& satisfies the EigenMatrixConcept??");
    static_assert(
        !EigenMatrixConcept<const TestArray>,
        "const Array2 satisfies the EigenMatrixConcept??");
    static_assert(
        !EigenMatrixConcept<const TestArray&>,
        "const Array2& satisfies the EigenMatrixConcept??");
    static_assert(
        !EigenMatrixConcept<TestArray&&>,
        "Array2&& satisfies the EigenMatrixConcept??");

    static_assert(
        MatrixConcept<TestMatrix3>,
        "Eigen::Matrix3 does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<TestMatrix3&>,
        "Eigen::Matrix3& does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<const TestMatrix3>,
        "const Eigen::Matrix3 does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<const TestMatrix3&>,
        "const Eigen::Matrix3& does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<TestMatrix3&&>,
        "Eigen::Matrix3&& does not satisfy the MatrixConcept");

    static_assert(
        MatrixConcept<TestVector>,
        "Eigen::Vector does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<TestVector&>,
        "Eigen::Vector& does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<const TestVector>,
        "const Eigen::Vector does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<const TestVector&>,
        "const Eigen::Vector& does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<TestVector&&>,
        "Eigen::Vector&& does not satisfy the MatrixConcept");

    static_assert(
        MatrixConcept<TestArray>, "Array2 does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<TestArray&>,
        "Array2& does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<const TestArray>,
        "const Array2 does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<const TestArray&>,
        "const Array2& does not satisfy the MatrixConcept");
    static_assert(
        MatrixConcept<TestArray&&>,
        "Array2&& does not satisfy the MatrixConcept");

    static_assert(
        Matrix33Concept<Matrix33f>,
        "Matrix33f does not satisfy the Matrix33Concept");
    static_assert(
        Matrix33Concept<Matrix33f&>,
        "Matrix33f& does not satisfy the Matrix33Concept");
    static_assert(
        Matrix33Concept<const Matrix33f>,
        "const Matrix33f does not satisfy the Matrix33Concept");
    static_assert(
        Matrix33Concept<const Matrix33f&>,
        "const Matrix33f& does not satisfy the Matrix33Concept");
    static_assert(
        Matrix33Concept<Matrix33f&&>,
        "Matrix33f&& does not satisfy the Matrix33Concept");

    static_assert(
        Matrix44Concept<Matrix44f>,
        "Matrix44f does not satisfy the Matrix44Concept");
    static_assert(
        Matrix44Concept<Matrix44f&>,
        "Matrix44f& does not satisfy the Matrix44Concept");
    static_assert(
        Matrix44Concept<const Matrix44f>,
        "const Matrix44f does not satisfy the Matrix44Concept");
    static_assert(
        Matrix44Concept<const Matrix44f&>,
        "const Matrix44f& does not satisfy the Matrix44Concept");
    static_assert(
        Matrix44Concept<Matrix44f&&>,
        "Matrix44f&& does not satisfy the Matrix44Concept");
}

#endif // MATRIX_H
