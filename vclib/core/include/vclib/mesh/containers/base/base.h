// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_BASE_BASE_H
#define VCL_MESH_CONTAINERS_BASE_BASE_H

#include "../../elements/base/base.h"

#include <vclib/base.h>
#include <vclib/space/core.h>

namespace vcl::mesh {

namespace detail {

/// Rebind

// Helper to rebind a container's ParentMesh to the actual Mesh class
template<typename T, typename NewParent>
struct RebindContainerHelper
{
    using type = T;
};

template<typename T, typename NewParent>
requires requires { typename T::template Rebind<NewParent>; }
struct RebindContainerHelper<T, NewParent>
{
    using type = typename T::template Rebind<NewParent>;
};

template<typename T, typename NewParent>
using RebindContainer = typename RebindContainerHelper<T, NewParent>::type;

/// Find Container

// Compile-time search for a container with the given ELEMENT_ID
template<ElemId ELEM_ID, typename... Types>
struct FindContainer;

template<ElemId ELEM_ID>
struct FindContainer<ELEM_ID>
{
    using type                  = void;
    static constexpr bool found = false;
};

template<ElemId ELEM_ID, typename Head, typename... Tail>
struct FindContainer<ELEM_ID, Head, Tail...>
{
private:
    static constexpr bool matches = requires {
        requires Head::ELEMENT_ID == ELEM_ID;
    };

public:
    using type = std::conditional_t<
        matches,
        Head,
        typename FindContainer<ELEM_ID, Tail...>::type>;
    static constexpr bool found =
        matches || FindContainer<ELEM_ID, Tail...>::found;
};

// Trait to check if a component has a static vector column (DataType is not void)
template<typename Comp>
struct HasVectorColumnPred
{
    static constexpr bool value = !std::is_void_v<typename Comp::DataType>;
};

// Trait to convert TypeWrapper<Components...> in a ParallelVectorsTuple
template<typename ComponentListWrapper>
struct ParallelVectorsTupleFromComponents;

template<typename... Components>
struct ParallelVectorsTupleFromComponents<vcl::TypeWrapper<Components...>>
{
    using type = ParallelVectorsTuple<typename Components::DataType...>;
};

} // namespace detail

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_BASE_BASE_H
