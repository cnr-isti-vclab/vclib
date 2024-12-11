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
 * @brief The ComponentConcept is evaluated to true whenever the type `T` is a
 * valid component, i.e., it has a static constexpr member `COMPONENT_ID` of
 * type `unsigned int`.
 *
 * The `COMPONENT_ID` is used to identify the component type in the mesh or in
 * the mesh element. Its value should be one of the values defined in the
 * @ref vcl::CompId enum.
 *
 * @tparam T The type to be evaluated.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept ComponentConcept = requires {
    { RemoveRef<T>::COMPONENT_ID } -> std::same_as<const uint&>;
};

/**
 * @brief Evaluates to true if the type `T` is a component that is stored
 * vertically in its element container.
 *
 * @tparam T The type to be evaluated.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept IsVerticalComponent =
    ComponentConcept<T> && RemoveRef<T>::IS_VERTICAL == true && requires {
        typename RemoveRef<T>::DataValueType;
        { RemoveRef<T>::IS_VERTICAL } -> std::same_as<const bool&>;
    };

/**
 * @brief Evaluates to true if the type `T` is a component that is stored
 * vertically in its element container, and it is optional.
 *
 * @tparam T The type to be evaluated.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept IsOptionalComponent =
    IsVerticalComponent<T> && RemoveRef<T>::IS_OPTIONAL == true && requires {
        { RemoveRef<T>::IS_OPTIONAL } -> std::same_as<const bool&>;
    };

/**
 * @private
 * @brief Evaluates to true if the type `T` is a component that has a `init()`
 * member function, that must be called to initialize the component.
 *
 * The `init()` function is necessary in components that may be optional and,
 * when enabled, they need to be initialized.
 *
 * @note Only components that store data that require non-default initialization
 * should have the `init()` function.
 *
 * @tparam T The type to be evaluated.
 */
template<typename T>
concept HasInitMemberFunction = ComponentConcept<T> && requires (T&& obj) {
    { obj.init() } -> std::same_as<void>;
};

/**
 * @private
 * @brief Evaluates to true if the type `T` is a component that has a
 * `isAvailable()` member function, that must be called to check if the
 * component is available (i.e. it could be optional and not enabled).
 *
 * @tparam T The type to be evaluated.
 */
template<typename T>
concept HasIsAvailableMemberFunction =
    ComponentConcept<T> && requires (T&& obj) {
        { obj.isAvailable() } -> std::same_as<bool>;
    };

/**
 * @private
 * @brief Evaluates to true if the type `T` is tied to the number of vertices in
 * the face.
 *
 * A component that is tied to the vertex number if it belongs to a face element
 * and its data is composed of a number of elements that must be equal to the
 * number of vertices in the face.
 *
 * To be evaluated as true, the component must have a static constexpr member
 * `TIED_TO_VERTEX_NUMBER` of type `bool` set to `true`.
 *
 * @tparam T the type to be evaluated.
 */
template<typename T>
concept IsTiedToVertexNumber =
    ComponentConcept<T> && RemoveRef<T>::TIED_TO_VERTEX_NUMBER;

/**
 * @private
 * @brief The HasReferencesOfType concept checks whether a *component* T stores
 * references (pointers or indices) of a type (element) R.
 *
 * Each component that store pointers/indices of a type R, must:
 *
 * - inherit from ReferencesComponentTriggerer<R> (automatically inherited by
 *   inheriting from Component class)
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
 *
 * @tparam T the type to be evaluated.
 * @tparam R the type of the references.
 */
template<typename T, typename R>
concept HasReferencesOfType =
    ComponentConcept<T> &&
    std::is_base_of<ReferencesComponentTriggerer<RemoveRef<R>>, RemoveRef<T>>::
        value;

/**
 * @private
 * @brief See @ref vcl::comp::HasReferencesOfType and @ref
 * vcl::comp::IsOptionalComponent.
 *
 * @tparam T the type to be evaluated.
 * @tparam R the type of the references.
 */
template<typename T, typename R>
concept HasOptionalReferencesOfType =
    HasReferencesOfType<T, R> && IsOptionalComponent<T>;

/**
 * @private
 * @brief The HasPointersOfType concept checks whether a component T stores
 * pointers of a type (element) R.
 *
 * The constraints are the same of @ref vcl::comp::HasReferencesOfType, but this
 * concept requires the component stores only pointers (not indices).
 *
 * @tparam T the type to be evaluated.
 * @tparam R the type of the pointers.
 */
template<typename T, typename R>
concept HasPointersOfType = HasReferencesOfType<T, R> && requires (T&& obj) {
    obj.template pointers<RemoveRef<R>>();
};

/**
 * @private
 * @brief See @ref vcl::comp::HasPointersOfType and @ref
 * vcl::comp::IsOptionalComponent.
 *
 * @tparam T the type to be evaluated.
 * @tparam R the type of the pointers.
 */
template<typename T, typename R>
concept HasOptionalPointersOfType =
    IsOptionalComponent<T> && HasPointersOfType<T, R>;

/**
 * @private
 * @brief The HasIndicesOfType concept checks whether a component T stores
 * indices of a type (element) R.
 *
 * The constraints are the same of @ref vcl::comp::HasReferencesOfType, but this
 * concept requires the component stores only indices (not pointers).
 *
 * @tparam T the type to be evaluated.
 * @tparam R the type of the indices.
 */
template<typename T, typename R>
concept HasIndicesOfType = HasReferencesOfType<T, R> && requires (T&& obj) {
    obj.template indices<RemoveRef<R>>();
};

/**
 * @private
 * @brief See @ref vcl::comp::HasIndicesOfType and @ref
 * vcl::comp::IsOptionalComponent.
 *
 * @tparam T the type to be evaluated.
 * @tparam R the type of the indices.
 */
template<typename T, typename R>
concept HasOptionalIndicesOfType =
    IsOptionalComponent<T> && HasIndicesOfType<T, R>;

// ======== Has Component Concepts ======== //
// Concepts that needs to be called on a type T that has the "Components" type
// defined as a TypeWrapper of components. The type T is generally a Mesh or a
// MeshElement.

/**
 * @brief The HasComponentOfType concept checks whether a type `T` (that may be
 * a Mesh or a MeshElement) has a component having its @ref vcl::CompId equal to
 * `COMP_ID`.
 *
 * The concept looks for the component having `COMP_ID` in the inner type
 * `Components` type defined in the type `T`, that must be a @ref
 * vcl::TypeWrapper of components.
 *
 * @tparam T The type to be evaluated.
 * @tparam COMP_ID The id of the component to be checked.
 *
 * @ingroup components_concepts
 * @ingroup elements_concepts
 */
template<typename T, uint COMP_ID>
concept HasComponentOfType = detail::
    ComponentOfTypePred<COMP_ID, typename RemoveRef<T>::Components>::value;

/**
 * @brief The HasVerticalComponentOfType concept checks whether a type `T` (that
 * should be a MeshElement) has a vertical component having its @ref vcl::CompId
 * equal to `COMP_ID`.
 *
 * The concept looks for the component having `COMP_ID` in the inner type
 * `Components` type defined in the type `T`, that must be a @ref
 * vcl::TypeWrapper of components. If the component exists, it checks if it is
 * vertical.
 *
 * @tparam T The type to be evaluated.
 * @tparam COMP_ID The id of the component to be checked.
 *
 * @ingroup components_concepts
 * @ingroup elements_concepts
 */
template<typename T, uint COMP_ID>
concept HasVerticalComponentOfType =
    HasComponentOfType<T, COMP_ID> &&
    IsVerticalComponent<
        ComponentOfType<COMP_ID, typename RemoveRef<T>::Components>>;

/**
 * @brief The HasOptionalComponentOfType concept checks whether a type `T` (that
 * should be a MeshElement) has an optional component having its @ref
 * vcl::CompId equal to `COMP_ID`.
 *
 * The concept looks for the component having `COMP_ID` in the inner type
 * `Components` type defined in the type `T`, that must be a @ref
 * vcl::TypeWrapper of components. If the component exists, it checks if it is
 * optional.
 *
 * @tparam T The type to be evaluated.
 * @tparam COMP_ID The id of the component to be checked.
 *
 * @ingroup components_concepts
 * @ingroup elements_concepts
 */
template<typename T, uint COMP_ID>
concept HasOptionalComponentOfType =
    HasComponentOfType<T, COMP_ID> &&
    IsOptionalComponent<
        ComponentOfType<COMP_ID, typename RemoveRef<T>::Components>>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H
