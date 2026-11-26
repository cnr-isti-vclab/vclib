// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BENCH_TYPES_H
#define BENCH_TYPES_H

#include <vclib/meshes.h>

#include <string_view>
#include <type_traits>

namespace bench {

template<typename Func>
void forEachMeshType(Func&& func)
{
    func(std::type_identity<vcl::TriMesh> {}, std::string_view("trimesh"));
    func(std::type_identity<vcl::TriMeshf> {}, std::string_view("trimeshf"));
    func(std::type_identity<vcl::TriMesh64> {}, std::string_view("trimesh64"));
    func(
        std::type_identity<vcl::TriMesh64f> {}, std::string_view("trimesh64f"));
}

} // namespace bench

#endif // BENCH_TYPES_H
