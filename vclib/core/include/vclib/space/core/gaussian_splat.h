// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_GAUSSIAN_SPLAT_H
#define VCL_SPACE_CORE_GAUSSIAN_SPLAT_H

#include "matrix.h"
#include "point.h"
#include "quaternion.h"

#include <algorithm>
#include <array>

namespace vcl {

/**
 * @brief The GaussianSplat class represents a 3D gaussian with:
 * - an opacity (1 scalar) component
 * - a scale (3 scalars) component
 * - a rotation (1 quat = 4 scalars) component
 * - 48 spherical harmonics components
 *
 * The Covariance matrix of the 3D gaussian can be calculated from
 * the scale and rotation components.
 * The position of the 3D gaussian must be stored in the position
 * component of the vertex.
 *
 * @tparam Scalar: The scalar type of the point components.
 *
 * @ingroup space_core
 */

template<typename Scalar>
class GaussianSplat
{
public:
    /**
     * @brief The scalar type.
     */
    using ScalarType = Scalar;

    /**
     * @brief The type of point used to represent the scale of the
     * covariance matrix.
     */
    using PointType = Point3<ScalarType>;

    /**
     * @brief The type of the quaternion used to represent the rotation of
     * the covariance matrix.
     */
    using QuatType = Quaternion<ScalarType>;

    /**
     * @brief The number of the spherical harmonics coefficients
     */
    static constexpr uint SH_DEGREE = 3;
    static constexpr uint SH_COLOR = 3; // RGB color channels
    static constexpr uint SH_COEFF_COUNT =
        SH_COLOR * (SH_DEGREE+1)*(SH_DEGREE+1);
        // e.g. l = 0..3 -> (1+3+5+7)*3 = 48

    /**
     * @brief The type of spherical harmonics coefficients array of the
     * 3D gaussian.
     */
    using SHCoeffArrayType = std::array<ScalarType, SH_COEFF_COUNT>;

private:
    ScalarType       mOpacity{1};
    PointType        mScale {1, 1, 1};
    QuatType         mRotation {1, 0, 0, 0}; // Identity rotation
    SHCoeffArrayType mSHCoeffs {};           // Initialized to zeros

public:
    GaussianSplat() = default;

    GaussianSplat(
        const PointType&        scale,
        const QuatType&         rotation,
        const SHCoeffArrayType& shCoeffs) :
            mScale(scale), mRotation(rotation), mSHCoeffs(shCoeffs)
    {
    }

    /**
     * @brief Returns a reference to the opacity of the GaussianSplat object.
     *
     * @return A reference to the opacity of the GaussianSplat object.
     */
    decltype(auto) opacity(this auto&& self) { return (self.mOpacity); }

    /**
     * @brief Returns a reference to the scale of the GaussianSplat object.
     *
     * @return A reference to the scale of the GaussianSplat object.
     */
    decltype(auto) scale(this auto&& self) { return (self.mScale); }

    /**
     * @brief Returns a reference to the rotation of the GaussianSplat object.
     *
     * @return A reference to the rotation of the GaussianSplat object.
     */
    decltype(auto) rotation(this auto&& self) { return (self.mRotation); }

    /**
     * @brief Returns a reference to the spherical harmonics coefficients of the
     * GaussianSplat object.
     *
     * @return A reference to the spherical harmonics coefficients of the
     * GaussianSplat object.
     */
    decltype(auto) shCoeffs(this auto&& self) { return (self.mSHCoeffs); }

    /**
     * @brief Returns a reference to the spherical harmonics coefficient at the
     * specified index of the GaussianSplat object.
     *
     * @param[in] i: The index of the spherical harmonics coefficient to
     * retrieve. Must be less than SH_COEFF_COUNT.
     *
     * @return A reference to the spherical harmonics coefficient at the
     * specified index of the GaussianSplat object.
     */
    decltype(auto) shCoeffAt(this auto&& self, uint i)
    {
        assert(i < self.SH_COEFF_COUNT);

        return self.mSHCoeffs[i];
    }

    /**
     * @brief Computes the covariance matrix of the 3D gaussian.
     *
     * This function computes the covariance matrix of the 3D gaussian,
     * which must be used during rendering.
     * The covariance matrix is calculated as such:
     *     cov = R * S * St * Rt
     * where R is the rotation matrix, S is the scale matrix,
     * and "t" indicates the matrix is transposed.
     *
     * @return The covariance matrix of the 3D gaussian.
     */
    Matrix33<ScalarType> covariance() const
    {
        Matrix33<ScalarType> rot = mRotation.normalized().toRotationMatrix();
        Matrix33<ScalarType> rs  = rot * mScale.asDiagonal();
        return rs * rs.transpose();
    }

    /**
     * @brief Returns the logarithmic scale of the GaussianSplat object.
     *
     * @return The logarithmic scale of the GaussianSplat object.
     */
    PointType logScale() const
    {
        return PointType(
            std::log(mScale[0]), std::log(mScale[1]), std::log(mScale[2]));
    }

    /**
     * @brief Sets the scale of the GaussianSplat object from a logarithmic
     * scale.
     */
    void setScaleFromLogScale(const PointType& logScale)
    {
        mScale[0] = std::exp(logScale[0]);
        mScale[1] = std::exp(logScale[1]);
        mScale[2] = std::exp(logScale[2]);
    }

    /**
     * @brief Sets all the spherical harmonics coefficients of the
     * GaussianSplat object from a set of scalar values.
     *
     * The member function takes a variable number of scalar arguments, which
     * are used to set the spherical harmonics coefficients of the GaussianSplat
     * object. The number of arguments must be equal to the number of spherical
     * harmonics coefficients of the GaussianSplat object, which
     * is determined by the static member SH_COEFF_COUNT.
     *
     * @tparam Scalars: The types of the scalar arguments.
     *
     * @param[in] scalars: The scalar arguments used to set the
     * spherical harmonics coefficients of the GaussianSplat object.
     */
    template<typename... Scalars>
    void setSHCoeffs(Scalars... scalars)
        requires (sizeof...(scalars) == SH_COEFF_COUNT)
    {
        mSHCoeffs = {static_cast<ScalarType>(scalars)...};
    }

    /**
     * @brief Sets all the spherical harmonics coefficients of the
     * GaussianSplat object by making a copy of the specified coefficients.
     *
     * @param[in] coeffs: The array argument used to set the
     * spherical harmonics coefficients of the GaussianSplat object.
     */
    void setSHCoeffs(const SHCoeffArrayType& coeffs) { mSHCoeffs = coeffs; }

    /**
     * @brief Casts the GaussianSplat object to a different scalar type.
     *
     * The function returns a new GaussianSplat object with the same attributes
     * as the original object, but with each scalar value casted to a different
     * type.
     *
     * @tparam S: The scalar type to cast to.
     *
     * @return A new GaussianSplat object with the same attributes as the
     * original object, but with each scalar value casted to a different type.
     */
    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<ScalarType, S>) {
            return *this;
        }
        else {
            std::array<S, SH_COEFF_COUNT> coeffs;

            std::transform(
                mSHCoeffs.begin(),
                mSHCoeffs.end(),
                coeffs.begin(),
                [](ScalarType value) { return static_cast<S>(value); });

            return GaussianSplat<S>(
                mScale.template cast<S>(),
                mRotation.template cast<S>(),
                coeffs);
        }
    }

    bool operator==(const GaussianSplat& gs) const = default;

    /**
     * @brief Serializes the 3d gaussian to the given output stream.
     * @param[in] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mOpacity);
        mScale.serialize(os);
        vcl::serializeN(os, mRotation.coeffs().data(), 4);
        vcl::serializeN(os, mSHCoeffs.data(), SH_COEFF_COUNT);
    }

    /**
     * @brief Deserializes the 3d gaussian from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mOpacity);
        mScale.deserialize(is);
        vcl::deserializeN(is, mRotation.coeffs().data(), 4);
        vcl::deserializeN(is, mSHCoeffs.begin(), SH_COEFF_COUNT);
    }
};

/* Specialization Aliases */

/**
 * @brief A convenience alias for GaussianSplat with floating-point components.
 *
 * The GaussianSplatf alias is a shorthand for a GaussianSplat class template
 * specialization with floating-point components.
 *
 * @ingroup space_core
 */
using GaussianSplatf = GaussianSplat<float>;

/**
 * @brief A convenience alias for GaussianSplat with double-precision
 * components.
 *
 * The GaussianSplatd alias is a shorthand for a GaussianSplat class template
 * specialization with double-precision components.
 *
 * @ingroup space_core
 */
using GaussianSplatd = GaussianSplat<double>;

/* Concepts */

/**
 * @brief A concept representing a GaussianSplat.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a GaussianSplat class having any Scalar type.
 *
 * @tparam T: The type to be tested for conformity to the GaussianSplatConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept GaussianSplatConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    GaussianSplat<typename RemoveRef<T>::ScalarType>>;

} // namespace vcl

#endif // VCL_SPACE_CORE_GAUSSIAN_SPLAT_H
