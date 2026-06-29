// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MATRIX_H
#define MATRIX_H

#include <vclib/space.h>

void matrixStaticAsserts()
{
    using namespace vcl;

    using TestMatrix    = Matrix<uint16_t, 3, 3>;
    using TestRowMatrix = Eigen::Matrix<uint8_t, 2, 4, Eigen::RowMajor>;
    using TestMatrix3   = Eigen::Matrix3d;
    using TestVector    = Eigen::VectorXi;

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
        "RowMajor const Eigen::Matrix& does not satisfy the "
        "EigenMatrixConcept");
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
