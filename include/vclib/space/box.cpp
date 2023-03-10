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

#include "box.h"

#include <algorithm>

#include <vclib/math/min_max.h>

namespace vcl {

/**
 * @brief The Empty constructor of a box, initializes a null box.
 * @see setNull
 */
template<PointConcept PointT>
Box<PointT>::Box()
{
	setNull();
}

/**
 * @brief Initializes the box with the given point. The box is a valid empty box (meaning that the
 * minimum and the maximum points are equal).
 *
 * @param p The point to initialize the box with.
 */
template<PointConcept PointT>
Box<PointT>::Box(const PointT& p) : minP(p), maxP(p)
{
}

/**
 * @brief Initializes the box with the given minimum and maximum points.
 *
 * If min > max for any dimension of the points, the box will be invalid.
 *
 * @param[in] min: The minimum point of the box.
 * @param[in] max: The maximum point of the box.
 */
template<PointConcept PointT>
Box<PointT>::Box(const PointT& min, const PointT& max) : minP(min), maxP(max)
{
}

/**
 * @brief Returns a reference to the minimum point of the box.
 *
 * @return A reference to the minimum point of the box.
 */
template<PointConcept PointT>
PointT& Box<PointT>::min()
{
	return minP;
}

/**
 * @brief Returns a const reference to the minimum point of the box.
 *
 * @return A const reference to the minimum point of the box.
 */
template<PointConcept PointT>
const PointT& Box<PointT>::min() const
{
	return minP;
}

/**
 * @brief Returns a reference to the maximum point of the box.
 *
 * @return A reference to the maximum point of the box.
 */
template<PointConcept PointT>
PointT& Box<PointT>::max()
{
	return maxP;
}

/**
 * @brief Returns a const reference to the maximum point of the box.
 *
 * @return A const reference to the maximum point of the box.
 */
template<PointConcept PointT>
const PointT& Box<PointT>::max() const
{
	return maxP;
}

/**
 * @brief Constructor that allows initializing a box from another box of different scalar type
 * (but same size).
 *
 * @tparam P: The scalar type of the other box.
 * @param[in] ob: The other box to copy.
 */
//template<PointConcept PointT>
//template<typename P>
//Box<PointT>::Box(const Box<P>& ob) requires (DIM == P::DIM) :
//		minP(ob.minP), maxP(ob.maxP)
//{
//}

/**
 * @brief Checks whether the box is null or not.
 *
 * A box is considered null if at least one minimum component is greater than the corresponding
 * maximum component.
 *
 * @return True if the box is null, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::isNull() const
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (minP[i] > maxP[i])
			return true;
	}
	return false;
}

/**
 * @brief Checks whether the box is empty or not.
 *
 * A box is considered empty if the minimum point and the maximum point are equal.
 *
 * @return True if the box is empty, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::isEmpty() const
{
	return minP == maxP;
}

/**
 * @brief Checks whether a given point is inside the box or not, bounds included.
 *
 * A point is considered inside the box if its coordinates are greater or equal to the corresponding
 * minimum point and less or equal to the corresponding maximum point for each dimension.
 *
 * @param[in] p: The point to be checked.
 * @return True if the point is inside the box, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::isInside(const PointT& p) const
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (p[i] < minP[i] || p[i] > maxP[i])
			return false;
	}
	return true;
}

/**
 * @brief Checks if a point is inside the open box (max bound excluded); e.g. p in [min, max).
 *
 * @param[in] p: the point to check if is inside the current box.
 * @return true if p is inside the open box, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::isInsideOpenBox(const PointT& p) const
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (p[i] < minP[i] || p[i] >= maxP[i])
			return false;
	}
	return true;
}

/**
 * @brief Check if two boxes overlap.
 *
 * @param[in] b: the box to check the collision with.
 * @return true if the two boxes overlap, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::overlap(const Box<PointT>& b) const
{
	// Check if any dimension of the boxes does not overlap.
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (b.minP[i] >= maxP[i] || b.maxP[i] <= minP[i])
			return false;
	}
	// All dimensions overlap, the boxes collide.
	return true;
}

/**
 * @brief Same as Box::overlap.
 *
 * @param[in] b: the box to check the collision with.
 * @return true if the two boxes overlap, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::collide(const Box<PointT>& b) const
{
	return overlap(b);
}

/**
 * @brief Same as Box::overlap.
 *
 * @param[in] b: the box to check the collision with.
 * @return true if the two boxes overlap, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::intersects(const Box<PointT>& b) const
{
	return overlap(b);
}

/**
 * @brief Calculates the diagonal length of the box.
 *
 * @return The diagonal length of the box.
 */
template<PointConcept PointT>
auto Box<PointT>::diagonal() const
{
	return minP.dist(maxP);
}

/**
 * @brief Calculates the squared length of the diagonal of the box.
 * @return The squared length of the diagonal of the box.
 */
template<PointConcept PointT>
auto Box<PointT>::squaredDiagonal() const
{
	return minP.squaredDist(maxP);
}

/**
 * @brief Calculates the center point of the box.
 *
 * @return The center point of the box.
 */
template<PointConcept PointT>
PointT Box<PointT>::center() const
{
	return (maxP + minP) / 2;
}

/**
 * @brief Computes the size of the box.
 *
 * @return The size of the box as a point, i.e., the difference between maxP and minP.
 */
template<PointConcept PointT>
PointT Box<PointT>::size() const
{
	return maxP - minP;
}

/**
 * @brief Computes the volume of the box.
 *
 * @return The volume of the box.
 */
template<PointConcept PointT>
auto Box<PointT>::volume() const
{
	// Initialize volume as the width of the first dimension.
	auto vol = maxP[0] - minP[0];
	// Multiply by the width of each dimension.
	for (uint i = 1; i < PointT::DIM; ++i) {
		vol *= maxP[i] - minP[i];
	}
	return vol;
}

/**
 * @brief Get the length of the box along a given dimension.
 *
 * @param[in] i: the index of the dimension to get the length of.
 * @return The length of the box along the given dimension.
 * @pre i < PointT::DIM.
 */
template<PointConcept PointT>
auto Box<PointT>::dim(uint i) const
{
	assert(i < PointT::DIM);
	return maxP[i]-minP[i];
}

/**
 * @brief Calculates the minimum dimension of the box, which is defined as the smallest difference
 * between the maximum and minimum coordinates along any dimension.
 *
 * @return The minimum dimension of the box.
 */
template<PointConcept PointT>
auto Box<PointT>::minDim() const
{
	auto m = maxP[0] - minP[0];
	for (uint i = 1; i < PointT::DIM; ++i) {
		if (maxP[i] - minP[i] < m)
			m = maxP[i] - minP[i];
	}
	return m;
}

/**
 * @brief Computes the maximum dimension of the box, which is defined as the greater difference
 * between the maximum and minimum coordinates along any dimension.
 *
 * @return The maximum dimension of the box.
 */
template<PointConcept PointT>
auto Box<PointT>::maxDim() const
{
	auto m = maxP[0] - minP[0];
	for (uint i = 1; i < PointT::DIM; ++i) {
		if (maxP[i] - minP[i] > m)
			m = maxP[i] - minP[i];
	}
	return m;
}

/**
 * @brief Computes and returns the intersection between the current box and the given box b
 *
 * @param[in] b: The box to intersect with the current box
 * @return The intersection between the current box and b
 */
template<PointConcept PointT>
Box<PointT> Box<PointT>::intersection(const Box<PointT>& b) const
{
	Box<PointT> res = *this;
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (minP[i] < b.minP[i])
			res.minP[i] = b.minP[i]; // set the minimum point to the larger value
		if (maxP[i] > b.maxP[i])
			res.maxP[i] = b.maxP[i]; // set the maximum point to the smaller value
		if (res.minP[i] > res.maxP[i]){
			res.setNull(); // if the minimum point is larger than the maximum point, the box is null
			return res;
		}
	}
	return res;
}

/**
 * @brief Sets the Box to null. A box is considered null if at least one min component
 * is greater than the corresponding max component.
 *
 * This member function sets all the min values to the max representable Scalar and the max
 * values to lowest representable Scalar.
 */
template<PointConcept PointT>
void Box<PointT>::setNull()
{
	minP.setConstant(std::numeric_limits<typename PointT::ScalarType>::max());
	maxP.setConstant(std::numeric_limits<typename PointT::ScalarType>::lowest());
}

/**
 * @brief Adds the given point to the current box, expanding this box in order to
 * contain also the values of the added point.
 *
 * If this box was null, both the minimum and maximum points of the box will be set to the given point.
 *
 * @param[in] p The point to add to the current box.
 */
template<PointConcept PointT>
void Box<PointT>::add(const PointT& p)
{
	if (isNull()) {
		// If the box was null, set both the minimum and maximum points to the given point
		*this = Box(p);
	}
	else {
		// Expand the current box to include the added point
		minP = vcl::min(minP, p);
		maxP = vcl::max(maxP, p);
	}
}

/**
 * @brief Adds the given point to the current box, expanding this box by a radius
 * around the point, so that it will contain all points within the radius.
 *
 * If this box was null, it will be initialized with the minimum and maximum points
 * obtained by subtracting and adding the radius to the given point.
 *
 * @tparam Scalar: The data type of the radius.
 * @param[in] p: The point to add to the current box.
 * @param[in] radius: The radius around the point.
 */
template<PointConcept PointT>
template<typename Scalar>
void Box<PointT>::add(const PointT& p, Scalar radius)
{
	if (isNull()) {
		// If the box was null, initialize it with the minimum and maximum points obtained by
		// subtracting and adding the radius to the given point
		*this = Box(p - radius, p + radius);
	}
	else {
		// Expand the current box to include the added point and all points within the radius around
		// it
		minP = vcl::min(minP, p - radius);
		maxP = vcl::max(maxP, p + radius);
	}
}

/**
 * @brief Adds the given box to the current box, expanding this box in order that will
 * contain also the b values.
 *
 * If this box was null, the current box will have the same values of b.
 *
 * @param[in] b: The box to add to the current box.
 */
template<PointConcept PointT>
void Box<PointT>::add(const Box<PointT>& b)
{
	// Adding a null box should do nothing
	if (!b.isNull()) {
		if (isNull()) {
			*this = b; // if the current box is null, set it equal to b
		}
		else {
			add(b.minP); // expand this box to contain the minimum point of b
			add(b.maxP); // expand this box to contain the maximum point of b
		}
	}
}

/**
 * @brief Translates the box by summing the values of p
 *
 * @param[in] p: The point to translate the box by
 */
template<PointConcept PointT>
void Box<PointT>::translate(const PointT& p)
{
	minP += p; // translate the minimum point by p
	maxP += p; // translate the maximum point by p
}

/**
 * @brief Compares the current box to another box for equality.
 *
 * @param[in] b: The box to compare to.
 * @return True if the boxes are equal, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::operator==(const Box<PointT>& b) const
{
	return minP == b.minP && maxP == b.maxP;
}

/**
 * @brief Compares the current box to another box for inequality.
 *
 * @param[in] b: The box to compare to.
 * @return True if the boxes are not equal, false otherwise.
 */
template<PointConcept PointT>
bool Box<PointT>::operator!=(const Box<PointT>& b) const
{
	return !(*this == b);
}

static_assert(Box2Concept<Box2i>, "Box2i does not satisfy the Box2Concept");
static_assert(Box2Concept<Box2f>, "Box2f does not satisfy the Box2Concept");
static_assert(Box2Concept<Box2d>, "Box2d does not satisfy the Box2Concept");

static_assert(Box3Concept<Box3i>, "Box3i does not satisfy the Box3Concept");
static_assert(Box3Concept<Box3f>, "Box3f does not satisfy the Box3Concept");
static_assert(Box3Concept<Box3d>, "Box3d does not satisfy the Box3Concept");

} // namespace vcl
