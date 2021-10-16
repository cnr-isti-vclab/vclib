/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT_T_H
#define MGP_POINT_T_H

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace mgp {

/**
 * @brief The Point class represents an N-dimensional point containing
 * N scalar values.
 */
template<class Scalar, int N>
class Point : private Eigen::Matrix<Scalar, 1, N>
{
public:
	Point()               = default; // default empty constructor
	Point(const Point& p) = default; // default copy constructor

	template<class S>
	Point(const Point<S, N>& p);                 // constructor from different scalar Point
	Point(const Eigen::Matrix<Scalar, 1, N>& v); // constructor from 1xN eigen matrix
	Point(const Eigen::Matrix<Scalar, N, 1>& v); // constructor from Nx1 eigen matrix

	Scalar           dot(const Point<Scalar, N>& p1) const;

	// eigen members
	using Eigen::Matrix<Scalar, 1, N>::norm;
	using Eigen::Matrix<Scalar, 1, N>::setConstant;
	using Eigen::Matrix<Scalar, 1, N>::setZero;
	using Eigen::Matrix<Scalar, 1, N>::setOnes;
	using Eigen::Matrix<Scalar, 1, N>::squaredNorm;

	// operators
	using Eigen::Matrix<Scalar, 1, N>::operator();
	using Eigen::Matrix<Scalar, 1, N>::operator<<;

	using Eigen::Matrix<Scalar, 1, N>::operator+;
	using Eigen::Matrix<Scalar, 1, N>::operator-;
	using Eigen::Matrix<Scalar, 1, N>::operator*;
	using Eigen::Matrix<Scalar, 1, N>::operator+=;
	using Eigen::Matrix<Scalar, 1, N>::operator-=;
	using Eigen::Matrix<Scalar, 1, N>::operator*=;

	Scalar  operator[](size_t i) const;
	Scalar& operator[](size_t i);

	template<class S, int M>
	friend std::ostream& operator<<(std::ostream& out, const Point<S, M>& p);

protected:
	// to enable these members just on some derived classes
	using Eigen::Matrix<Scalar, 1, N>::x;
	using Eigen::Matrix<Scalar, 1, N>::y;
	using Eigen::Matrix<Scalar, 1, N>::z;
	using Eigen::Matrix<Scalar, 1, N>::w;
	Point<Scalar, N> cross(const Point<Scalar, N>& p1) const;
};

template<class Scalar, int N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p);

} // namespace mgp

#include "point_t.cpp"

#endif // POINT_T_H
