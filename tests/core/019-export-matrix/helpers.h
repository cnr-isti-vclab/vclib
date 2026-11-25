// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HELPERS_H
#define HELPERS_H

#include <vclib/meshes/edge_mesh.h>
#include <vclib/meshes/poly_mesh.h>
#include <vclib/meshes/tri_mesh.h>

#include <random>
#include <tuple>

template<typename ScalarType>
using EigenRowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

template<typename ScalarType>
using Eigen3RowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 3, Eigen::RowMajor>;

template<typename ScalarType>
using Eigen4RowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 4, Eigen::RowMajor>;

template<typename ScalarType>
using EigenColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;

template<typename ScalarType>
using Eigen3ColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 3, Eigen::ColMajor>;

template<typename ScalarType>
using Eigen4ColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 4, Eigen::ColMajor>;

template<typename ScalarType, typename...>
using EigenVector = Eigen::Vector<ScalarType, Eigen::Dynamic>;

template<typename ScalarType, typename...>
using VclVector = vcl::Vector<ScalarType, -1>;

using Meshes   = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf  = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using Meshes64 = std::tuple<vcl::TriMesh64, vcl::PolyMesh64, vcl::EdgeMesh64>;
using Meshes64f =
    std::tuple<vcl::TriMesh64f, vcl::PolyMesh64f, vcl::EdgeMesh64f>;

template<vcl::ElemId ELEM_ID>
void randomSelection(auto& tm)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (auto el : tm.template elements<ELEM_ID>()) {
        bool sel      = dis(gen);
        el.selected() = sel;
    }
}

template<vcl::ElemId ELEM_ID>
void randomColor(auto& tm)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    tm.template enableElementComponent<ELEM_ID, vcl::CompId::COLOR>();

    for (auto el : tm.template elements<ELEM_ID>()) {
        el.color() = vcl::Color(dis(gen), dis(gen), dis(gen), dis(gen));
    }
}

template<vcl::ElemId ELEM_ID>
void randomQuality(auto& tm)
{
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(-100, 100);

    tm.template enableElementComponent<ELEM_ID, vcl::CompId::QUALITY>();

    for (auto el : tm.template elements<ELEM_ID>()) {
        double qual  = dis(gen);
        el.quality() = qual;
    }
}

#endif // HELPERS_H
