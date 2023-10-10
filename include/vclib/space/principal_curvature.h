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

#ifndef VCL_SPACE_PRINCIPAL_CURVATURE_H
#define VCL_SPACE_PRINCIPAL_CURVATURE_H

#include "point.h"

namespace vcl {

/**
 * @brief The PrincipalCurvature class stores the principal curvature directions
 * and values at a point on a surface.
 *
 * It stores the two principal curvature directions (`maxDir()` and `minDir()`,
 * also note as k1 and k2 respectively) and the maximum and minimum values of
 * the curvature (`maxValue()` and `minValue()`).
 *
 * @tparam Scalar: The scalar type of the curvature values.
 *
 * @ingroup space
 */
template<typename Scalar>
class PrincipalCurvature
{
	Point3<Scalar> dir1, dir2;
	Scalar         k1 = 0, k2 = 0;

public:
	/**
	 * @brief The scalar type of the curvature values.
	 */
	using ScalarType = Scalar;

	/**
	 * @brief Empty constructor. Directions and values are set to 0.
	 */
	PrincipalCurvature() {}

	/**
	 * @brief Casts the PrincipalCurvature object to a different scalar type.
	 *
	 * The function returns a new PrincipalCurvature object with each scalar
	 * value casted to a different type.
	 *
	 * @tparam S: The scalar type to cast to.
	 *
	 * @return A new PrincipalCurvature object with each scalar value casted to
	 * a different type.
	 */
	template<typename S>
	PrincipalCurvature<S> cast() const
	{
		if constexpr (std::is_same<Scalar, S>::value) {
			return *this;
		}
		else {
			PrincipalCurvature<S> tmp;
			tmp.maxDir() = dir1.template cast<S>();
			tmp.minDir() = dir2.template cast<S>();
			tmp.maxValue() = k1;
			tmp.minValue() = k2;
			return tmp;
		}
	}

	/**
	 * @brief Returns a const reference to the maximum curvature direction.
	 * @return A const reference to the maximum curvature direction.
	 */
	const Point3<Scalar>& maxDir() const { return dir1; }

	/**
	 * @brief Returns a reference to the maximum curvature direction.
	 * @return A reference to the maximum curvature direction.
	 */
	Point3<Scalar>& maxDir() { return dir1; }

	/**
	 * @brief Returns a const reference to the minimum curvature direction.
	 * @return A const reference to the minimum curvature direction.
	 */
	const Point3<Scalar>& minDir() const { return dir2; }

	/**
	 * @brief Returns a reference to the minimum curvature direction.
	 * @return A reference to the minimum curvature direction.
	 */
	Point3<Scalar>& minDir() { return dir2; }

	/**
	 * @brief Returns a const reference to the maximum curvature value.
	 * @return A const reference to the maximum curvature value.
	 */
	const Scalar& maxValue() const { return k1; }

	/**
	 * @brief Returns a reference to the maximum curvature value.
	 * @return A reference to the maximum curvature value.
	 */
	Scalar& maxValue() { return k1; }

	/**
	 * @brief Returns a const reference to the minimum curvature value.
	 * @return A const reference to the minimum curvature value.
	 */
	const Scalar& minValue() const { return k2; }

	/**
	 * @brief Returns a reference to the minimum curvature value.
	 * @return A reference to the minimum curvature value.
	 */
	Scalar& minValue() { return k2; }
};

} // namespace vcl

#endif // VCL_SPACE_PRINCIPAL_CURVATURE_H
