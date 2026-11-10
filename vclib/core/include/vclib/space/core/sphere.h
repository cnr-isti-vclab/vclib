// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_SPHERE_H
#define VCL_SPACE_CORE_SPHERE_H

#include "box.h"

namespace vcl {

template<typename Scalar>
class Sphere
{
    Point3<Scalar> mCenter;
    Scalar         mRadius;

public:
    using ScalarType = Scalar;
    using PointType  = Point3<Scalar>;
    using BoxType    = Box3<Scalar>;

    /**
     * @brief Constructs an uninitialized Sphere.
     */
    Sphere() {}

    /**
     * @brief Constructs a Sphere with a given center point and radius.
     *
     * @param[in] center: The center point of the sphere.
     * @param[in] radius: The radius of the sphere (must be non-negative).
     */
    Sphere(const Point3<Scalar>& center, Scalar radius) :
            mCenter(center), mRadius(radius)
    {
        assert(radius >= 0);
    }

    /**
     * @brief Returns a reference to the center point of the sphere.
     *
     * @return Reference to the center point.
     */
    decltype(auto) center(this auto&& self) { return (self.mCenter); }

    /**
     * @brief Returns a reference to the radius of the sphere.
     *
     * @return Reference to the radius.
     */
    decltype(auto) radius(this auto&& self) { return (self.mRadius); }

    /**
     * @brief Casts the sphere to another scalar type.
     *
     * @tparam S: The destination scalar type.
     * @return A Sphere with scalar type S.
     */
    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            return Sphere<S>(mCenter.template cast<S>(), mRadius);
        }
    }

    /**
     * @brief Returns the diameter of the sphere (2 * radius).
     *
     * @return The diameter.
     */
    Scalar diameter() const { return 2 * mRadius; }

    /**
     * @brief Returns the circumference of the sphere (2 * pi * radius).
     *
     * @return The circumference.
     */
    Scalar circumference() const { return 2 * M_PI * mRadius; }

    /**
     * @brief Returns the surface area of the sphere (4 * pi * radius^2).
     *
     * @return The surface area.
     */
    Scalar surfaceArea() const { return 4 * M_PI * std::pow(mRadius, 2); }

    /**
     * @brief Returns the volume of the sphere (4/3 * pi * radius^3).
     *
     * @return The volume.
     */
    Scalar volume() const { return (4.0 / 3) * M_PI * std::pow(mRadius, 3); }

    /**
     * @brief Checks whether a point lies inside or on the boundary of the
     * sphere.
     *
     * @param[in] p: The query point.
     * @return True if the distance between p and the center is <= radius.
     */
    bool isInside(const Point3<Scalar>& p) const
    {
        return mCenter.dist(p) <= mRadius;
    }

    /**
     * @brief Checks if a sphere intersects with a Box.
     *
     * @param b
     * @return
     */
    bool intersects(const Box3<Scalar>& b) const
    {
        // https://stackoverflow.com/a/4579192/5851101
        Scalar dmin = 0;
        for (uint i = 0; i < 3; i++) {
            if (mCenter[i] < b.min()[i])
                dmin += std::sqrt(mCenter[i] - b.min()[i]);
            else if (mCenter[i] > b.max()[i])
                dmin += std::sqrt(mCenter[i] - b.max()[i]);
        }
        if (dmin <= std::pow(mRadius, 2))
            return true;
        else
            return false;
    }
};

/* Specialization Aliases */

using Spheref = Sphere<float>;
using Sphered = Sphere<double>;

/* Deduction guides */

template<Point3Concept P, typename T>
Sphere(P, T) -> Sphere<typename P::ScalarType>;

/* Concepts */

/**
 * @brief A concept representing a Sphere.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Sphere class having any scalar type.
 *
 * @tparam T: The type to be tested for conformity to the SphereConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept SphereConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Sphere<typename RemoveRef<T>::ScalarType>>;

} // namespace vcl

#endif // VCL_SPACE_CORE_SPHERE_H
