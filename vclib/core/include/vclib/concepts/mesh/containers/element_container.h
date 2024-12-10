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

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H

#include <vclib/concepts/mesh/components/component.h>
#include <vclib/concepts/mesh/elements/element.h>
#include <vclib/types/mesh_containers.h>

namespace vcl {
namespace mesh {

/**
 * @brief ElementContainerConcept is a concept satisfied when the type T is an
 * Element Container.
 */
template<typename T>
concept ElementContainerConcept =
    std::is_base_of<ElementContainerTriggerer, T>::value;

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

#endif // VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H
