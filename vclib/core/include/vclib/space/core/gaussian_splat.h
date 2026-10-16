#ifndef GAUSSIAN_SPLAT_H
#define GAUSSIAN_SPLAT_H

#include "point.h"
#include "quaternion.h"
#include "matrix/matrix.h"

#include <array>

namespace vcl {

/**
 * @brief The GaussianSplat class represents a 3D gaussian with:
 * - a scale (3 scalars) component
 * - a rotation (1 quat = 4 scalars) component
 * - 45 spherical harmonics components
 *
 * The Covariance matrix of the 3D gaussian can be calculated from
 * the scale and rotation components.
 * The position of the 3D gaussian must be stored in the position
 * component of the vertex.
 * The base color (spherical harmonics coefficients of order l = 0) of
 * the 3D gaussian must be stored in the color component of the vertex.
 * The opacity component of the 3D gaussian must be stored in the color
 * component of the vector.
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
     * @brief The number of the spherical harmonics coefficients of order
     * l = 1, 2, 3. The coefficients of order l = 0 are stored in the color
     * component of the vertex.
     */
    static constexpr uint SHCoeffCount = 45; // l = 1..3 -> (3+5+7)*3 = 45

    /**
     * @brief The type of spherical harmonics coefficients vector of the
     * 3D gaussian.
     */
    using SHCoeffVecType = std::array<ScalarType, SHCoeffCount>;

private:
    PointType      mScale{1, 1, 1};
    QuatType       mRotation{1, 0, 0, 0}; // Identity rotation
    SHCoeffVecType mSHCoeffs{}; // Initialized to zeros

public:
    GaussianSplat() = default;

    GaussianSplat(const PointType scale, const QuatType& rotation, const SHCoeffVecType& shCoeffs) :
        mScale(scale), mRotation(rotation), mSHCoeffs(shCoeffs)
    {}

    /**
     * @brief Returns a reference to the scale of the GaussianSplate object.
     *
     * @return A reference to the scale of the GaussianSplate object.
     */
    PointType& scale() {
        return mScale;
    }

    /**
     * @brief Returns a const reference to the scale of the GaussianSplate object.
     *
     * @return A const reference to the scale of the GaussianSplate object.
     */
    const PointType& scale() const {
        return mScale;
    }

    /**
     * @brief Returns the logarithmic scale of the GaussianSplate object.
     *
     * @return The logarithmic scale of the GaussianSplate object.
     */
    PointType logScale() const {
        PointType logScale;

        logScale.at(0) = log(mScale.at(0));
        logScale.at(1) = log(mScale.at(1));
        logScale.at(2) = log(mScale.at(2));

        return logScale;
    }

    /**
     * @brief Sets the scale of the GaussianSplate object from a logarithmic scale.
     */
    void setScaleFromLogScale(const PointType& logScale) {
        mScale[0] = exp(logScale.at(0));
        mScale[1] = exp(logScale.at(1));
        mScale[2] = exp(logScale.at(2));
    }

    /**
     * @brief Returns a reference to the rotation of the GaussianSplate object.
     *
     * @return A reference to the rotation of the GaussianSplate object.
     */
    QuatType& rotation() {
        return mRotation;
    }

    /**
     * @brief Returns a const reference to the rotation of the GaussianSplate object.
     *
     * @return A const reference to the rotation of the GaussianSplate object.
     */
    const QuatType& rotation() const {
        return mRotation;
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
    Matrix33<ScalarType> covariance() const {
        Matrix33<ScalarType> rot = mRotation.normalized().toRotationMatrix();
        Matrix33<ScalarType> rs = rot * mScale.asDiagonal();
        return rs * rs.transpose();
    }

    /**
     * @brief Returns a reference to the spherical harmonics coefficients of the GaussianSplate object.
     *
     * @return A reference to the spherical harmonics coefficients of the GaussianSplate object.
     */
    SHCoeffVecType& shCoeffs() {
        return mSHCoeffs;
    }

    /**
     * @brief Returns a const reference to the spherical harmonics coefficients of the GaussianSplate object.
     *
     * @return A const reference to the spherical harmonics coefficients of the GaussianSplate object.
     */
    const SHCoeffVecType& shCoeffs() const {
        return mSHCoeffs;
    }

    ScalarType& shCoeffAt(uint i) {
        assert(i < SHCoeffCount);

        return mSHCoeffs[i];
    }

    const ScalarType& shCoeffAt(uint i) const  {
        assert(i < SHCoeffCount);

        return mSHCoeffs[i];
    }

    /**
     * @brief Sets all the spherical harmonics coefficients of the
     * GaussianSplat object from a set of scalar values.
     *
     * The member function takes a variable number of scalar arguments, which
     * are used to set the spherical harmonics coefficients of the GaussianSplat
     * object. The number of arguments must be equal to the number of spherical
     * harmonics coefficients of the GaussianSplat object, which
     * is determined by the static member SHCoeffCount.
     *
     * @tparam Scalars: The types of the scalar arguments.
     *
     * @param[in] scalars: The scalar arguments used to set the
     * spherical harmonics coefficients of the GaussianSplat object.
     */
    template<typename... Scalars>
    void setSHCoeffs(Scalars... scalars) requires (sizeof...(scalars) == SHCoeffCount)
    {
        ScalarType args[SHCoeffCount] = {static_cast<ScalarType>(scalars)...};
        for (uint i = 0; i < SHCoeffCount; i++)
            mSHCoeffs[i] = args[i];
    }

    /**
     * @brief Sets all the spherical harmonics coefficients of the
     * GaussianSplat object by making a copy of the specified coefficients.
     *
     *
     * @param[in] coeffs: The vector argument used to set the
     * spherical harmonics coefficients of the GaussianSplat object.
     */
    void setSHCoeffs(const SHCoeffVecType& coeffs) { mSHCoeffs = coeffs; }

    /**
     * @brief Casts the GaussianSplat object to a different scalar type.
     *
     * The function returns a new GaussianSplat object with the same attributes
     * as the original object, but with each scalar value casted to a different type.
     *
     * @tparam S: The scalar type to cast to.
     *
     * @return A new GaussianSplat object with the same attributes as the original
     * object, but with each scalar value casted to a different type.
     */
    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<ScalarType, S>) {
            return *this;
        }
        else {
            return GaussianSplat<S>(
                mScale.template cast<S>(),
                mRotation.template cast<S>(),
                mSHCoeffs.template cast<S>()
            );
        }
    }

    bool operator==(const GaussianSplat& gs) const = default;
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
 * @brief A convenience alias for GaussianSplat with double-precision components.
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

#endif // GAUSSIAN_SPLAT_H
