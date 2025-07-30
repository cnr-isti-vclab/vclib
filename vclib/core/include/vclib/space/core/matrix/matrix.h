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

#ifndef VCL_SPACE_CORE_MATRIX_MATRIX_H
#define VCL_SPACE_CORE_MATRIX_MATRIX_H

#include <vclib/concepts.h>
#include <vclib/serialization.h>

#include <Eigen/Core>

namespace vcl {

template<typename Scalar, int R, int C>
class Matrix : public Eigen::Matrix<Scalar, R, C>
{
    using Base = Eigen::Matrix<Scalar, R, C>;

public:
    Matrix() = default;

    // inherit constructors
    using Base::Base;

    template<typename OtherDerived>
    Matrix(const Eigen::MatrixBase<OtherDerived>& other) : Base(other)
    {
    }

    template<typename OtherDerived>
    Matrix& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
        this->Base::operator=(other);
        return *this;
    }

    void serialize(std::ostream& os) const
    {
        uint rows = Base::rows();
        uint cols = Base::cols();
        vcl::serialize(os, rows, cols);
        vcl::serializeN(os, Base::data(), Base::rows() * Base::cols());
    }

    void deserialize(std::istream& is)
    {
        uint rows, cols;
        vcl::deserialize(is, rows, cols);
        Base::resize(rows, cols);
        vcl::deserializeN(is, Base::data(), Base::rows() * Base::cols());
    }
};

/* Specialization Aliases */

template<typename Scalar>
using Matrix33 = Matrix<Scalar, 3, 3>;

using Matrix33i = Matrix33<int>;
using Matrix33f = Matrix33<float>;
using Matrix33d = Matrix33<double>;

template<typename Scalar>
using Matrix44 = Matrix<Scalar, 4, 4>;

using Matrix44i = Matrix44<int>;
using Matrix44f = Matrix44<float>;
using Matrix44d = Matrix44<double>;

/* Concepts */

/**
 * @brief A concept representing an Eigen Matrix.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from an Eigen matrix class having any scalar type and any number of rows and
 * columns.
 *
 * @tparam T: The type to be tested for conformity to the EigenMatrixConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept EigenMatrixConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Eigen::Matrix<
        typename RemoveRef<T>::Scalar,
                  RemoveRef<T>::RowsAtCompileTime,
                  RemoveRef<T>::ColsAtCompileTime,
                  RemoveRef<T>::Options>>;

} // namespace vcl

#endif // VCL_SPACE_CORE_MATRIX_MATRIX_H
