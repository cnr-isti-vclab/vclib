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

#ifndef VCL_ALGORITHMS_CORE_STAT_H
#define VCL_ALGORITHMS_CORE_STAT_H

#include <vclib/space/core/matrix.h>
#include <vclib/space/core/polygon.h>

#include <vector>

namespace vcl {

/**
 * @brief Compute the covariance matrix of a set of points.
 * @param pointVec
 * @return The 3x3 covariance matrix of the given set of points.
 */
template<Point3Concept PointType>
auto covarianceMatrixOfPointCloud(const std::vector<PointType>& pointVec)
{
    Matrix33<typename PointType::ScalarType> m;
    m.setZero();
    PointType barycenter = Polygon<PointType>::barycenter(pointVec);

    // compute covariance matrix
    for (const PointType& p : pointVec) {
        PointType e = p - barycenter;
        m += e.outerProduct(e);
    }
    return m;
}

/**
 * @brief Compute the weighted covariance matrix of a set of points.
 * @param pointVec
 * @param weights
 * @return
 */
template<Point3Concept PointType>
auto weightedCovarianceMatrixOfPointCloud(
    const std::vector<PointType>&                      pointVec,
    const std::vector<typename PointType::ScalarType>& weights)
{
    Matrix33<typename PointType::ScalarType> m;
    m.setZero();
    PointType barycenter =
        Polygon<PointType>::weightedBarycenter(pointVec, weights);

    // compute covariance matrix
    typename PointType::ScalarType wsum = 0;
    for (uint i = 0; i < pointVec.size(); ++i) {
        PointType e = (pointVec[i] - barycenter) * weights[i];
        m += e.outerProduct(e);
        wsum += weights[i];
    }
    return m / wsum;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_STAT_H
