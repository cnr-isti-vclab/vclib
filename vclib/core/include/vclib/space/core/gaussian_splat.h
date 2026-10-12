#ifndef GAUSSIAN_SPLAT_H
#define GAUSSIAN_SPLAT_H

#include "point.h"
#include "quaternion.h"

#include <array>
#include <cmath>

//TODO position (already in vertex)
//TODO base color and opacity (already in vertex)

namespace vcl {

template<typename Scalar>
class GaussianSplat
{
public:
    /**
     * @brief The scalar type of the ray.
     */
    using ScalarType = Scalar;

    /**
     * @brief The type of point used to represent the scale of the
     * covariance matrix.
     */
    using PointType = Point3<Scalar>;

    /**
     * @brief The type of the quaternion used to represent the rotation of
     * the covariance matrix.
     */
    using QuatType = Quaternion<Scalar>;

    static constexpr uint SHCoeffCount = 45; // l = 1..3 -> (3+5+7)*3 = 45

private:
    // Stored as log-scale; use scale() to retrieve exp(logScale_)
    PointType mLogScale{0, 0, 0};
    QuatType  mRotation{1, 0, 0, 0}; // Identity rotation
    Scalar    mOpacity = Scalar(1.0);
    std::array<Scalar, SHCoeffCount> mSHCoeffs{}; // Initialized to zeros

public:
    GaussianSplat() = default;

    /**
     * @brief Returns the actual scale (exp of stored log-scale), component
     * wise. This is the value that should be used in rendering / evaluation.
     */
    PointType scale() const
    {
        PointType s;

        for (uint i = 0; i < 3; ++i) {
            s[i] = std::exp(mLogScale[i]);
        }

        return s;
    }
};

} // namespace vcl

#endif // GAUSSIAN_SPLAT_H
