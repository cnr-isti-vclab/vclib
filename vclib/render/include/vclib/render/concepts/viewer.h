// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_VIEWER_H
#define VCL_RENDER_CONCEPTS_VIEWER_H

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/editors.h>

#include <utility>

namespace vcl {

/**
 * @brief Concept that verifies if a class provides the standard viewer interface.
 *
 * A type satisfies ViewerConcept if it provides the base types `ViewerType` and 
 * `EditorType`, exposes a `drawableObjectVector`, and provides methods to set 
 * the drawable object vector, push a drawable object, refresh editors, and 
 * fit the scene.
 *
 * @tparam T: The type to be checked against the ViewerConcept.
 */
template<typename T>
concept ViewerConcept =
    requires (
        T&& obj,
        std::shared_ptr<vcl::DrawableObjectVector> vec,
        vcl::DrawableObject&& drawableObj) {
        typename RemoveRef<T>::ViewerType;
        typename RemoveRef<T>::EditorType;

        {
            std::as_const(obj).drawableObjectVector()
        } -> std::same_as<const vcl::DrawableObjectVector&>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.setDrawableObjectVector(vec) } -> std::same_as<void>;

            { obj.pushDrawableObject(std::move(drawableObj)) } -> std::same_as<uint>;

            { obj.refreshEditors() } -> std::same_as<void>;

            { obj.fitScene() } -> std::same_as<void>;
        };
    };

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_VIEWER_H
