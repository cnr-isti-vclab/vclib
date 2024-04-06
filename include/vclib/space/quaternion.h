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
    Eigen::Quaternion<Scalar> mQ = Eigen::Quaternion<Scalar>(1, 0, 0, 0);

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
            mQ(w, x, y, z)
    {
    }

    Quaternion(const Scalar& angle, const Point3<Scalar>& axis) :
            mQ(Eigen::AngleAxis<Scalar>(angle, axis.eigenVector()))
    {
    }

    Quaternion(const Eigen::Quaternion<Scalar>& qq) : mQ(qq) {}

    Quaternion(const Matrix33<Scalar>& rotMatrix) : mQ(rotMatrix) {}

    Quaternion(const Matrix44<Scalar>& rotMatrix) :
            mQ(Matrix33<Scalar>(rotMatrix.block(0, 0, 3, 3)))
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
        mQ.setFromTwoVectors(a.eigenVector(), b.eigenVector());
    }

    Scalar& w() { return mQ.w(); }

    const Scalar& w() const { return mQ.w(); }

    Scalar& x() { return mQ.x(); }

    const Scalar& x() const { return mQ.x(); }

    Scalar& y() { return mQ.y(); }

    const Scalar& y() const { return mQ.y(); }

    Scalar& z() { return mQ.z(); }

    const Scalar& z() const { return mQ.z(); }

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
            return Quaternion<S>(mQ.template cast<S>());
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
        return Quaternion<Scalar>(mQ.conjugate());
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
    Scalar dot(const Quaternion<Scalar>& q2) const { return mQ.dot(q2.mQ); }

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
        return Quaternion<Scalar>(mQ.inverse());
    }

    Scalar norm() const { return mQ.norm(); }

    Scalar squaredNorm() const { return mQ.squaredNorm(); }

    constexpr uint size() const { return 4; }

    void setIdentity() { mQ.setIdentity(); }

    void set(const Scalar& w, const Scalar& x, const Scalar& y, const Scalar& z)
    {
        mQ.w() = w;
        mQ.x() = x;
        mQ.y() = y;
        mQ.z() = z;
    }

    void setFromTwoVectors(const Point3<Scalar>& a, const Point3<Scalar>& b)
    {
        mQ.setFromTwoVectors(a.eigenVector(), b.eigenVector());
    }

    void setFromAngleAxis(const Scalar& angle, const Point3<Scalar>& axis)
    {
        mQ = Eigen::Quaternion<Scalar>(
            Eigen::AngleAxis<Scalar>(angle, axis.eigenVector()));
    }

    Quaternion<Scalar> normalized() const
    {
        return Quaternion<Scalar>(mQ.normalized());
    }

    void normalize() { mQ.normalize(); }

    const Eigen::Quaternion<Scalar>& eigenQuaternion() const { return mQ; }

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
            vcl::hashCombine(h, mQ(i));
        return h;
    }

    Scalar& operator()(uint i) { return mQ.coeffs()[i]; }

    const Scalar& operator()(uint i) const { return mQ.coeffs()[i]; }

    Scalar& operator[](uint i) { return mQ.coeffs()[i]; }

    const Scalar& operator[](uint i) const { return mQ.coeffs()[i]; }

    bool operator==(const Quaternion<Scalar>& q2) const { return mQ == q2.mQ; }

    bool operator!=(const Quaternion<Scalar>& q2) const { return mQ != q2.mQ; }

    Quaternion<Scalar> operator*(const Quaternion<Scalar>& q2) const
    {
        return Quaternion<Scalar>(mQ * q2.mQ);
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
        const Eigen::Matrix<Scalar, 1, 3>& v = p.eigenVector();

        Eigen::Matrix<Scalar, 1, 3> fc = mQ.vec().cross(v);

        Eigen::Matrix<Scalar, 1, 3> fd = v * mQ.w();

        Eigen::Matrix<Scalar, 1, 3> s = fc + fd;

        Eigen::Matrix<Scalar, 1, 3> sc = mQ.vec().cross(s);

        return Point3<Scalar>(v + sc * 2.0);
    }

    Quaternion& operator*=(const Quaternion<Scalar>& q2)
    {
        mQ *= q2.mQ;
        return *this;
    }

    Matrix33<Scalar> toRotationMatrix() const { return mQ.toRotationMatrix(); }

    /// @private
    template<typename S>
    friend std::ostream& operator<<(std::ostream& out, const Quaternion<S>& p);
};

/**
 * @brief Writes this quaternion to an output stream.
 *
 * This operator writes this quaternion to an output stream and returns the
 * output stream.
 *
 * @param[in,out] out: The output stream to write to.
 * @param[in] p1 The quaternion to write.
 * @return The output stream after the quaternion is written.
 */
template<typename Scalar>
std::ostream& operator<<(std::ostream& out, const Quaternion<Scalar>& p1)
{
    out << p1.mQ;
    return out;
}

} // namespace vcl

#endif // VCL_SPACE_QUATERNION_H
