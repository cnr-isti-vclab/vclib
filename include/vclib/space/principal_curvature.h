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
    Point3<Scalar> mDir1, mDir2;
    Scalar         mK1 = 0, mK2 = 0;

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
    auto cast() const
    {
        if constexpr (std::is_same<Scalar, S>::value) {
            return *this;
        }
        else {
            PrincipalCurvature<S> tmp;
            tmp.maxDir()   = mDir1.template cast<S>();
            tmp.minDir()   = mDir2.template cast<S>();
            tmp.maxValue() = mK1;
            tmp.minValue() = mK2;
            return tmp;
        }
    }

    /**
     * @brief Returns a const reference to the maximum curvature direction.
     * @return A const reference to the maximum curvature direction.
     */
    const Point3<Scalar>& maxDir() const { return mDir1; }

    /**
     * @brief Returns a reference to the maximum curvature direction.
     * @return A reference to the maximum curvature direction.
     */
    Point3<Scalar>& maxDir() { return mDir1; }

    /**
     * @brief Returns a const reference to the minimum curvature direction.
     * @return A const reference to the minimum curvature direction.
     */
    const Point3<Scalar>& minDir() const { return mDir2; }

    /**
     * @brief Returns a reference to the minimum curvature direction.
     * @return A reference to the minimum curvature direction.
     */
    Point3<Scalar>& minDir() { return mDir2; }

    /**
     * @brief Returns a const reference to the maximum curvature value.
     * @return A const reference to the maximum curvature value.
     */
    const Scalar& maxValue() const { return mK1; }

    /**
     * @brief Returns a reference to the maximum curvature value.
     * @return A reference to the maximum curvature value.
     */
    Scalar& maxValue() { return mK1; }

    /**
     * @brief Returns a const reference to the minimum curvature value.
     * @return A const reference to the minimum curvature value.
     */
    const Scalar& minValue() const { return mK2; }

    /**
     * @brief Returns a reference to the minimum curvature value.
     * @return A reference to the minimum curvature value.
     */
    Scalar& minValue() { return mK2; }
};

} // namespace vcl

#endif // VCL_SPACE_PRINCIPAL_CURVATURE_H
