/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_SPACE_CORE_ORTHO_BASIS_H
#define VCL_SPACE_CORE_ORTHO_BASIS_H

#include "matrix.h"
#include "point.h"

#include <array>

namespace vcl {

/**
 * @brief An N-dimensional orthonormal basis.
 *
 * An `OrthoBasis` is a set of N mutually orthonormal vectors that define a
 * coordinate frame orientation in R^N (without any origin point).  It provides
 * linear conversions between *local* (frame-relative) and *world* (global)
 * direction vectors.
 *
 * Given a direction vector @f$ \mathbf{l} @f$ expressed in local coordinates,
 * the corresponding world-space direction is:
 * @f[
 *   \mathbf{w} = \sum_{i=0}^{N-1} l_i \,\mathrm{axis}(i)
 * @f]
 * and the inverse (world → local) is:
 * @f[
 *   l_i = \mathbf{w} \cdot \mathrm{axis}(i)
 * @f]
 * which is valid because the axes are orthonormal.
 *
 * @note Both `toWorld` and `toLocal` are pure linear transforms (no
 * translation). To include an origin offset, use `OrthoFrame`, which combines
 * an `OrthoBasis` with an origin point.
 *
 * @tparam Scalar: Floating-point type used for coordinates.
 * @tparam N: Number of spatial dimensions (must be > 0).
 *
 * @see OrthoFrame
 *
 * @ingroup space_core
 */
template<typename Scalar, uint N>
class OrthoBasis
{
    static_assert(N > 0, "Number of dimensions of OrthoBasis must be > 0.");

    std::array<Point<Scalar, N>, N> mAxes;

public:
    using ScalarType = Scalar;
    using PointType  = Point<Scalar, N>;

    static const uint DIM = N;

    /**
     * @brief Constructs an identity basis.
     *
     * The i-th axis is set to the i-th standard basis vector @f$ e_i @f$.
     */
    OrthoBasis()
    {
        for (uint i = 0; i < N; ++i) {
            mAxes[i]    = PointType();
            mAxes[i](i) = Scalar(1);
        }
    }

    /**
     * @brief Constructs a basis from N individual axis vectors.
     *
     * The axes are taken as-is and are not normalised or orthogonalised by the
     * constructor; the caller is responsible for passing an orthonormal set.
     *
     * @param[in] axes: The N orthonormal basis vectors.
     */
    template<typename... Axes>
    OrthoBasis(Axes&&... axes) requires (sizeof...(Axes) == N)
            : mAxes {std::forward<Axes>(axes)...}
    {
    }

    /**
     * @brief Constructs a basis from an array of N axis vectors.
     *
     * @param[in] axes: Array of N orthonormal basis vectors.
     */
    OrthoBasis(const std::array<PointType, N>& axes) : mAxes(axes) {}

    /**
     * @brief Constructs a right-handed basis from a normal vector.
     *
     * Two orthonormal in-plane vectors are computed automatically using
     * `Point::orthoBase` and stored as `axis(0)` and `axis(1)`.  The
     * normalised normal is stored as `axis(2)`.
     *
     * @note The input @p normal must be non-zero; it is normalised internally.
     *
     * @param[in] normal: A (not necessarily unit) vector orthogonal to the
     * frame's XY-plane.
     * @return An OrthoBasis whose `axis(2)` is the normalised normal.
     */
    static OrthoBasis fromNormal(const PointType& normal) requires (N == 3)
    {
        PointType n = normal.normalized();
        PointType u, v;
        n.orthoBase(u, v);
        u.normalize();
        v.normalize();
        return OrthoBasis(u, v, n);
    }

    /**
     * @brief Returns the i-th basis vector.
     * @param[in] i: Axis index (0-based, must be < N).
     * @return i-th orthonormal basis vector.
     */
    const PointType& axis(uint i) const { return mAxes[i]; }

    /**
     * @brief Returns a reference to the i-th basis vector.
     * @param[in] i: Axis index (0-based, must be < N).
     * @return i-th orthonormal basis vector.
     */
    PointType& axis(uint i) { return mAxes[i]; }

    /**
     * @brief Transforms a direction vector from local to world coordinates.
     *
     * This is a pure linear transform (rotation only, no translation):
     * @f[
     *   \mathbf{w} = \sum_{i=0}^{N-1} \mathrm{local}(i) \cdot \mathrm{axis}(i)
     * @f]
     *
     * @param[in] local: Direction vector in local frame coordinates.
     * @return Corresponding direction vector in world space.
     */
    PointType toWorld(const PointType& local) const
    {
        PointType p;
        p.setZero();
        for (uint i = 0; i < N; ++i)
            p += mAxes[i] * local(i);
        return p;
    }

    /**
     * @brief Transforms a direction vector from world to local coordinates.
     *
     * Because the axes are orthonormal, the inverse is simply:
     * @f[
     *   \mathrm{local}(i) = \mathbf{world} \cdot \mathrm{axis}(i)
     * @f]
     *
     * @param[in] world: Direction vector in world space.
     * @return Corresponding direction vector in local frame coordinates.
     */
    PointType toLocal(const PointType& world) const
    {
        PointType local;
        for (uint i = 0; i < N; ++i)
            local(i) = world.dot(mAxes[i]);
        return local;
    }

    /**
     * @brief Returns the rotation matrix represented by this basis.
     *
     * The i-th column of the returned matrix is `axis(i)`, so that
     * @f[ R \, \mathbf{l} = \mathrm{toWorld}(\mathbf{l}) @f]
     * and @f[ R^\top \, \mathbf{w} = \mathrm{toLocal}(\mathbf{w}) @f]
     * (valid because the axes are orthonormal, so @f$ R^{-1} = R^\top @f$).
     *
     * @return An NxN rotation matrix whose columns are the basis vectors.
     */
    Matrix<Scalar, N, N> rotationMatrix() const
    {
        Matrix<Scalar, N, N> r;
        for (uint i = 0; i < N; ++i)
            r.col(i) = mAxes[i];
        return r;
    }

    /**
     * @brief Casts the basis to a different scalar type.
     * @tparam S: Target scalar type.
     * @return A new OrthoBasis with all axes cast to @p S.
     */
    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            std::array<Point<S, N>, N> newAxes;
            for (uint i = 0; i < N; ++i)
                newAxes[i] = mAxes[i].template cast<S>();
            return OrthoBasis<S, N>(newAxes);
        }
    }

    /**
     * @brief Serializes the basis to the given output stream.
     * @param[in] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        for (uint i = 0; i < N; ++i)
            mAxes[i].serialize(os);
    }

    /**
     * @brief Deserializes the basis from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        for (uint i = 0; i < N; ++i)
            mAxes[i].deserialize(is);
    }

    bool operator==(const OrthoBasis& b) const { return mAxes == b.mAxes; }

    bool operator!=(const OrthoBasis& b) const { return !(*this == b); }
};

/* Specialization Aliases */

template<typename Scalar>
using OrthoBasis2 = OrthoBasis<Scalar, 2>;

template<typename Scalar>
using OrthoBasis3 = OrthoBasis<Scalar, 3>;

using OrthoBasis2f = OrthoBasis<float, 2>;
using OrthoBasis2d = OrthoBasis<double, 2>;
using OrthoBasis3f = OrthoBasis<float, 3>;
using OrthoBasis3d = OrthoBasis<double, 3>;

/* Concepts */

/**
 * @brief A concept representing an OrthoBasis.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from an OrthoBasis class having any scalar type and any dimension.
 *
 * @tparam T: The type to be tested for conformity to the OrthoBasisConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept OrthoBasisConcept = std::derived_from<
    std::remove_cvref_t<T>,
    OrthoBasis<typename RemoveRef<T>::ScalarType, RemoveRef<T>::DIM>>;

/**
 * @brief A concept representing a 2D OrthoBasis.
 *
 * @tparam T: The type to be tested for conformity to the OrthoBasis2Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept OrthoBasis2Concept = OrthoBasisConcept<T> && RemoveRef<T>::DIM == 2;

/**
 * @brief A concept representing a 3D OrthoBasis.
 *
 * @tparam T: The type to be tested for conformity to the OrthoBasis3Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept OrthoBasis3Concept = OrthoBasisConcept<T> && RemoveRef<T>::DIM == 3;

} // namespace vcl

#endif // VCL_SPACE_CORE_ORTHO_BASIS_H
