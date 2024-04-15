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

#ifndef VCL_ALGORITHMS_CORE_FITTING_H
#define VCL_ALGORITHMS_CORE_FITTING_H

#include <Eigen/Eigenvalues>

#include <vclib/space/plane.h>

#include "stat.h"

namespace vcl {

/**
 * @brief Compute the plane best fitting a set of points
 * The algorithm used is the classical Covariance matrix eigenvector approach.
 */
template<typename Scalar>
Plane<Scalar> fitPlaneToPointCloud(const std::vector<Point3<Scalar>>& pointVec)
{
    Matrix33<Scalar> covMat = covarianceMatrixOfPointCloud(pointVec);
    Point3<Scalar>   b = polygonBarycenter(pointVec.begin(), pointVec.end());

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<Scalar, 3, 3>> eig(covMat);

    Eigen::Matrix<Scalar, 3, 1> eval = eig.eigenvalues();
    Eigen::Matrix<Scalar, 3, 3> evec = eig.eigenvectors();

    eval = eval.cwiseAbs();
    int minInd;
    eval.minCoeff(&minInd);
    Point3<Scalar> d;
    d[0] = evec(0, minInd);
    d[1] = evec(1, minInd);
    d[2] = evec(2, minInd);

    return Plane<Scalar>(b, d);
}

/**
 * @brief Compute the plane best fitting a wighted set of points
 * The algorithm used is the wighted Covariance matrix eigenvector approach.
 */
template<Point3Concept PointType>
Plane<typename PointType::ScalarType> fitPlaneToWeightedPointCloud(
    const std::vector<PointType>& pointVec,
    const std::vector<typename PointType::ScalarType>& weights)
{
    using Scalar = typename PointType::ScalarType;

    Matrix33<Scalar> covMat =
        weightedCovarianceMatrixOfPointCloud(pointVec, weights);
    Point3<Scalar> b =
        Polygon<PointType>::weightedBarycenter(pointVec, weights);

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<Scalar, 3, 3>> eig(covMat);

    Eigen::Matrix<Scalar, 3, 1> eval = eig.eigenvalues();
    Eigen::Matrix<Scalar, 3, 3> evec = eig.eigenvectors();

    eval = eval.cwiseAbs();
    int minInd;
    eval.minCoeff(&minInd);
    Point3<Scalar> d;
    d[0] = evec(0, minInd);
    d[1] = evec(1, minInd);
    d[2] = evec(2, minInd);

    return Plane<Scalar>(b, d);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_FITTING_H
