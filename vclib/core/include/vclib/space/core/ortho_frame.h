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

#ifndef VCL_SPACE_CORE_ORTHO_FRAME_H
#define VCL_SPACE_CORE_ORTHO_FRAME_H

#include "ortho_basis.h"

namespace vcl {

/**
 * @brief An N-dimensional affine orthonormal coordinate frame.
 *
 * An `OrthoFrame` combines an `OrthoBasis` (N orthonormal axes) with an
 * origin point, forming a full affine coordinate frame in R^N.  It provides
 * conversions between *local* (frame-relative) and *world* (global)
 * coordinates for points.
 *
 * Given local coordinates @f$ l = (l_0, \ldots, l_{N-1}) @f$, the
 * corresponding world point is:
 * @f[
 *   p = \mathrm{origin} + \sum_{i=0}^{N-1} l_i \,\mathrm{axis}(i)
 * @f]
 * and the inverse (world → local) is:
 * @f[
 *   l_i = (p - \mathrm{origin}) \cdot \mathrm{axis}(i)
 * @f]
 * which is valid because the axes are orthonormal.
 *
 * @note For a pure rotation without a translation (operating on direction
 * vectors rather than points), use `OrthoBasis` directly.
 *
 * @tparam Scalar: Floating-point type used for coordinates.
 * @tparam N: Number of spatial dimensions (must be > 0).
 *
 * @see OrthoBasis, OrthoFrame2, OrthoFrame3
 *
 * @ingroup space_core
 */
template<typename Scalar, uint N>
class OrthoFrame
{
    static_assert(N > 0, "Number of dimensions of OrthoFrame must be > 0.");

    Point<Scalar, N>      mOrigin;
    OrthoBasis<Scalar, N> mBasis;

public:
    using ScalarType = Scalar;
    using PointType  = Point<Scalar, N>;
    using BasisType  = OrthoBasis<Scalar, N>;

    static const uint DIM = N;

    /**
     * @brief Constructs an identity frame centred at the world origin.
     *
     * The origin is set to the zero vector and the basis is set to the
     * identity (i-th axis = @f$ e_i @f$).
     */
    OrthoFrame() = default;

    /**
     * @brief Constructs a frame from an origin point and N individual axis
     * vectors.
     *
     * The axes are forwarded to `OrthoBasis` as-is; the caller is responsible
     * for passing an orthonormal set.
     *
     * @param[in] origin: The origin of the frame in world space.
     * @param[in] axes: The N orthonormal basis vectors.
     */
    template<typename... Axes>
    OrthoFrame(const PointType& origin, Axes&&... axes)
        requires (sizeof...(Axes) == N)
            : mOrigin(origin), mBasis(std::forward<Axes>(axes)...)
    {
    }

    /**
     * @brief Constructs a frame from an origin point and an array of N axis
     * vectors.
     *
     * @param[in] origin: The origin of the frame in world space.
     * @param[in] axes: Array of N orthonormal basis vectors.
     */
    OrthoFrame(const PointType& origin, const std::array<PointType, N>& axes) :
            mOrigin(origin), mBasis(axes)
    {
    }

    /**
     * @brief Constructs a frame from an origin point and an existing
     * `OrthoBasis`.
     *
     * @param[in] origin: The origin of the frame in world space.
     * @param[in] basis: The orthonormal basis.
     */
    OrthoFrame(const PointType& origin, const BasisType& basis) :
            mOrigin(origin), mBasis(basis)
    {
    }

    /**
     * @brief Constructs a right-handed frame from a world-space origin and a
     * normal vector by automatically computing two in-plane axes.
     *
     * Delegates to `OrthoBasis::fromNormal` to build the basis; the normal is
     * stored as `axis(2)`.
     *
     * @note The input @p normal must be non-zero; it is normalised internally.
     *
     * @param[in] origin: The origin of the frame in world space.
     * @param[in] normal: A (not necessarily unit) vector orthogonal to the
     * frame's XY-plane.
     * @return An OrthoFrame whose `axis(2)` is the normalised normal.
     */
    static OrthoFrame fromNormal(
        const PointType& origin,
        const PointType& normal) requires (N == 3)
    {
        return OrthoFrame(origin, BasisType::fromNormal(normal));
    }

    /**
     * @brief Returns the origin of the frame.
     * @return Origin point.
     */
    const PointType& origin() const { return mOrigin; }

    /**
     * @brief Returns a reference to the origin of the frame.
     * @return Origin point.
     */
    PointType& origin() { return mOrigin; }

    /**
     * @brief Returns the orthonormal basis of the frame.
     * @return The `OrthoBasis` component.
     */
    const BasisType& basis() const { return mBasis; }

    /**
     * @brief Returns a reference to the orthonormal basis of the frame.
     * @return The `OrthoBasis` component.
     */
    BasisType& basis() { return mBasis; }

    /**
     * @brief Returns the i-th basis vector of the frame.
     *
     * Convenience forwarder to `basis().axis(i)`.
     *
     * @param[in] i: Axis index (0-based, must be < N).
     * @return i-th orthonormal basis vector.
     */
    const PointType& axis(uint i) const { return mBasis.axis(i); }

    /**
     * @brief Returns a reference to the i-th basis vector of the frame.
     *
     * Convenience forwarder to `basis().axis(i)`.
     *
     * @param[in] i: Axis index (0-based, must be < N).
     * @return i-th orthonormal basis vector.
     */
    PointType& axis(uint i) { return mBasis.axis(i); }

    /**
     * @brief Converts a point expressed in local (frame) coordinates to world
     * coordinates.
     *
     * @f[
     *   \mathrm{world} = \mathrm{origin} +
     * \mathrm{basis.toWorld}(\mathrm{local})
     * @f]
     *
     * @param[in] local: Point in local frame coordinates.
     * @return Corresponding world-space point.
     */
    PointType toWorld(const PointType& local) const
    {
        PointType p = mOrigin;
        p += mBasis.toWorld(local);
        return p;
    }

    /**
     * @brief Converts a world-space point to local (frame) coordinates.
     *
     * @f[
     *   \mathrm{local} = \mathrm{basis.toLocal}(\mathrm{world} -
     * \mathrm{origin})
     * @f]
     *
     * @param[in] world: Point in world space.
     * @return Corresponding point in local frame coordinates.
     */
    PointType toLocal(const PointType& world) const
    {
        return mBasis.toLocal(world - mOrigin);
    }

    /**
     * @brief Returns the affine transform matrix represented by this frame.
     *
     * The returned transform maps local-frame points to world-space points:
     * @f[ T \, \mathbf{l}_{\mathrm{hom}} = \mathbf{w}_{\mathrm{hom}} @f]
     * Its linear part is the rotation matrix of the underlying `OrthoBasis`
     * and its translation part is the origin of the frame.
     *
     * @return An affine transform equivalent to `toWorld`.
     */
    Eigen::Transform<Scalar, N, Eigen::Affine> transformMatrix() const
    {
        Eigen::Transform<Scalar, N, Eigen::Affine> t;
        t.linear()      = mBasis.rotationMatrix();
        t.translation() = mOrigin;
        return t;
    }

    /**
     * @brief Casts the frame to a different scalar type.
     * @tparam S: Target scalar type.
     * @return A new OrthoFrame with all components cast to @p S.
     */
    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            return OrthoFrame<S, N>(
                mOrigin.template cast<S>(), mBasis.template cast<S>());
        }
    }

    /**
     * @brief Serializes the frame to the given output stream.
     * @param[in] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        mOrigin.serialize(os);
        mBasis.serialize(os);
    }

    /**
     * @brief Deserializes the frame from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        mOrigin.deserialize(is);
        mBasis.deserialize(is);
    }

    bool operator==(const OrthoFrame& f) const
    {
        return mOrigin == f.mOrigin && mBasis == f.mBasis;
    }

    bool operator!=(const OrthoFrame& f) const { return !(*this == f); }
};

/* Specialization Aliases */

template<typename Scalar>
using OrthoFrame2 = OrthoFrame<Scalar, 2>;

template<typename Scalar>
using OrthoFrame3 = OrthoFrame<Scalar, 3>;

using OrthoFrame2f = OrthoFrame<float, 2>;
using OrthoFrame2d = OrthoFrame<double, 2>;
using OrthoFrame3f = OrthoFrame<float, 3>;
using OrthoFrame3d = OrthoFrame<double, 3>;

/* Concepts */

/**
 * @brief A concept representing an OrthoFrame.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from an OrthoFrame class having any scalar type and any dimension.
 *
 * @tparam T: The type to be tested for conformity to the OrthoFrameConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept OrthoFrameConcept = std::derived_from<
    std::remove_cvref_t<T>,
    OrthoFrame<typename RemoveRef<T>::ScalarType, RemoveRef<T>::DIM>>;

/**
 * @brief A concept representing a 2D OrthoFrame.
 *
 * @tparam T: The type to be tested for conformity to the OrthoFrame2Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept OrthoFrame2Concept = OrthoFrameConcept<T> && RemoveRef<T>::DIM == 2;

/**
 * @brief A concept representing a 3D OrthoFrame.
 *
 * @tparam T: The type to be tested for conformity to the OrthoFrame3Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept OrthoFrame3Concept = OrthoFrameConcept<T> && RemoveRef<T>::DIM == 3;

} // namespace vcl

#endif // VCL_SPACE_CORE_ORTHO_FRAME_H
