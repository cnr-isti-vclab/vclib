/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_MESH_CONTAINERS_BASE_BASE_H
#define VCL_MESH_CONTAINERS_BASE_BASE_H

#include <vclib/mesh/components/concepts/component.h>

#include <vclib/base.h>

namespace vcl {

namespace mesh {

namespace detail {

/**
 * @brief Given the ID of an Element and a list of ElementContainers, this
 * predicate sets its bool `value` to `true` if there exists an ElementContainer
 * in the list that contains Elements having the ELEM_ID, and sets `type` to
 * the TypeWrapper of the found container.
 *
 * If no Container was found, value will be set to `false` and type will contain
 * an empty TypeWrapper.
 */
template<uint ELEM_ID, typename... Containers>
struct ContainerOfElementPred
{
private:
    template<typename Cont>
    struct SameElPred
    {
        static constexpr bool value = Cont::ELEMENT_ID == ELEM_ID;
    };

public:
    // TypeWrapper of the found container, if any
    using type =
        typename FilterTypesByCondition<SameElPred, Containers...>::type;
    static constexpr bool value = NumberOfTypes<type>::value == 1;
};

// TypeWrapper specialization
template<uint ELEM_ID, typename... Containers>
struct ContainerOfElementPred<ELEM_ID, TypeWrapper<Containers...>> :
        public ContainerOfElementPred<ELEM_ID, Containers...>
{
};

} // namespace detail

class ElementContainerTriggerer
{
};

/**
 * @brief The predicate IsElementContainerPred sets its bool `value` to `true`
 * when the type T satisfies the ElementContainerConcept concept
 */
template<typename T>
struct IsElementContainerPred
{
    static const bool value =
        std::is_base_of<ElementContainerTriggerer, RemoveRef<T>>::value;
};

/**
 * @brief The ContainerOfElement structure exposes the type of the container of
 * the given MeshType having element with the given ELEM_ID.
 *
 * If no container was found, the usage of this structure will cause a build
 * error.
 *
 * @tparam ELEM_ID: The ID of the Element
 * @tparam MeshType: The MeshType of the Mesh from which we need the type of the
 * Container of Element having the given ElementEnumType
 *
 * Usage:
 * @code{.cpp}
 *
 * using VertexContainer = typename mesh::ContainerOfElement<ElemId::VERTEX,
 *                                                           MyMesh>::type;
 *
 * @endcode
 */
template<uint ELEM_ID, typename MeshType>
struct ContainerOfElement
{
public:
    using type = FirstTypeT<typename detail::ContainerOfElementPred<
        ELEM_ID,
        typename RemoveRef<MeshType>::Containers>::type>;
};

/**
 * @brief The ContainerOfElementType structure exposes the type of the container
 * of the given MeshType having Element with ID ELEM_ID.
 *
 * If no container was found, the usage of this structure will cause a build
 * error.
 *
 * @tparam ELEM_ID: The ElementEnumType of the Element
 * @tparam MeshType: The MeshType of the Mesh from which we need the type of the
 * Container of Element having the given ELEM_ID
 *
 * Usage:
 * @code{.cpp}
 *
 * using VertexContainer = mesh::ContainerOfElementType<ElemId::VERTEX, MyMesh>;
 *
 * @endcode
 */
template<uint ELEM_ID, typename MeshType>
using ContainerOfElementType =
    typename ContainerOfElement<ELEM_ID, MeshType>::type;

template<typename El, typename MeshType>
struct HasContainerOfPred
{
    static constexpr bool value = detail::ContainerOfElementPred<
        RemoveRef<El>::ELEMENT_ID,
        typename RemoveRef<MeshType>::Containers>::value;
};

template<uint ELEM_ID, typename MeshType>
struct HasContainerOfElementPred
{
    static constexpr bool value = detail::ContainerOfElementPred<
        ELEM_ID,
        typename RemoveRef<MeshType>::Containers>::value;
};

template<typename MeshType, uint ELEM_ID>
concept HasElementContainer =
    HasContainerOfElementPred<ELEM_ID, MeshType>::value;

template<typename MeshType, uint ELEM_ID, uint COMP_ID>
concept HasPerElementComponent =
    HasElementContainer<MeshType, ELEM_ID> &&
    comp::HasComponentOfType<
        typename ContainerOfElementType<ELEM_ID, MeshType>::ElementType,
        COMP_ID>;

template<typename MeshType, uint ELEM_ID, uint COMP_ID>
concept HasPerElementOptionalComponent =
    HasElementContainer<MeshType, ELEM_ID> &&
    comp::HasOptionalComponentOfType<
        typename ContainerOfElementType<ELEM_ID, MeshType>::ElementType,
        COMP_ID>;

} // namespace mesh

} // namespace vcl

#endif // VCL_MESH_CONTAINERS_BASE_BASE_H
