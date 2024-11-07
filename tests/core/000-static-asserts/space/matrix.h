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

#ifndef MATRIX_H
#define MATRIX_H

#include <vclib/space.h>

void matrixStaticAsserts()
{
    using namespace vcl;

    using TestMatrix  = Matrix<uint16_t, 3, 3>;
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
        MatrixConcept<TestArray>,
        "Array2 does not satisfy the MatrixConcept");
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
}

#endif // MATRIX_H
