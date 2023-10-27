/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_SPACE_QUATERNION_H
#define VCL_SPACE_QUATERNION_H

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
 */
template<typename Scalar>
class Quaternion
{
    template<typename S>
    friend class Quaternion;

protected:
    Eigen::Quaternion<Scalar> q = Eigen::Quaternion<Scalar>(1, 0, 0, 0);

public:
    /**
     * @brief The Scalar type of the Quaternion.
     */
    using ScalarType = Scalar;

    /**
     * @brief Constructs a quaternion representing the identity rotation
     * (w = 1, 0, 0, 0).
     */
    Quaternion() = default;

    /**
     * @brief Constructs and initializes the quaternion `w+xi+yj+zk` from its
     * four coefficients w, x, y and z.
     *
     * @note Note the order of the arguments: the real w coefficient first,
     * while internally the coefficients are stored in the following order: [x,
     * y, z, w]
     *
     * @param[in] w: the real coefficient.
     * @param[in] x: the first imaginary coefficient.
     * @param[in] y: the second imaginary coefficient.
     * @param[in] z: the third imaginary coefficient.
     */
    Quaternion(
        const Scalar& w,
        const Scalar& x,
        const Scalar& y,
        const Scalar& z) :
            q(w, x, y, z)
    {
    }

    Quaternion(const Scalar& angle, const Point3<Scalar>& axis) :
            q(Eigen::AngleAxis<Scalar>(angle, axis.eigenVector()))
    {
    }

    Quaternion(const Eigen::Quaternion<Scalar>& qq) { q << qq; }

    Quaternion(const Matrix33<Scalar>& rotMatrix) : q(rotMatrix) {}

    Quaternion(const Matrix44<Scalar>& rotMatrix) : q(rotMatrix) {}

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
        q.setFromTwoVectors(a.eigenVector(), b.eigenVector());
    }

    Scalar& w() { return q.w(); }

    const Scalar& w() const { return q.w(); }

    Scalar& x() { return q.x(); }

    const Scalar& x() const { return q.x(); }

    Scalar& y() { return q.y(); }

    const Scalar& y() const { return q.y(); }

    Scalar& z() { return q.z(); }

    const Scalar& z() const { return q.z(); }

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
    const Quaternion<S> cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            return Quaternion<S>(q.template cast<S>());
        }
    }

    /**
     * @brief Returns the conjugate of the quaternion, which represents the
     * opposite rotation.
     *
     * The conjugate of the quaternion is equal to the multiplicative inverse if
     * the quaternion is normalized.
     *
     * @return The conjugate of the quaternion.
     */
    Quaternion<Scalar> conjugate() const
    {
        return Quaternion<Scalar>(q.conjugate());
    }

    /**
     * @brief Returns the dot product between this quaternion and the given
     * quaternion.
     *
     * The dot product between two unit quaternions corresponds to the cosine of
     * half the angle between the two rotations.
     *
     * @param[in] q2: the quaternion to compute the dot product with.
     * @return The dot product between this quaternion and the given quaternion.
     */
    Scalar dot(const Quaternion<Scalar>& q2) const { return q.dot(q2.q); }

    /**
     * @brief Returns the inverse of the quaternion, which represents the
     * inverse rotation.
     *
     * The inverse of the quaternion is equal to the multiplicative inverse.
     *
     * @note Note that in most cases, i.e., if you simply want the opposite
     * rotation, and/or the quaternion is normalized, then it is enough to use
     * the conjugate.
     *
     * @return The inverse of the quaternion.
     */
    Quaternion<Scalar> inverse() const
    {
        return Quaternion<Scalar>(q.inverse());
    }

    Scalar norm() const { return q.norm(); }

    Scalar squaredNorm() const { return q.squaredNorm(); }

    constexpr uint size() const { return 4; }

    void setIdentity() { q.setIdentity(); }

    void set(const Scalar& w, const Scalar& x, const Scalar& y, const Scalar& z)
    {
        q.w() = w;
        q.x() = x;
        q.y() = y;
        q.z() = z;
    }

    void setFromTwoVectors(const Point3<Scalar>& a, const Point3<Scalar>& b)
    {
        q.setFromTwoVectors(a.eigenVector(), b.eigenVector());
    }

    void setFromAngleAxis(const Scalar& angle, const Point3<Scalar>& axis)
    {
        q = Eigen::Quaternion<Scalar>(
            Eigen::AngleAxis<Scalar>(angle, axis.eigenVector()));
    }

    Quaternion<Scalar> normalized() const
    {
        return Quaternion<Scalar>(q.normalized());
    }

    void normalize() { q.normalize(); }

    const Eigen::Quaternion<Scalar>& eigenQuaternion() const { return q; }

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
            vcl::hashCombine(h, q(i));
        return h;
    }

    Scalar& operator()(uint i) { return q.coeffs()[i]; }

    const Scalar& operator()(uint i) const { return q.coeffs()[i]; }

    Scalar& operator[](uint i) { return q.coeffs()[i]; }

    const Scalar& operator[](uint i) const { return q.coeffs()[i]; }

    bool operator==(const Quaternion<Scalar>& q2) const { return q == q2.q; }

    bool operator!=(const Quaternion<Scalar>& q2) const { return q != q2.q; }

    Quaternion<Scalar> operator*(const Quaternion<Scalar>& q2)
    {
        return Quaternion<Scalar>(q * q2.q);
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
    Point3<Scalar> operator*(const Point3<Scalar>& p)
    {
        const Eigen::Matrix<Scalar, 1, 3>& v = p.eigenVector();
        return Point3<Scalar>(
            v + 2.0 * q.vec().cross(q.vec().cross(v) + q.w() * v));
    }

    Quaternion& operator*=(const Quaternion<Scalar>& q2)
    {
        q *= q2.q;
        return *this;
    }

    Matrix33<Scalar> toRotationMatrix() const { return q.toRotationMatrix(); }
};

} // namespace vcl

#endif // VCL_SPACE_QUATERNION_H
