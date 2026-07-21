// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_CORE_STAT_H
#define VCL_ALGORITHMS_CORE_STAT_H

#include <vclib/space/core.h>

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
