// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_VIEWER_H
#define VCL_RENDER_CONCEPTS_VIEWER_H

#include <vclib/render/drawable/drawable_object_vector.h>

#include <utility>

namespace vcl {

template<typename T>
concept ViewerConcept =
    requires (T&& obj, std::shared_ptr<vcl::DrawableObjectVector> vec) {
        {
            std::as_const(obj).drawableObjectVector()
        } -> std::same_as<const vcl::DrawableObjectVector&>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.setDrawableObjectVector(vec) } -> std::same_as<void>;
        };
    };

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_VIEWER_H
