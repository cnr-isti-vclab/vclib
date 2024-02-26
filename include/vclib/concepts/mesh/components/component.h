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

namespace detail {

/**
 * @brief Given the ID of a Component and a list of Component types,
 * this predicate sets its bool `value` to `true` if there exists a Component in
 * the list having the COMP_ID, and sets `type` to the TypeWrapper of the
 * found component.
 *
 * If no Component was found, value will be set to `false` and type will contain
 * an empty TypeWrapper.
 */
template<uint COMP_ID, typename... Components>
struct ComponentOfTypePred
{
private:
    template<typename Comp>
    struct SameCompPred
    {
        static constexpr bool value = Comp::COMPONENT_ID == COMP_ID;
    };

public:
    // TypeWrapper of the found container, if any
    using type =
        typename vcl::FilterTypesByCondition<SameCompPred, Components...>::type;
    static constexpr bool value = NumberOfTypes<type>::value == 1;
};

// TypeWrapper specialization
template<uint COMP_ID, typename... Components>
struct ComponentOfTypePred<COMP_ID, TypeWrapper<Components...>> :
        public ComponentOfTypePred<COMP_ID, Components...>
{
};

} // namespace detail

/**
 * @brief The ComponentConcept is evaluated to true whenever the type T is a
 * valid component.
 *
 * @tparam T: the type to be evaluated
 */
template<typename T>
concept ComponentConcept = requires {
    // clang-format off
    { T::COMPONENT_ID } -> std::same_as<const uint&>;
    // clang-format on
};

/**
 * @brief The predicate struct IsComponentPred sets its bool `value` to `true`
 * when the type T satisfies the ComponentConcept concept
 *
 * @tparam T: the type to be evaluated
 */
template<typename T>
struct IsComponentPred
{
    static const bool value = ComponentConcept<T>;
};

template<uint COMP_ID, typename... Components>
using ComponentOfType = typename FirstType<
    typename detail::ComponentOfTypePred<COMP_ID, Components...>::type>::type;

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
concept IsVerticalComponent = T::IS_VERTICAL == true && requires (T o) {
    // clang-format off
    typename T::DataValueType;
    { o.IS_VERTICAL } -> std::same_as<const bool&>;
    // clang-format on
};

template<typename T>
struct IsVerticalComponentPred
{
    static const bool value = IsVerticalComponent<T>;
};

template<typename T>
concept IsOptionalComponent =
    IsVerticalComponent<T> && T::IS_OPTIONAL == true && requires (T o) {
        // clang-format off
        { o.IS_OPTIONAL } -> std::same_as<const bool&>;
        // clang-format on
    };

template<typename T>
class PointersComponentTriggerer
{
};

/**
 * @brief HasPointersOfType concept
 *
 * Each component that store pointers of a type R, must:
 *
 * - inherit from PointersComponentTriggerer<R> (automatic from Component class)
 * - provide the following **protected** member functions:
 *   - void updatePointers(const R* oldBase, const R* newBase);
 *
 *     the function updates the stored R pointers having the old base of the
 *     container and the new base of the container.
 *
 *   - void updatePointers(
 *         const R* base, const std::vector<uint>& newIndices);
 *
 *     the function updates the stored R pointers having the base of the
 *     container, and, for each old element index, its new index in the
 *     container.
 *
 *   - template<typename Element, typename ElRType>
 *     void importPointersFrom(
 *         const Element& e, const R* base, const ElRType* ebase);
 *
 *     the function imports the pointers from the pointers of another element.
 *     - e is the another element;
 *     - base is the base of container that stores this element
 *     - ebase is the base of the container that stores the another elements
 */
template<typename T, typename R>
concept HasPointersOfType =
    std::is_base_of<PointersComponentTriggerer<R>, T>::value;

template<typename T, typename R>
concept HasOptionalPointersOfType =
    HasPointersOfType<T, R> && IsOptionalComponent<T>;

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
