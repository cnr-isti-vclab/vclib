// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENT_H
#define VCL_MESH_VIEWS_ELEMENT_H

#include <vclib/base.h>

namespace vcl::views {

namespace detail {
    struct UnsupportedElementView {};
}

template<uint ELEM_ID>
inline constexpr auto elements = detail::UnsupportedElementView{};

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_ELEMENT_H
