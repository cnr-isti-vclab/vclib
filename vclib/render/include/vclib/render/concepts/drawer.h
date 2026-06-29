// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_DRAWER_H
#define VCL_RENDER_CONCEPTS_DRAWER_H

#include <vclib/base.h>

namespace vcl {

template<typename T>
concept DrawerConcept = requires (T&& obj) {
    // constructors
    RemoveRef<T>();
    RemoveRef<T>(uint(), uint());

    // non const requirements
    requires IsConst<T> || requires {
        { obj.onInit(uint()) } -> std::same_as<void>;
        { obj.onResize(uint(), uint()) } -> std::same_as<void>;
        { obj.onDraw(uint()) } -> std::same_as<void>;
        { obj.onDrawId(uint()) } -> std::same_as<void>;
        { obj.onDrawContent(uint()) } -> std::same_as<void>;
        { obj.onPostDraw() } -> std::same_as<void>;
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_DRAWER_H
