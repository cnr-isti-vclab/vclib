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

#ifndef VCL_SPACE_POINT_H
#define VCL_SPACE_POINT_H

#include <compare>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vclib/concepts/space/point.h>
#include <vclib/math/base.h>
#include <vclib/misc/hash.h>

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
 * The Point class is implemented using an Eigen matrix of size 1xN to store the
 * point components. The class also defines a number of type aliases and static
 * constants for convenience.
 *
 * @tparam Scalar: The scalar type of the point components.
 * @tparam N: The number of dimensions of the point.
 *
 * @ingroup space
 */
template<typename Scalar, uint N>
class Point
{
    template<typename S, uint M>
    friend class Point;

protected:
    Eigen::Matrix<Scalar, 1, N> mP = Eigen::Matrix<Scalar, 1, N>::Zero();

public:
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
    Point() = default;

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
     * @brief Constructs a Point object from an Eigen row vector.
     *
     * The constructor initializes the components of the Point object from an
     * Eigen row vector with the same dimensionality as the Point object.
     *
     * @param[in] v: An Eigen row vector with the same dimensionality as the
     * Point object.
     */
    Point(const Eigen::Matrix<Scalar, 1, N>& v) { mP << v; }

    /**
     * @brief Returns a reference to the x-component of the Point object.
     *
     * The function returns a reference to the first component of the Point
     * object. If the Point object has fewer than one component, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the x-component of the Point object.
     */
    ScalarType& x() requires (N >= 1) { return mP(0); }

    /**
     * @brief Returns a const reference to the x-component of the Point object.
     *
     * The function returns a const reference to the first component of the
     * Point object. If the Point object has fewer than one component, calling
     * this member function results in a compile-time error.
     *
     * @return A const reference to the x-component of the Point object.
     */
    const ScalarType& x() const requires (N >= 1) { return mP(0); }

    /**
     * @brief Returns a reference to the y-component of the Point object.
     *
     * The function returns a reference to the second component of the Point
     * object. If the Point object has fewer than two components, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the y-component of the Point object.
     */
    ScalarType& y() requires (N >= 2) { return mP(1); }

    /**
     * @brief Returns a const reference to the y-component of the Point object.
     *
     * The function returns a const reference to the second component of the
     * Point object. If the Point object has fewer than two components, calling
     * this member function results in a compile-time error.
     *
     * @return A const reference to the y-component of the Point object.
     */
    const ScalarType& y() const requires (N >= 2) { return mP(1); }

    /**
     * @brief Returns a reference to the z-component of the Point object.
     *
     * The function returns a reference to the third component of the Point
     * object. If the Point object has fewer than three components, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the z-component of the Point object.
     */
    ScalarType& z() requires (N >= 3) { return mP(2); }

    /**
     * @brief Returns a const reference to the z-component of the Point object.
     *
     * The function returns a const reference to the third component of the
     * Point object. If the Point object has fewer than three components,
     * calling this member function results in a compile-time error.
     *
     * @return A const reference to the z-component of the Point object.
     */
    const ScalarType& z() const requires (N >= 3) { return mP(2); }

    /**
     * @brief Returns a reference to the w-component of the Point object.
     *
     * The function returns a reference to the fourth component of the Point
     * object. If the Point object has fewer than four components, calling this
     * member function results in a compile-time error.
     *
     * @return A reference to the w-component of the Point object.
     */
    ScalarType& w() requires (N >= 4) { return mP(3); }

    /**
     * @brief Returns a const reference to the w-component of the Point object.
     *
     * The function returns a const reference to the fourth component of the
     * Point object. If the Point object has fewer than four components, calling
     * this member function results in a compile-time error.
     *
     * @return A const reference to the w-component of the Point object.
     */
    const ScalarType& w() const requires (N >= 4) { return mP(3); }

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
            return Point<S, N>(mP.template cast<S>());
        }
    }

    /**
     * @brief Returns the pointer to the underlying scalar storage.
     *
     * @return The pointer to the underlying scalar storage.
     */
    const ScalarType* data() const { return mP.data(); }

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
            if (isDegenerate(mP(i)))
                return true;
        return false;
    }

    /**
     * @brief Compares two Point objects within a given epsilon tolerance.
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
    bool epsilonCompare(const Point& p1, Scalar epsilon)
    {
        bool b = true;
        for (size_t i = 0; i < DIM; ++i)
            b = b && vcl::epsilonCompare(mP(i), p1(i), epsilon);
        return b;
    }

    /**
     * @brief Computes the dot product of two Point objects.
     *
     * The function computes the dot product of two Point objects, which is
     * defined as the sum of the products of the corresponding components of the
     * two Point objects. The two Point objects must have the same dimension.
     *
     * @param[in] p1: The Point object to compute the dot product with.
     *
     * @return The dot product of the two Point objects.
     */
    Scalar dot(const Point& p1) const { return mP.dot(p1.mP); }

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
        Scalar w = mP.norm() * p1.mP.norm();
        if (w == 0)
            return -1;
        Scalar t = dot(p1) / w;
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
    Scalar dist(const Point& p1) const { return (mP - p1.mP).norm(); }

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
        return (mP - p1.mP).squaredNorm();
    }

    /**
     * @brief Returns the cross product between two points.
     *
     * This function returns the cross product between two points. The two
     * points must have the same dimension, and this function is available only
     * on Points having size == 3.
     *
     * @param[in] p1: The other point to compute the cross product with.
     *
     * @return The cross product between this and the other point.
     *
     * @note This function is available only on Points having size == 3.
     */
    Point cross(const Point& p1) const requires (N == 3)
    {
        return mP.cross(p1.mP);
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
            tmp[i] = mP[i] * p1.mP[i];
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
            if (p1.mP[i] == 0)
                throw std::runtime_error(
                    "Math error: Attempted to divide by Zero\n");
            tmp[i] = mP[i] / p1.mP[i];
        }
        return tmp;
    }

    /**
     * @brief Computes the Euclidean norm of the Point object.
     *
     * This function computes the Euclidean norm (magnitude) of the Point
     * object, which is defined as the square root of the sum of the squares of
     * its components.
     *
     * @return The Euclidean norm of the Point object.
     */
    Scalar norm() const { return mP.norm(); }

    /**
     * @brief Computes the squared Euclidean norm of the Point object.
     *
     * This function computes the squared Euclidean norm (magnitude squared) of
     * the Point object, which is defined as the sum of the squares of its
     * components.
     *
     * @return The squared Euclidean norm of the Point object.
     */
    Scalar squaredNorm() const { return mP.squaredNorm(); }

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
     * @brief Sets all the components of the Point object to a constant value.
     *
     * This function sets all the components of the Point object to a constant
     * value specified by the input parameter.
     *
     * @param s The constant value to set all the components of the Point object
     * to.
     */
    void setConstant(Scalar s) { mP.setConstant(s); }

    /**
     * @brief Sets all the components of the Point object to zero.
     *
     * This function sets all the components of the Point object to zero.
     */
    void setZero() { mP.setZero(); }

    /**
     * @brief Sets all the components of the Point object to one.
     *
     * This function sets all the components of the Point object to one.
     */
    void setOnes() { mP.setOnes(); }

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
            mP(i) = args[i];
    }

    /**
     * @brief Returns a normalized copy of the Point object.
     *
     * This function returns a normalized copy of the Point object, which has
     * the same direction as the original Point object but a magnitude of 1.
     *
     * @return A normalized copy of the Point object.
     *
     * @throws std::runtime_error if the norm of the Point object is zero.
     *
     * @sa norm()
     */
    Point<Scalar, N> normalized() const
    {
        if (norm() == 0)
            throw std::runtime_error(
                "Math error: Attempted to divide by Zero\n");

        return Point<Scalar, N>(mP.array() / norm());
    }

    /**
     * @brief Normalizes the Point object in-place.
     *
     * This function normalizes the Point object in-place, which means that the
     * direction of the Point object is preserved but its magnitude is changed
     * to 1.
     *
     * @throws std::runtime_error if the norm of the Point object is zero.
     *
     * @sa norm()
     */
    void normalize()
    {
        if (norm() == 0)
            throw std::runtime_error(
                "Math error: Attempted to divide by Zero\n");

        mP /= norm();
    }

    /**
     * @brief Returns the outer product between this point mP and p1, which is mP
     * * p1^T
     *
     * The returned type is a DIM*DIM Eigen Matrix, where DIM is the number of
     * dimensions of the two points.
     *
     * @param p1 The Point object to compute the outer product with.
     *
     * @return A DIM*DIM Eigen Matrix representing the outer product between
     * this point mP and p1.
     */
    auto outerProduct(const Point& p1) const
    {
        Eigen::Matrix<ScalarType, DIM, DIM> res;
        for (uint i = 0; i < DIM; i++) {
            for (uint j = 0; j < DIM; j++) {
                res(i, j) = mP(i) * p1(j);
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
        u = cross(up);

        double len = u.norm();
        if (len < locEps) {
            if (std::abs(mP[0]) < std::abs(mP[1])) {
                if (std::abs(mP[0]) < std::abs(mP[2]))
                    up = Point<Scalar, 3>(1, 0, 0); // x is the min
                else
                    up = Point<Scalar, 3>(0, 0, 1); // z is the min
            }
            else {
                if (std::abs(mP[1]) < std::abs(mP[2]))
                    up = Point<Scalar, 3>(0, 1, 0); // y is the min
                else
                    up = Point<Scalar, 3>(0, 0, 1); // z is the min
            }
            u = cross(up);
        }
        v = cross(u);
    }

    /**
     * @brief Returns the Eigen Vector representation of the point.
     *
     * This function returns an Eigen Vector representation of the point, which
     * is a one-row matrix with N columns.
     *
     * @return An Eigen Vector representation of the point.
     */
    const Eigen::Matrix<Scalar, 1, N>& eigenVector() const { return mP; }

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
            vcl::hashCombine(h, mP(i));
        return h;
    }

    /**
     * @brief Provides access to the i-th coordinate of the point.
     *
     * This operator provides read and write access to the i-th coordinate of
     * the point. The index i is zero-based and must be less than the dimension
     * of the point.
     *
     * @param[in] i: The index of the coordinate to access.
     * @return A reference to the i-th coordinate of the point.
     *
     * @note The behavior of this operator is undefined if i is greater than or
     * equal to the dimension of the point.
     */
    Scalar& operator()(uint i) { return mP(i); }

    /**
     * @brief Provides read-only access to the i-th coordinate of the point.
     *
     * This operator provides read-only access to the i-th coordinate of the
     * point. The index i is zero-based and must be less than the dimension of
     * the point.
     *
     * @param[in] i: The index of the coordinate to access.
     * @return A const reference to the i-th coordinate of the point.
     *
     * @note The behavior of this operator is undefined if i is greater than or
     * equal to the dimension of the point.
     */
    const Scalar& operator()(uint i) const { return mP(i); }

    /**
     * @brief Provides access to the i-th coordinate of the point.
     *
     * This operator provides read and write access to the i-th coordinate of
     * the point. The index i is zero-based and must be less than the dimension
     * of the point.
     *
     * @param[in] i: The index of the coordinate to access.
     * @return A reference to the i-th coordinate of the point.
     *
     * @note The behavior of this operator is undefined if i is greater than or
     * equal to the dimension of the point.
     */
    Scalar& operator[](uint i) { return mP(i); }

    /**
     * @brief Provides read-only access to the i-th coordinate of the point.
     *
     * This operator provides read-only access to the i-th coordinate of the
     * point. The index i is zero-based and must be less than the dimension of
     * the point.
     *
     * @param[in] i: The index of the coordinate to access.
     * @return A const reference to the i-th coordinate of the point.
     *
     * @note The behavior of this operator is undefined if i is greater than or
     * equal to the dimension of the point.
     */
    const Scalar& operator[](uint i) const { return mP(i); }

    bool operator==(const Point& p1) const = default;

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
        while (i < DIM && mP[i] == p1.mP[i]) {
            ++i;
        }
        return i == DIM ? std::strong_ordering::equal : mP[i] <=> p1.mP[i];
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
    Point operator+(const Scalar& s) const
    {
        return Point<Scalar, N>(mP.array() + s);
    }

    /**
     * @brief Adds another point to this point.
     *
     * This operator adds another point to this point and returns the resulting
     * point. The addition is performed component-wise on each coordinate of the
     * two points.
     *
     * @param[in] p1: The point to add.
     * @return The point obtained by adding the other point to this point.
     */
    Point operator+(const Point& p1) const
    {
        return Point<Scalar, N>(mP + p1.mP);
    }

    /**
     * @brief Negates each coordinate of the point.
     *
     * This operator negates each coordinate of the point and returns the
     * resulting point. The negation is performed component-wise on each
     * coordinate of the point.
     *
     * @return The point obtained by negating each coordinate of the point.
     */
    Point operator-() const { return Point<Scalar, N>(-mP); }

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
    Point operator-(const Scalar& s) const
    {
        return Point<Scalar, N>(mP.array() - s);
    }

    /**
     * @brief Subtracts another point from this point.
     *
     * This operator subtracts another point from this point and returns the
     * resulting point. The subtraction is performed component-wise on each
     * coordinate of the two points.
     *
     * @param[in] p1: The point to subtract.
     * @return The point obtained by subtracting the other point from this
     * point.
     */
    Point operator-(const Point& p1) const
    {
        return Point<Scalar, N>(mP - p1.mP);
    }

    /**
     * @brief Multiplies each coordinate of the point by a scalar value.
     *
     * This operator multiplies each coordinate of the point by a scalar value
     * and returns the resulting point. The multiplication is performed
     * component-wise on each coordinate of the point.
     *
     * @param[in] s: The scalar value to multiply by.
     * @return The point obtained by multiplying each coordinate of the point by
     * the scalar value.
     */
    Point operator*(const Scalar& s) const { return Point<Scalar, N>(mP * s); }

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
    Scalar operator*(const Point& p1) const { return dot(p1); }

    /**
     * @brief Multiplies this point by a matrix.
     *
     * This operator multiplies this point by a matrix and returns the resulting
     * point. The multiplication is performed by treating the point as a row
     * vector and the matrix as a linear transformation that acts on the vector
     * from the left.
     *
     * @param[in] m: The matrix to multiply by.
     * @return The point obtained by multiplying this point by the matrix.
     */
    Point operator*(const Eigen::Matrix<Scalar, N, N>& m) const
    {
        return Point<Scalar, N>(mP * m);
    }

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

        s(0) = m(0, 0) * mP(0) + m(0, 1) * mP(1) + m(0, 2) * mP(2) + m(0, 3);
        s(1) = m(1, 0) * mP(0) + m(1, 1) * mP(1) + m(1, 2) * mP(2) + m(1, 3);
        s(2) = m(2, 0) * mP(0) + m(2, 1) * mP(1) + m(2, 2) * mP(2) + m(2, 3);

        Scalar w = mP(0) * m(3, 0) + mP(1) * m(3, 1) + mP(2) * m(3, 2) + m(3, 3);
        if (w != 0)
            s = s / w;
        return Point(s);
    }

    /**
     * @brief Divides this point by a scalar.
     *
     * This operator divides this point by a scalar and returns the resulting
     * point. The division is performed by dividing each coordinate of the point
     * by the scalar.
     *
     * @param[in] s: The scalar to divide by.
     * @return The point obtained by dividing this point by the scalar.
     *
     * @throws std::runtime_error If the scalar is zero.
     */
    Point operator/(const Scalar& s) const
    {
        if (s == 0)
            throw std::runtime_error(
                "Math error: Attempted to divide by Zero\n");
        return Point<Scalar, N>(mP / s);
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
        mP = mP.array() + s;
        return *this;
    }

    /**
     * @brief Adds a point to this point.
     *
     * This operator adds each coordinate of another point to the corresponding
     * coordinate of this point and returns a reference to this point. The
     * addition is performed by adding the coordinates of the other point to the
     * coordinates of this point element-wise.
     *
     * @param[in] p1: The point to add to this point.
     * @return A reference to this point after the addition operation.
     */
    Point& operator+=(const Point& p1)
    {
        mP += p1.mP;
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
        mP = mP.array() - s;
        return *this;
    }

    /**
     * @brief Subtracts a point from this point.
     *
     * This operator subtracts each coordinate of another point from the
     * corresponding coordinate of this point and returns a reference to this
     * point. The subtraction is performed by subtracting the coordinates of the
     * other point from the coordinates of this point element-wise.
     *
     * @param[in] p1: The point to subtract from this point.
     * @return A reference to this point after the subtraction operation.
     */
    Point& operator-=(const Point& p1)
    {
        mP -= p1.mP;
        return *this;
    }

    /**
     * @brief Multiplies this point by a scalar value.
     *
     * This operator multiplies each coordinate of this point by a scalar value
     * and returns a reference to this point. The multiplication is performed by
     * multiplying the scalar value with each coordinate of the point.
     *
     * @param[in] s: The scalar value to multiply by.
     * @return A reference to this point after the multiplication operation.
     */
    Point& operator*=(const Scalar& s)
    {
        mP *= s;
        return *this;
    }

    /**
     * @brief Multiplies this point by a square matrix.
     *
     * This operator multiplies this point by a square matrix and returns a
     * reference to this point. The multiplication is performed by multiplying
     * the square matrix with the column vector representation of this point.
     *
     * @param[in] m: The square matrix to multiply by.
     * @return A reference to this point after the multiplication operation.
     */
    Point& operator*=(const Eigen::Matrix<Scalar, N, N>& m)
    {
        mP *= m;
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

    /**
     * @brief Divides this point by a scalar value.
     *
     * This operator divides each coordinate of this point by a scalar value and
     * returns a reference to this point. The division is performed by dividing
     * each coordinate of the point by the scalar value.
     *
     * @param[in] s: The scalar value to divide by.
     * @return A reference to this point after the division operation.
     *
     * @note If the divisor is zero, a runtime error is thrown.
     */
    Point& operator/=(const Scalar& s)
    {
        if (s == 0)
            throw std::runtime_error(
                "Math error: Attempted to divide by Zero\n");
        mP /= s;
        return *this;
    }

    /**
     * @brief Assigns this point to a row vector.
     *
     * This operator assigns this point to a row vector and returns a reference
     * to this point. The assignment is performed by setting this point's
     * coordinates to the values of the row vector.
     *
     * @param[in] v: The row vector to assign from.
     * @return A reference to this point after the assignment operation.
     */
    Point& operator=(const Eigen::Matrix<Scalar, 1, N>& v)
    {
        mP << v;
        return *this;
    }

    /// @private
    template<typename S, uint M>
    friend std::ostream& operator<<(std::ostream& out, const Point<S, M>& p);
};

/**
 * @brief Writes this point to an output stream.
 *
 * This operator writes this point to an output stream and returns the output
 * stream. The point is written as a column vector of its coordinates.
 *
 * @param[in,out] out: The output stream to write to.
 * @param[in] p1 The point to write.
 * @return The output stream after the point is written.
 */
template<typename Scalar, uint N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p1)
{
    out << p1.mP;
    return out;
}

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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
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
 * @ingroup space
 */
using Point4d = Point4<double>;

/* Deduction guides */

template<typename S, typename... Scalars>
Point(S, Scalars... scalars) -> Point<S, sizeof...(Scalars) + 1>;

} // namespace vcl

// inject vcl::Point hash function in std namespace
namespace std {

template<typename Scalar, uint N>
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

#endif // VCL_SPACE_POINT_H
