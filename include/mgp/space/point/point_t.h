/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT_T_H
#define MGP_POINT_T_H

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <mgp/math/base.h>

namespace mgp {

/**
 * @brief The Point class represents an N-dimensional point containing
 * N scalar values.
 */
template<class Scalar, int N>
class Point
{
public:
	using ScalarType      = Scalar;
	static const int DIM  = N;
	Point()               = default; // default empty constructor
	Point(const Point& p) = default; // default copy constructor
	Point(Point&& p)      = default; // default move constructor

	Point& operator=(const Point& p) = default; // default copy assignment operator
	Point& operator=(Point&& p) = default;      // default move assignment operator

	template<class S>
	Point(const Point<S, N>& p);                 // constructor from different scalar Point
	Point(const Eigen::Matrix<Scalar, 1, N>& v); // constructor from 1xN eigen matrix

	bool isDegenerate() const;

	template<class S>
	Scalar dot(const Point<S, N>& p1) const;

	template<class S>
	Scalar angle(const Point<S, N>& p1) const;

	Scalar       norm() const;
	Scalar       squaredNorm() const;
	unsigned int size() const;

	void setConstant(Scalar s);
	void setZero();
	void setOnes();

	void normalize();

	// operators
	Scalar&       operator()(unsigned int i);
	const Scalar& operator()(unsigned int i) const;

	bool operator==(const Point& p1) const;
	bool operator!=(const Point& p1) const;
	bool operator<(const Point& p1) const;

	Point  operator+(const Scalar& s) const;
	Point  operator+(const Point& p1) const;
	Point  operator-(const Scalar& s) const;
	Point  operator-(const Point& p1) const;
	Point  operator*(const Scalar& s) const;
	Scalar operator*(const Point& p1) const;
	Point  operator/(const Scalar& s) const;
	Point& operator+=(const Point& p1);
	Point& operator-=(const Point& p1);
	Point& operator*=(const Scalar& s);
	Point& operator/=(const Scalar& s);

	Scalar  operator[](size_t i) const;
	Scalar& operator[](size_t i);

	template<class S, int M>
	friend std::ostream& operator<<(std::ostream& out, const Point<S, M>& p);

protected:
	Eigen::Matrix<Scalar, 1, N> p;
};

template<class Scalar, int N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p);

} // namespace mgp

#include "point_t.cpp"

#endif // POINT_T_H
