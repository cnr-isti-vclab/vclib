// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_COMPONENT_PREDICATES_H
#define VCL_MESH_COMPONENTS_BASE_COMPONENT_PREDICATES_H

#include "component.h"
#include "../../elements/base/base.h"

#include <vclib/base.h>

namespace vcl::comp {

namespace detail {

template<typename Comp>
concept IsOptionalComponent =
    requires { requires std::remove_cvref_t<Comp>::IS_OPTIONAL; };

template<CompId COMP_ID, typename T>
inline constexpr bool HasComponentImpl =
    requires { requires std::remove_cvref_t<T>::COMPONENT_ID == COMP_ID; };

template<CompId COMP_ID, typename... Components>
inline constexpr bool
    HasComponentImpl<COMP_ID, vcl::TypeWrapper<Components...>> =
        (HasComponentImpl<COMP_ID, Components> || ...);

// Helper details for finding the index of a Component by COMP_ID in a list of
// Components. It recursively checks each component's COMPONENT_ID against the
// target COMP_ID, incrementing the CurrentIndex until a match is found or the
// list is exhausted.
template<CompId COMP_ID, uint CurrentIndex, typename... Components>
struct IndexOfCompIdImpl;

template<CompId COMP_ID, uint CurrentIndex, typename First, typename... Rest>
struct IndexOfCompIdImpl<COMP_ID, CurrentIndex, First, Rest...>
{
    static constexpr uint value =
        HasComponentImpl<COMP_ID, First> ?
            CurrentIndex :
            IndexOfCompIdImpl<COMP_ID, CurrentIndex + 1, Rest...>::value;
};

// Base case when the component is not found
template<CompId COMP_ID, uint CurrentIndex>
struct IndexOfCompIdImpl<COMP_ID, CurrentIndex>
{
    static constexpr uint value = UINT_NULL;
};

template<typename... Ts>
vcl::TypeWrapper<Ts...> extractTypeWrapper(const vcl::TypeWrapper<Ts...>&);

// Helper to extract the inner Components type from a type T if present,
// or the underlying TypeWrapper if T inherits from TypeWrapper,
// otherwise uses T directly.
template<typename T>
struct ComponentListHelper
{
private:
    static auto helper()
    {
        using RawT = std::remove_cvref_t<T>;
        if constexpr (requires { typename RawT::Components; }) {
            return typename RawT::Components {};
        }
        else if constexpr (requires {
                               extractTypeWrapper(std::declval<RawT>());
                           }) {
            return decltype(extractTypeWrapper(std::declval<RawT>())) {};
        }
        else {
            return RawT {};
        }
    }

public:
    using type = decltype(helper());
};

template<ElemId TARGET_ELEM_ID, typename T>
struct HasReferencesToImpl : std::false_type {};

template<ElemId TARGET_ELEM_ID, typename Comp>
requires requires {
    requires std::remove_cvref_t<Comp>::template referencesElement<TARGET_ELEM_ID>();
}
struct HasReferencesToImpl<TARGET_ELEM_ID, Comp> :
    std::bool_constant<std::remove_cvref_t<Comp>::template referencesElement<TARGET_ELEM_ID>()> {};

template<ElemId TARGET_ELEM_ID, typename... Comps>
struct HasReferencesToImpl<TARGET_ELEM_ID, vcl::TypeWrapper<Comps...>> :
    std::bool_constant<(HasReferencesToImpl<TARGET_ELEM_ID, Comps>::value || ...)> {};

template<ElemId TARGET_ELEM_ID, typename T>
struct HasOptionalReferencesToImpl : std::false_type {};

template<ElemId TARGET_ELEM_ID, typename Comp>
requires (HasReferencesToImpl<TARGET_ELEM_ID, Comp>::value && IsOptionalComponent<Comp>)
struct HasOptionalReferencesToImpl<TARGET_ELEM_ID, Comp> : std::true_type {};

template<ElemId TARGET_ELEM_ID, typename... Comps>
struct HasOptionalReferencesToImpl<TARGET_ELEM_ID, vcl::TypeWrapper<Comps...>> :
    std::bool_constant<(HasOptionalReferencesToImpl<TARGET_ELEM_ID, Comps>::value || ...)> {};

} // namespace detail

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) contains a
 * component with the given COMP_ID.
 */
template<CompId COMP_ID, typename T>
concept HasComponent = detail::
    HasComponentImpl<COMP_ID, typename detail::ComponentListHelper<T>::type>;

/**
 * @brief Concept that checks whether a component type is optional
 * (has static constexpr bool IS_OPTIONAL = true).
 */
template<typename Comp>
concept IsOptionalComponent = detail::IsOptionalComponent<Comp>;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) stores
 * references to elements with TARGET_ELEM_ID (e.g. ElemId::VERTEX).
 */
template<ElemId TARGET_ELEM_ID, typename T>
concept HasReferencesTo = detail::HasReferencesToImpl<
    TARGET_ELEM_ID,
    typename detail::ComponentListHelper<T>::type>::value;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) stores
 * optional references to elements with TARGET_ELEM_ID (e.g. ElemId::VERTEX).
 */
template<ElemId TARGET_ELEM_ID, typename T>
concept HasOptionalReferencesTo = detail::HasOptionalReferencesToImpl<
    TARGET_ELEM_ID,
    typename detail::ComponentListHelper<T>::type>::value;

/**
 * @brief Concept that checks whether a component Comp is tied to the vertex
 * count (has static constexpr bool TIED_TO_VERTEX_COUNT = true).
 */
template<typename Comp>
concept IsTiedToVertexCount = requires {
    {
        std::remove_cvref_t<Comp>::TIED_TO_VERTEX_COUNT
    } -> std::convertible_to<bool>;
} && std::remove_cvref_t<Comp>::TIED_TO_VERTEX_COUNT;

/**
 * @brief Trait to find the index of a Component by its COMP_ID inside a
 * TypeWrapper.
 *
 * Usage: IndexOfCompId<COMP_ID, TypeWrapper<...>>::value
 */
template<CompId COMP_ID, typename ComponentListWrapper>
struct IndexOfCompId;

template<CompId COMP_ID, typename... Components>
struct IndexOfCompId<COMP_ID, vcl::TypeWrapper<Components...>>
{
    static_assert(
        detail::HasComponentImpl<COMP_ID, vcl::TypeWrapper<Components...>>,
        "Component ID not found in the ComponentList");

    static constexpr uint value =
        detail::IndexOfCompIdImpl<COMP_ID, 0, Components...>::value;
};

/**
 * @brief Trait to extract the Component type with the given COMP_ID from a
 * TypeWrapper or from an object that contains components (ElementProxy,
 * ElementContainer, Mesh, etc.).
 */
template<CompId COMP_ID, typename ComponentListWrapper>
struct ComponentType;

template<CompId COMP_ID>
struct ComponentType<COMP_ID, vcl::TypeWrapper<>>
{
    using type = void;
};

template<CompId COMP_ID, typename First, typename... Rest>
struct ComponentType<COMP_ID, vcl::TypeWrapper<First, Rest...>>
{
    using type = std::conditional_t<
        First::COMPONENT_ID == COMP_ID,
        First,
        typename ComponentType<COMP_ID, vcl::TypeWrapper<Rest...>>::type>;
};

template<CompId COMP_ID, typename T>
using ComponentTypeT = typename ComponentType<
    COMP_ID,
    typename detail::ComponentListHelper<T>::type>::type;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) contains an
 * optional component with the given COMP_ID.
 */
template<CompId COMP_ID, typename T>
concept HasOptionalComponent =
    HasComponent<COMP_ID, T> &&
    detail::IsOptionalComponent<ComponentTypeT<COMP_ID, T>>;

/**
 * @brief Concept that checks whether a component type provides a default value,
 * either through a static constexpr defaultValue() member function or a
 * static constexpr DEFAULT_VALUE variable.
 *
 * @tparam Comp: The component type to check.
 *
 * @ingroup components_concepts
 */
template<typename Comp>
concept HasDefaultValue = requires {
    {
        std::remove_cvref_t<Comp>::defaultValue()
    } -> std::convertible_to<typename std::remove_cvref_t<Comp>::DataType>;
} || requires {
    {
        std::remove_cvref_t<Comp>::DEFAULT_VALUE
    } -> std::convertible_to<typename std::remove_cvref_t<Comp>::DataType>;
};

/**
 * @brief Returns the default initial value for a component type satisfying
 * HasDefaultValue.
 *
 * @tparam Comp: The component type.
 * @return The default value.
 */
template<typename Comp>
constexpr auto defaultValue()
{
    using PureComp = std::remove_cvref_t<Comp>;
    if constexpr (requires {
                      {
                          PureComp::defaultValue()
                      } -> std::convertible_to<typename PureComp::DataType>;
                  }) {
        return PureComp::defaultValue();
    }
    else if constexpr (requires {
                           {
                               PureComp::DEFAULT_VALUE
                           }
                           -> std::convertible_to<typename PureComp::DataType>;
                       }) {
        return PureComp::DEFAULT_VALUE;
    }
}

/**
 * @brief Concept that checks whether a CompId corresponds to a standard
 * data attribute component (non-structural, non-reference, non-TTVC).
 *
 * These components can be safely accessed as raw values on ElementProxy
 * via componentValue<COMP_ID>() and on ElementContainerProxy via
 * component<COMP_ID>() without risking container invariants or
 * desynchronizing Tied-To-Vertex-Count (TTVC) dynamic arrays.
 */
template<CompId COMP_ID>
concept IsAttributeComponent =
    COMP_ID == CompId::POSITION ||
    COMP_ID == CompId::NORMAL ||
    COMP_ID == CompId::COLOR ||
    COMP_ID == CompId::QUALITY ||
    COMP_ID == CompId::TEX_COORD ||
    COMP_ID == CompId::TANGENT ||
    COMP_ID == CompId::PRINCIPAL_CURVATURE ||
    COMP_ID == CompId::MATERIAL_INDEX;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_COMPONENT_PREDICATES_H
