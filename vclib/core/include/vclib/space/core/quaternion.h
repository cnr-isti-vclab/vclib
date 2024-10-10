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

#ifndef VCL_SPACE_CORE_QUATERNION_H
#define VCL_SPACE_CORE_QUATERNION_H

#include "matrix.h"
#include "point.h"

namespace vcl {

/**
 * @brief Quaternion class.
 *
 * This class represents a quaternion, `w + xi + yj + zk` vector with a scalar
 * part and a vector part.
 *
 * Note that the scalar part is stored in the `w` component, while the vector
 * part is stored in the `x`, `y` and `z` components.
 *
 * @note Note that internally the coefficients are stored in the following
 * order: [x, y, z, w]. This means that accessing to the first component of the
 * quaternion (index 0) will return the `x` component, and the last component
 * (index 3) will return the `w` component.
 *
 * @tparam Scalar: the scalar used to represent the quaternion.
 *
 * @ingroup space_core
 */
template<typename Scalar>
class Quaternion : public Eigen::Quaternion<Scalar>
{
    using Base = Eigen::Quaternion<Scalar>;

public:
    // inherit Base constructors
    using Base::Base;

    // inherit Base operators
    using Base::operator*;
    using Base::operator*=;

    /**
     * @brief The Scalar type of the Quaternion.
     */
    using ScalarType = Scalar;

    /**
     * @brief Constructs a quaternion representing the identity rotation
     * (w = 1, 0, 0, 0).
     */
    Quaternion() : Base(1, 0, 0, 0) {}

    template<typename S, int Options>
    Quaternion(const Eigen::Quaternion<S, Options>& q) : Base(q)
    {
    }

    Quaternion(const Scalar& angle, const Point3<Scalar>& axis) :
            Base(Eigen::AngleAxis<Scalar>(angle, axis))
    {
    }

    Quaternion(const Matrix44<Scalar>& rotMatrix) :
            Base(Matrix33<Scalar>(rotMatrix.block(0, 0, 3, 3)))
    {
    }

    /**
     * @brief Constructs the quaternion that will represent the rotation between
     * the two arbitrary vectors a and b.
     *
     * In other words, the built rotation represent a rotation sending the line
     * of direction a to the line of direction b, both lines passing through the
     * origin.
     *
     * @param[in] a: first input vector.
     * @param[in] b: second input vector.
     */
    Quaternion(const Point3<Scalar>& a, const Point3<Scalar>& b)
    {
        Base::setFromTwoVectors(a, b);
    }

    /**
     * @brief Casts the Quaternion object to a different scalar type.
     *
     * The function returns a new Quaternion object with each scalar value
     * casted to a different type.
     *
     * @tparam S: The scalar type to cast to.
     *
     * @return A new Quaternion object with each scalar value casted to a
     * different type.
     */
    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            return Quaternion<S>(Base::template cast<S>());
        }
    }

    constexpr uint size() const { return 4; }

    // void setIdentity() { mQ.setIdentity(); }

    void set(const Scalar& w, const Scalar& x, const Scalar& y, const Scalar& z)
    {
        Base::w() = w;
        Base::x() = x;
        Base::y() = y;
        Base::z() = z;
    }

    void setFromAngleAxis(const Scalar& angle, const Point3<Scalar>& axis)
    {
        *this = Base(Eigen::AngleAxis<Scalar>(angle, axis));
    }

    /**
     * @brief Computes the hash value of the quaternion.
     *
     * This function computes a hash value of the quaternion, which can be used
     * for hashing and comparison purposes.
     *
     * @return The hash value of the quaternion.
     */
    std::size_t hash() const
    {
        std::size_t h = 0;
        for (size_t i = 0; i < 4; ++i)
            vcl::hashCombine(h, Base::operator()(i));
        return h;
    }

    /**
     * @brief Quaternion-Vector multiplication.
     *
     * Corresponds to the operation q * v * q^{-1} where q is the quaternion and
     * v is the vector.
     *
     * @param[in] p: The vector to rotate.
     * @return The rotated vector.
     */
    Point3<Scalar> operator*(const Point3<Scalar>& p) const
    {
        Eigen::Matrix<Scalar, 1, 3> fc = Base::vec().cross(p);

        Eigen::Matrix<Scalar, 1, 3> fd = p * Base::w();

        Eigen::Matrix<Scalar, 1, 3> s = fc + fd;

        Eigen::Matrix<Scalar, 1, 3> sc = Base::vec().cross(s);

        return Point3<Scalar>(p + sc * 2.0);
    }
};

/**
 * @brief A convenience alias for Quaternion with floating-point components.
 *
 * The Quaternionf alias is a shorthand for a Quaternion class template
 * specialization with floating-point components.
 *
 * @ingroup space_core
 */
using Quaternionf = Quaternion<float>;

/**
 * @brief A convenience alias for Quaternion with double-precision
 * floating-point components.
 *
 * The Quaterniond alias is a shorthand for a Quaternion class template
 * specialization with double-precision floating-point components.
 *
 * @ingroup space_core
 */
using Quaterniond = Quaternion<double>;

} // namespace vcl

#endif // VCL_SPACE_CORE_QUATERNION_H
