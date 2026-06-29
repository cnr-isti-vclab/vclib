// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_RENDER_APP_H
#define VCL_RENDER_CONCEPTS_RENDER_APP_H

#include <vclib/base.h>

namespace vcl {

template<typename T>
concept RenderAppConcept = requires (
    T&&                                obj,
    typename RemoveRef<T>::ParentType* pPtr,
    std::string                        s,
    uint                               u) {
    typename RemoveRef<T>::ParentType;

    RemoveRef<T>();
    RemoveRef<T>(pPtr);
    RemoveRef<T>(s, u, u);
    RemoveRef<T>(s, u, u, pPtr);

    // non const requirements
    requires IsConst<T> || requires {
        { obj.update() } -> std::same_as<void>;
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_RENDER_APP_H
