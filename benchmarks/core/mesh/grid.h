// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BENCH_GRID_H
#define BENCH_GRID_H

#include <vclib/base.h>

#include <Eigen/Core>

#include <string>

namespace bench {

template<
    typename VMatrix = Eigen::MatrixX3d,
    typename FMatrix = Eigen::MatrixX3i>
struct MeshMatrices
{
    VMatrix vertices;
    FMatrix faces;
};

/**
 * @brief Generates a regular triangulated grid as Eigen matrices.
 *
 * @param rows Number of vertex rows.
 * @param cols Number of vertex columns.
 * @return MeshMatrices containing vertices (rows*cols x 3) and faces
 * ((rows-1)*(cols-1)*2 x 3).
 */
inline MeshMatrices<Eigen::MatrixX3d, Eigen::MatrixX3i> makeGridMatrices(
    vcl::uint rows,
    vcl::uint cols)
{
    MeshMatrices<Eigen::MatrixX3d, Eigen::MatrixX3i> m;
    vcl::uint                                        numVerts = rows * cols;
    vcl::uint numFaces = (rows - 1) * (cols - 1) * 2;

    m.vertices.resize(numVerts, 3);
    m.faces.resize(numFaces, 3);

    for (vcl::uint r = 0; r < rows; ++r) {
        for (vcl::uint c = 0; c < cols; ++c) {
            vcl::uint idx      = r * cols + c;
            m.vertices(idx, 0) = static_cast<double>(c);
            m.vertices(idx, 1) = static_cast<double>(r);
            m.vertices(idx, 2) = 0.0;
        }
    }

    vcl::uint fIdx = 0;
    for (vcl::uint r = 0; r < rows - 1; ++r) {
        for (vcl::uint c = 0; c < cols - 1; ++c) {
            vcl::uint v00 = r * cols + c;
            vcl::uint v01 = r * cols + (c + 1);
            vcl::uint v10 = (r + 1) * cols + c;
            vcl::uint v11 = (r + 1) * cols + (c + 1);

            m.faces(fIdx, 0) = v00;
            m.faces(fIdx, 1) = v10;
            m.faces(fIdx, 2) = v01;
            fIdx++;

            m.faces(fIdx, 0) = v01;
            m.faces(fIdx, 1) = v10;
            m.faces(fIdx, 2) = v11;
            fIdx++;
        }
    }

    return m;
}

} // namespace bench

#endif // BENCH_GRID_H
