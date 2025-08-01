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

#ifndef VCL_SPACE_CORE_POINT_H
#define VCL_SPACE_CORE_POINT_H

#include <vclib/serialization.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <compare>

namespace vcl {

/**
 * @brief The Point class represents an N-dimensional point containing N scalar
 * values.
 *
 * The Point class template represents an N-dimensional point containing N
 * scalar values. The scalar type and the number of dimensions are template
 * parameters of the class. The class provides a number of member functions for
 * accessing, manipulating, and comparing points, as well as arithmetic and
 * assignment operators for points.
 *
 * The Point class is implemented using an Eigen matrix of size Nx1 to store the
 * point components. The class also defines a number of type aliases, static
 * constants and member functions for convenience.
 *
 * @tparam Scalar: The scalar type of the point components.
 * @tparam N: The number of dimensions of the point.
 *
 * @ingroup space_core
 */
template<typename Scalar, uint N>
class Point : public Eigen::Matrix<Scalar, N, 1>
{
    using Base = Eigen::Matrix<Scalar, N, 1>;

public:
    using BaseMatrixType = Base;

    // inherit Base operators
    using Base::operator+;
    using Base::operator-;
    using Base::operator*;
    using Base::operator/;
    using Base::operator+=;
    using Base::operator-=;
    using Base::operator*=;
    using Base::operator/=;

    /**
     * @brief The Scalar type of the Point.
     */
    using ScalarType = Scalar;

    /**
     * @brief DIM: the number of dimensions of the Point
     */
    static const uint DIM = N;

    /**
     * @brief Constructs a Point object with all components set to zero.
     */
    Point() { Base::setZero(); }

    /**
     * @brief Constructs a Point object from an Eigen matrix.
     *
     * The constructor initializes the components of the Point object from an
     * Eigen matrix with the same dimensionality as the Point object.
     *
     * @param[in] other: An Eigen matrix with the same dimensionality as the
     * Point object.
     */
    template<typename OtherDerived>
    Point(const Eigen::MatrixBase<OtherDerived>& other) : Base(other)
    {
    }

    /**
     * @brief Constructs a Point object from a set of scalar values.
     *
     * The constructor takes a variable number of scalar arguments, which are
     * used to initialize the components of the Point object. The number of
     * arguments must be equal to the dimensionality of the Point object, which
     * is determined by the template parameter N.
     *
     * @tparam Scalars: The types of the scalar arguments.
     *
     * @param[in] scalars: The scalar arguments used to initialize the
     * components of the Point object.
     */
    template<typename... Scalars>
    Point(Scalars... scalars) requires (sizeof...(scalars) == N)
    {
        set(scalars...);
    }

    /**
     * @brief Returns a reference to the x-component of the Point object.
     *
     * The function returns a reference to the first component of the Point
     * object. If the Point object has fewer than one component, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the x-component of the Point object.
     */
    ScalarType& x() requires (N >= 1) { return at(0); }

    /**
     * @brief Returns a const reference to the x-component of the Point object.
     *
     * The function returns a const reference to the first component of the
     * Point object. If the Point object has fewer than one component, calling
     * this member function results in a compile-time error.
     *
     * @return A const reference to the x-component of the Point object.
     */
    const ScalarType& x() const requires (N >= 1) { return at(0); }

    /**
     * @brief Returns a reference to the y-component of the Point object.
     *
     * The function returns a reference to the second component of the Point
     * object. If the Point object has fewer than two components, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the y-component of the Point object.
     */
    ScalarType& y() requires (N >= 2) { return at(1); }

    /**
     * @brief Returns a const reference to the y-component of the Point object.
     *
     * The function returns a const reference to the second component of the
     * Point object. If the Point object has fewer than two components, calling
     * this member function results in a compile-time error.
     *
     * @return A const reference to the y-component of the Point object.
     */
    const ScalarType& y() const requires (N >= 2) { return at(1); }

    /**
     * @brief Returns a reference to the z-component of the Point object.
     *
     * The function returns a reference to the third component of the Point
     * object. If the Point object has fewer than three components, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the z-component of the Point object.
     */
    ScalarType& z() requires (N >= 3) { return at(2); }

    /**
     * @brief Returns a const reference to the z-component of the Point object.
     *
     * The function returns a const reference to the third component of the
     * Point object. If the Point object has fewer than three components,
     * calling this member function results in a compile-time error.
     *
     * @return A const reference to the z-component of the Point object.
     */
    const ScalarType& z() const requires (N >= 3) { return at(2); }

    /**
     * @brief Returns a reference to the w-component of the Point object.
     *
     * The function returns a reference to the fourth component of the Point
     * object. If the Point object has fewer than four components, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the w-component of the Point object.
     */
    ScalarType& w() requires (N >= 4) { return at(3); }

    /**
     * @brief Returns a const reference to the w-component of the Point object.
     *
     * The function returns a const reference to the fourth component of the
     * Point object. If the Point object has fewer than four components, calling
     * this member function results in a compile-time error.
     *
     * @return A const reference to the w-component of the Point object.
     */
    const ScalarType& w() const requires (N >= 4) { return at(3); }

    ScalarType& at(uint i) { return Base::operator()(i); }

    const ScalarType& at(uint i) const { return Base::operator()(i); }

    /**
     * @brief Casts the Point object to a different scalar type.
     *
     * The function returns a new Point object with the same dimension as the
     * original object, but with each scalar value casted to a different type.
     *
     * @tparam S: The scalar type to cast to.
     *
     * @return A new Point object with the same dimension as the original
     * object, but with each scalar value casted to a different type.
     */
    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            return Point<S, N>(Base::template cast<S>());
        }
    }

    /**
     * @brief Returns true if at least one of its components is NaN or inf.
     *
     * The function checks whether at least one of the scalar components of the
     * Point object is NaN (Not-a-Number) or inf (Infinity). If any component is
     * NaN or inf, the function returns true, indicating that the Point object
     * is degenerate. Otherwise, the function returns false, indicating that the
     * Point object is not degenerate.
     *
     * @return true if the Point object is degenerate, false otherwise.
     */
    bool isDegenerate() const
    {
        for (size_t i = 0; i < DIM; ++i)
            if (vcl::isDegenerate(at(i)))
                return true;
        return false;
    }

    /**
     * @brief Checks for the equality of two Point objects within a given
     * epsilon tolerance.
     *
     * The function compares two Point objects component-wise within a given
     * epsilon tolerance. If the difference between the corresponding components
     * of the two Point objects is less than or equal to the epsilon tolerance,
     * the components are considered equal, and the function returns true.
     * Otherwise, the function returns false.
     *
     * @param[in] p1: The Point object to compare against.
     * @param[in] epsilon: The epsilon tolerance to use for the comparison.
     *
     * @return true if the two Point objects are considered equal within the
     * epsilon tolerance, false otherwise.
     */
    bool epsilonEquals(
        const Point& p1,
        Scalar       epsilon = std::numeric_limits<Scalar>::epsilon()) const
    {
        bool b = true;
        for (size_t i = 0; i < DIM; ++i)
            b = b && vcl::epsilonEquals(at(i), p1(i), epsilon);
        return b;
    }

    /**
     * @brief Computes the angle between two Point objects.
     *
     * The function computes the angle between two Point objects, which is
     * defined as the inverse cosine of the dot product of the two Point objects
     * divided by the product of their magnitudes. The two Point objects must
     * have the same dimension.
     *
     * @param[in] p1: The Point object to compute the angle with.
     *
     * @return The angle between the two Point objects in radians, or -1 if the
     * magnitude of one of the Point objects is 0.
     */
    Scalar angle(const Point& p1) const
    {
        Scalar w = Base::norm() * p1.norm();
        if (w == 0)
            return -1;
        Scalar t = Base::dot(p1) / w;
        if (t > 1)
            t = 1;
        else if (t < -1)
            t = -1;
        return (Scalar) acos(t);
    }

    /**
     * @brief Computes the Euclidean distance between two Point objects.
     *
     * The function computes the Euclidean distance between two Point objects,
     * which is defined as the square root of the sum of the squares of the
     * differences of the corresponding components of the two Point objects. The
     * two Point objects must have the same dimension.
     *
     * @param[in] p1: The Point object to compute the distance to.
     *
     * @return The Euclidean distance between the two Point objects.
     */
    Scalar dist(const Point& p1) const { return (*this - p1).norm(); }

    /**
     * @brief Computes the squared Euclidean distance between two Point objects.
     *
     * The function computes the squared Euclidean distance between two Point
     * objects, which is defined as the sum of the squares of the differences of
     * the corresponding components of the two Point objects. The two Point
     * objects must have the same dimension.
     *
     * @param[in] p1: The Point object to compute the squared distance to.
     *
     * @return The squared Euclidean distance between the two Point objects.
     */
    Scalar squaredDist(const Point& p1) const
    {
        return (*this - p1).squaredNorm();
    }

    /**
     * @brief Multiplies the components of two Point objects.
     *
     * This function multiplies the components of two Point objects
     * element-wise, and returns the result as a new Point object. The two Point
     * objects must have the same dimension.
     *
     * @param[in] p1: The other Point object to multiply with.
     *
     * @return A new Point object that contains the resulting products of the
     * component-wise multiplication of the two Point objects.
     */
    Point mul(const Point& p1) const
    {
        Point<Scalar, N> tmp;
        for (size_t i = 0; i < DIM; ++i)
            tmp[i] = at(i) * p1[i];
        return tmp;
    }

    /**
     * @brief Divides the components of two Point objects.
     *
     * This function divides the components of two Point objects element-wise,
     * and returns the result as a new Point object. The two Point objects must
     * have the same dimension. If any component of the second Point object is
     * zero, the function throws a std::runtime_error exception.
     *
     * @param[in] p1: The other Point object to divide with.
     *
     * @return A new Point object that contains the resulting quotients of the
     * component-wise division of the two Point objects.
     *
     * @throws std::runtime_error if any component of the second Point object is
     * zero.
     */
    Point div(const Point& p1) const
    {
        Point<Scalar, N> tmp;
        for (size_t i = 0; i < DIM; ++i) {
            if (p1[i] == 0)
                throw std::runtime_error(
                    "Math error: Attempted to divide by Zero\n");
            tmp[i] = at(i) / p1[i];
        }
        return tmp;
    }

    /**
     * @brief Returns the size of the Point object.
     *
     * This function returns the size (number of components) of the Point
     * object.
     *
     * @return The size of the Point object.
     */
    constexpr uint size() const { return N; }

    /**
     * @brief Sets all the components of the Point object from a set of scalar
     * values.
     *
     * The member function takes a variable number of scalar arguments, which
     * are used to set the components of the Point object. The number of
     * arguments must be equal to the dimensionality of the Point object, which
     * is determined by the template parameter N.
     *
     * @tparam Scalars: The types of the scalar arguments.
     *
     * @param[in] scalars: The scalar arguments used to set the
     * components of the Point object.
     */
    template<typename... Scalars>
    void set(Scalars... scalars) requires (sizeof...(scalars) == N)
    {
        Scalar args[N] = {static_cast<Scalar>(scalars)...};
        for (uint i = 0; i < N; i++)
            at(i) = args[i];
    }

    /**
     * @brief Returns the outer product between this point p and p1, which is
     * p * p1^T
     *
     * The returned type is a DIM*DIM Eigen Matrix, where DIM is the number of
     * dimensions of the two points.
     *
     * @param p1 The Point object to compute the outer product with.
     *
     * @return A DIM*DIM Eigen Matrix representing the outer product between
     * this point p and p1.
     */
    auto outerProduct(const Point& p1) const
    {
        Eigen::Matrix<ScalarType, DIM, DIM> res;
        for (uint i = 0; i < DIM; i++) {
            for (uint j = 0; j < DIM; j++) {
                res(i, j) = at(i) * p1(j);
            }
        }
        return res;
    }

    /**
     * @brief Computes an [Orthonormal
     * Basis](https://en.wikipedia.org/wiki/Orthonormal_basis) starting from
     * this point n.
     *
     * This function computes an Orthonormal Basis starting from this point n.
     * The orthonormal basis is composed of three vectors: the input vector n
     * and two output vectors u and v. The two output vectors are orthogonal to
     * n and each other, and have unit length.
     *
     * This function is available only on Points having size == 3.
     *
     * @param[out] u The first output vector of the orthonormal basis,
     * orthogonal to n and v.
     * @param[out] v The second output vector of the orthonormal basis,
     * orthogonal to n and u.
     *
     * @note This function uses the cross product operation, which is only
     * defined for 3D vectors.
     */
    void orthoBase(Point& u, Point& v) const requires (N == 3)
    {
        const double locEps = double(1e-7);

        Point<Scalar, 3> up(0, 1, 0);
        u = Base::cross(up);

        double len = u.norm();
        if (len < locEps) {
            if (std::abs(at(0)) < std::abs(at(1))) {
                if (std::abs(at(0)) < std::abs(at(2)))
                    up = Point<Scalar, 3>(1, 0, 0); // x is the min
                else
                    up = Point<Scalar, 3>(0, 0, 1); // z is the min
            }
            else {
                if (std::abs(at(1)) < std::abs(at(2)))
                    up = Point<Scalar, 3>(0, 1, 0); // y is the min
                else
                    up = Point<Scalar, 3>(0, 0, 1); // z is the min
            }
            u = Base::cross(up);
        }
        v = Base::cross(u);
    }

    /**
     * @brief Serializes the point to the given output stream.
     * @param[in] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        vcl::serializeN(os, Base::data(), N);
    }

    /**
     * @brief Deserializes the point from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        vcl::deserializeN(is, Base::data(), N);
    }

    /**
     * @brief Computes the hash value of the point.
     *
     * This function computes a hash value of the point, which can be used for
     * hashing and comparison purposes.
     *
     * @return The hash value of the point.
     */
    std::size_t hash() const
    {
        std::size_t h = 0;
        for (size_t i = 0; i < DIM; ++i)
            hashCombine(h, at(i));
        return h;
    }

    /**
     * @brief Assigns the point to the given Eigen matrix.
     * @param[in] other: The Eigen matrix to assign to.
     * @return A reference to the assigned point.
     */
    template<typename OtherDerived>
    Point& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
        this->Base::operator=(other);
        return *this;
    }

    /**
     * @brief Compares the point with another point using the spaceship
     * operator.
     *
     * This operator compares the point with another point using the spaceship
     * operator. The comparison is performed by comparing the coordinates of the
     * two points in lexicographic order. The comparison stops as soon as a pair
     * of coordinates that are not equal is found. If all coordinates are equal,
     * the comparison returns std::strong_ordering::equal.
     *
     * @param[in] p1: The point to compare with.
     * @return A three-way comparison result indicating the relative order of
     * the two points.
     */
    auto operator<=>(const Point& p1) const
    {
        uint i = 0;
        while (i < DIM && at(i) == p1[i]) {
            ++i;
        }
        return i == DIM ? std::strong_ordering::equal : at(i) <=> p1[i];
    }

    /**
     * @brief Adds a scalar value to each coordinate of the point.
     *
     * This operator adds a scalar value to each coordinate of the point and
     * returns the resulting point. The scalar value is added component-wise to
     * each coordinate of the point.
     *
     * @param[in] s: The scalar value to add.
     * @return The point obtained by adding the scalar value to each coordinate
     * of the point.
     */
    Point operator+(const Scalar& s) const { return Base(Base::array() + s); }

    /**
     * @brief Subtracts a scalar value from each coordinate of the point.
     *
     * This operator subtracts a scalar value from each coordinate of the point
     * and returns the resulting point. The scalar value is subtracted
     * component-wise from each coordinate of the point.
     *
     * @param[in] s: The scalar value to subtract.
     * @return The point obtained by subtracting the scalar value from each
     * coordinate of the point.
     */
    Point operator-(const Scalar& s) const { return Base(Base::array() - s); }

    /**
     * @brief Computes the dot product of this point with another point.
     *
     * This operator computes the dot product of this point with another point
     * and returns the resulting scalar value. The dot product is computed by
     * taking the component-wise product of the coordinates of the two points
     * and summing the products.
     *
     * @param[in] p1: The point to compute the dot product with.
     * @return The dot product of this point with the other point.
     */
    Scalar operator*(const Point& p1) const { return Base::dot(p1); }

    /**
     * @brief Returns a new 3D point/vector on which has been applied a TRS 4x4
     * matrix.
     *
     * This operator returns a new 3D point/vector obtained by applying a 4x4
     * TRS matrix to this point/vector. The TRS matrix is a combination of a
     * translation, rotation, and scaling transformation. This operator is
     * available only for points of size 3.
     *
     * @param[in] m: The TRS matrix to apply.
     * @return The new 3D point/vector obtained by applying the TRS matrix to
     * this point/vector.
     *
     * @note This function is available only on Points having size == 3.
     */
    Point operator*(const Eigen::Matrix<Scalar, N + 1, N + 1>& m) const
        requires (N == 3)
    {
        Eigen::Matrix<Scalar, 1, N> s;

        s(0) = m(0, 0) * at(0) + m(0, 1) * at(1) + m(0, 2) * at(2) + m(0, 3);
        s(1) = m(1, 0) * at(0) + m(1, 1) * at(1) + m(1, 2) * at(2) + m(1, 3);
        s(2) = m(2, 0) * at(0) + m(2, 1) * at(1) + m(2, 2) * at(2) + m(2, 3);

        Scalar w =
            at(0) * m(3, 0) + at(1) * m(3, 1) + at(2) * m(3, 2) + m(3, 3);
        if (w != 0) [[likely]]
            s = s / w;
        return Point(s);
    }

    /**
     * @brief Adds a scalar value to this point.
     *
     * This operator adds a scalar value to each coordinate of this point and
     * returns a reference to this point. The addition is performed by adding
     * the scalar value to each coordinate of the point.
     *
     * @param[in] s: The scalar value to add.
     * @return A reference to this point after the addition operation.
     */
    Point& operator+=(const Scalar& s)
    {
        *this = *this + s;
        return *this;
    }

    /**
     * @brief Subtracts a scalar value from this point.
     *
     * This operator subtracts a scalar value from each coordinate of this point
     * and returns a reference to this point. The subtraction is performed by
     * subtracting the scalar value from each coordinate of the point.
     *
     * @param[in] s: The scalar value to subtract.
     * @return A reference to this point after the subtraction operation.
     */
    Point& operator-=(const Scalar& s)
    {
        *this = *this - s;
        return *this;
    }

    /**
     * @brief Applies a TRS 4x4 matrix transformation to this point.
     *
     * This operator applies a TRS 4x4 matrix transformation to this point and
     * returns a reference to this point. The transformation is performed by
     * multiplying the TRS 4x4 matrix with the homogeneous coordinate
     * representation of this point.
     *
     * @param[in] m: The TRS 4x4 matrix to apply.
     * @return A reference to this point after the transformation.
     *
     * @note This function is available only on Points having size == 3.
     */
    Point& operator*=(const Eigen::Matrix<Scalar, N + 1, N + 1>& m)
        requires (N == 3)
    {
        *this = *this * m;
        return *this;
    }

private:
    // hide Base begin and end members
    using Base::begin;
    using Base::end;
};

/* Specialization Aliases */

/**
 * @brief A convenience alias for a 2-dimensional Point.
 *
 * The Point2 alias is a shorthand for a Point class template specialization
 * with a scalar type of Scalar and two dimensions. It is implemented as an
 * alias template for the Point class template.
 *
 * @tparam Scalar: The scalar type of the point components.
 *
 * @ingroup space_core
 */
template<typename Scalar>
using Point2 = Point<Scalar, 2>;

/**
 * @brief A convenience alias for a 2-dimensional Point with integer components.
 *
 * The Point2i alias is a shorthand for a Point class template specialization
 * with integer components and two dimensions. It is implemented as an alias
 * template for the Point2 alias template.
 *
 * @ingroup space_core
 */
using Point2i = Point2<int>;

/**
 * @brief A convenience alias for a 2-dimensional Point with floating-point
 * components.
 *
 * The Point2f alias is a shorthand for a Point class template specialization
 * with floating-point components and two dimensions. It is implemented as an
 * alias template for the Point2 alias template.
 *
 * @ingroup space_core
 */
using Point2f = Point2<float>;

/**
 * @brief A convenience alias for a 2-dimensional Point with double-precision
 * floating-point components.
 *
 * The Point2d alias is a shorthand for a Point class template specialization
 * with double-precision floating-point components and two dimensions. It is
 * implemented as an alias template for the Point2 alias template.
 *
 * @ingroup space_core
 */
using Point2d = Point2<double>;

/**
 * @brief A convenience alias for a 3-dimensional Point.
 *
 * The Point3 alias is a shorthand for a Point class template specialization
 * with a scalar type of Scalar and three dimensions. It is implemented as an
 * alias template for the Point class template.
 *
 * @tparam Scalar: The scalar type of the point components.
 *
 * @ingroup space_core
 */
template<typename Scalar>
using Point3 = Point<Scalar, 3>;

/**
 * @brief A convenience alias for a 3-dimensional Point with integer components.
 *
 * The Point3i alias is a shorthand for a Point class template specialization
 * with integer components and three dimensions. It is implemented as an alias
 * template for the Point3 alias template.
 *
 * @ingroup space_core
 */
using Point3i = Point3<int>;

/**
 * @brief A convenience alias for a 3-dimensional Point with floating-point
 * components.
 *
 * The Point3f alias is a shorthand for a Point class template specialization
 * with floating-point components and three dimensions. It is implemented as an
 * alias template for the Point3 alias template.
 *
 * @ingroup space_core
 */
using Point3f = Point3<float>;

/**
 * @brief A convenience alias for a 3-dimensional Point with double-precision
 * floating-point components.
 *
 * The Point3d alias is a shorthand for a Point class template specialization
 * with double-precision floating-point components and three dimensions. It is
 * implemented as an alias template for the Point3 alias template.
 *
 * @ingroup space_core
 */
using Point3d = Point3<double>;

/**
 * @brief A convenience alias for a 4-dimensional Point.
 *
 * The Point4 alias is a shorthand for a Point class template specialization
 * with a scalar type of Scalar and four dimensions. It is implemented as an
 * alias template for the Point class template.
 *
 * @tparam Scalar: The scalar type of the point components.
 *
 * @ingroup space_core
 */
template<typename Scalar>
using Point4 = Point<Scalar, 4>;

/**
 * @brief A convenience alias for a 4-dimensional Point with integer components.
 *
 * The Point4i alias is a shorthand for a Point class template specialization
 * with integer components and four dimensions. It is implemented as an alias
 * template for the Point4 alias template.
 *
 * @ingroup space_core
 */
using Point4i = Point4<int>;

/**
 * @brief A convenience alias for a 4-dimensional Point with floating-point
 * components.
 *
 * The Point4f alias is a shorthand for a Point class template specialization
 * with floating-point components and four dimensions. It is implemented as an
 * alias template for the Point4 alias template.
 *
 * @ingroup space_core
 */
using Point4f = Point4<float>;

/**
 * @brief A convenience alias for a 4-dimensional Point with double-precision
 * floating-point components.
 *
 * The Point4d alias is a shorthand for a Point class template specialization
 * with double-precision floating-point components and four dimensions. It is
 * implemented as an alias template for the Point4 alias template.
 *
 * @ingroup space_core
 */
using Point4d = Point4<double>;

/* Concepts */

/**
 * @brief A concept representing a Point.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Point class having any scalar type and any dimension.
 *
 * @tparam T: The type to be tested for conformity to the PointConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept PointConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Point<typename RemoveRef<T>::ScalarType, RemoveRef<T>::DIM>>;

/**
 * @brief A concept representing a 2D Point.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Point class having any scalar type and dimension 2.
 *
 * @tparam T: The type to be tested for conformity to the Point2Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept Point2Concept = PointConcept<T> && RemoveRef<T>::DIM == 2;

/**
 * @brief A concept representing a 3D Point.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Point class having any scalar type and dimension 3.
 *
 * @tparam T: The type to be tested for conformity to the Point4Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept Point3Concept = PointConcept<T> && RemoveRef<T>::DIM == 3;

/**
 * @brief A concept representing a 4D Point.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Point class having any scalar type and dimension 4.
 *
 * @tparam T: The type to be tested for conformity to the Point4Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept Point4Concept = PointConcept<T> && RemoveRef<T>::DIM == 4;

/**
 * @brief A concept representing an iterator that iterates over Points
 * (specifically, a class that satisfies the PointConcept).
 *
 * The concept is satisfied when `T` is an iterator having its `value_type`
 * that satisfies the vcl::PointConcept.
 *
 * @tparam It: The type to be tested for conformity to the PointIteratorConcept.
 *
 * @ingroup space_core
 */
template<typename It>
concept PointIteratorConcept =
    IteratorConcept<It> && PointConcept<typename It::value_type>;

/**
 * @brief A concept representing an iterator that iterates over 2D Points
 * (specifically, a class that satisfies the Point2Concept).
 *
 * The concept is satisfied when `T` is an iterator having its `value_type`
 * that satisfies the vcl::Point2Concept.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point2IteratorConcept.
 *
 * @ingroup space_core
 */
template<typename It>
concept Point2IteratorConcept =
    IteratorConcept<It> && Point2Concept<typename It::value_type>;

/**
 * @brief A concept representing an iterator that iterates over 3D Points
 * (specifically, a class that satisfies the Point3Concept).
 *
 * The concept is satisfied when `T` is an iterator having its `value_type`
 * that satisfies the vcl::Point3Concept.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point3IteratorConcept.
 *
 * @ingroup space_core
 */
template<typename It>
concept Point3IteratorConcept =
    IteratorConcept<It> && Point3Concept<typename It::value_type>;

/**
 * @brief A concept representing an iterator that iterates over 4D Points
 * (specifically, a class that satisfies the Point4Concept).
 *
 * The concept is satisfied when `T` is an iterator having its `value_type`
 * that satisfies the vcl::Point4Concept.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point4IteratorConcept.
 *
 * @ingroup space_core
 */
template<typename It>
concept Point4IteratorConcept =
    IteratorConcept<It> && Point4Concept<typename It::value_type>;

/* Utility functions */

/**
 * @brief Compares two points for equality.
 *
 * This function is a specialization of the epsilonEquals function template for
 * two points. It compares two points for equality by comparing each coordinate
 * of the points for equality within a specified epsilon value.
 *
 * @param[in] p1: The first point to compare.
 * @param[in] p2: The second point to compare.
 * @param[in] epsilon: The epsilon value for equality comparison.
 * @return True if the points are equal within the epsilon value, false
 * otherwise.
 */
template<typename Scalar, uint N>
bool epsilonEquals(
    const Point<Scalar, N>& p1,
    const Point<Scalar, N>& p2,
    const Scalar&           epsilon = std::numeric_limits<Scalar>::epsilon())
{
    return p1.epsilonEquals(p2, epsilon);
}

/**
 * @brief Returns the minimum between two points.
 *
 * This function returns a point that contains the minimum value between each
 * component of the two input points.
 *
 * @param[in] p1: The first point.
 * @param[in] p2: The second point.
 * @return The point containing the minimum value between the two input points.
 */
template<typename Scalar, uint N>
constexpr Point<Scalar, N> min(
    const Point<Scalar, N>& p1,
    const Point<Scalar, N>& p2)
{
    Point<Scalar, N> p;
    for (size_t i = 0; i < p.DIM; i++) {
        p[i] = std::min(p1[i], p2[i]);
    }
    return p;
}

/**
 * @brief Returns the maximum between two points.
 *
 * This function returns a point that contains the maximum value between each
 * component of the two input points.
 *
 * @param[in] p1: The first point.
 * @param[in] p2: The second point.
 * @return The point containing the maximum value between the two input points.
 */
template<typename Scalar, uint N>
constexpr Point<Scalar, N> max(
    const Point<Scalar, N>& p1,
    const Point<Scalar, N>& p2)
{
    Point<Scalar, N> p;
    for (size_t i = 0; i < p.DIM; i++) {
        p[i] = std::max(p1[i], p2[i]);
    }
    return p;
}

/**
 * @brief Stream insertion operator for Point objects.
 *
 * This operator allows Point objects to be printed to output streams in a
 * single row format, rather than the default column format used by Eigen.
 * The output format is: [x, y, z, ...] where the components are separated
 * by commas and spaces.
 *
 * @tparam Scalar: The scalar type of the point components.
 * @tparam N: The number of dimensions of the point.
 * @param[in] os: The output stream to write to.
 * @param[in] p: The Point object to write.
 * @return A reference to the output stream.
 */
template<typename Scalar, uint N>
std::ostream& operator<<(std::ostream& os, const Point<Scalar, N>& p)
{
    // Create a custom IOFormat for single row output
    // Parameters: precision, flags, coeffSeparator, rowSeparator, rowPrefix,
    // rowSuffix, matPrefix, matSuffix
    static const Eigen::IOFormat rowFormat(
        Eigen::StreamPrecision, // precision (use stream's precision)
        Eigen::DontAlignCols,   // flags
        ", ",                   // coeffSeparator (between elements)
        ", ",                   // rowSeparator (not used for vectors)
        "",                     // rowPrefix
        "",                     // rowSuffix
        "[",                    // matPrefix
        "]"                     // matSuffix
    );

    return os << static_cast<const typename Point<Scalar, N>::BaseMatrixType&>(
                     p)
                     .format(rowFormat);
}

/* Deduction guides */

template<typename S, typename... Scalars>
Point(S, Scalars... scalars) -> Point<S, sizeof...(Scalars) + 1>;

} // namespace vcl

// inject vcl::Point hash function in std namespace
namespace std {

template<typename Scalar, unsigned int N>
struct hash<vcl::Point<Scalar, N>>
{
    /**
     * @brief Computes the hash value for a Point object.
     *
     * This function computes the hash value for a Point object by calling the
     * Point's own hash() function.
     *
     * @param[in] id: The Point object to compute the hash value for.
     * @return The hash value for the Point object.
     */
    size_t operator()(const vcl::Point<Scalar, N>& id) const noexcept
    {
        return id.hash();
    }
};

} // namespace std

#endif // VCL_SPACE_CORE_POINT_H
