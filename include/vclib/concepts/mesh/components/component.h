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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H
#define VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H

#include <vclib/types.h>

namespace vcl::comp {

/**
 * @brief The ComponentConcept is evaluated to true whenever the type T is a
 * valid component.
 *
 * @tparam T the type to be evaluated
 */
template<typename T>
concept ComponentConcept = requires {
    // clang-format off
    { T::COMPONENT_ID } -> std::same_as<const uint&>;
    // clang-format on
};

template<typename T>
concept HasInitMemberFunction = requires (T o) {
    // clang-format off
    { o.init() } -> std::same_as<void>;
    // clang-format on
};

template<typename T>
concept HasIsAvailableMemberFunction = requires (T o) {
    // clang-format off
    { o.isAvailable() } -> std::same_as<bool>;
    // clang-format on
};

template<typename T>
concept IsTiedToVertexNumber = T::TIED_TO_VERTEX_NUMBER;

template<typename T>
concept IsVerticalComponent = T::IS_VERTICAL == true && requires {
    // clang-format off
    typename T::DataValueType;
    { T::IS_VERTICAL } -> std::same_as<const bool&>;
    // clang-format on
};

template<typename T>
concept IsOptionalComponent =
    IsVerticalComponent<T> && T::IS_OPTIONAL == true && requires {
        // clang-format off
        { T::IS_OPTIONAL } -> std::same_as<const bool&>;
        // clang-format on
    };

/**
 * @brief The HasReferencesOfType concept checks whether a type T stores
 * references (pointers or indices) of a type R.
 *
 * Each component that store pointers/indices of a type R, must:
 *
 * - inherit from ReferencesComponentTriggerer<R> (automatic from Component
 *   class)
 * - provide the following **protected** member functions:
 *   - void updateReferences(const R* oldBase, std::size_t offset = 0);
 *
 *     the function updates the stored R pointers/indices having the old base
 *     of the container (needed to update pointers) and the possible non-zero
 *     offset (used for append operations).
 *
 *   - void updateReferences(const std::vector<uint>& newIndices);
 *
 *     the function updates the stored R pointers/indices having, for each old
 *     element index, its new index in the container.
 */
template<typename T, typename R>
concept HasReferencesOfType =
    std::is_base_of<ReferencesComponentTriggerer<R>, T>::value;

template<typename T, typename R>
concept HasOptionalReferencesOfType =
    HasReferencesOfType<T, R> && IsOptionalComponent<T>;

template<typename T, typename R>
concept HasPointersOfType =
    HasReferencesOfType<T, R> && requires (T o) { o.template pointers<R>(); };

template<typename T, typename R>
concept HasOptionalPointersOfType =
    HasOptionalReferencesOfType<T, R> &&
    requires (T o) { o.template pointers<R>(); };

template<typename T, typename R>
concept HasIndicesOfType =
    HasReferencesOfType<T, R> && requires (T o) { o.template indices<R>(); };

template<typename T, typename R>
concept HasOptionalIndicesOfType = HasOptionalReferencesOfType<T, R> &&
                                   requires (T o) { o.template indices<R>(); };

// ======== Has Component Concepts ======== //
// Concepts that needs to be called on a type T that has the "Components" type
// defined as a TypeWrapper of components. The type T is generally a Mesh or a
// MeshElement.

template<typename T, uint COMP_ID>
concept HasComponentOfType =
    detail::ComponentOfTypePred<COMP_ID, typename T::Components>::value;

template<typename T, uint COMP_ID>
concept HasVerticalComponentOfType =
    HasComponentOfType<T, COMP_ID> &&
    IsVerticalComponent<ComponentOfType<COMP_ID, typename T::Components>>;

template<typename T, uint COMP_ID>
concept HasOptionalComponentOfType =
    HasComponentOfType<T, COMP_ID> &&
    IsOptionalComponent<ComponentOfType<COMP_ID, typename T::Components>>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H
