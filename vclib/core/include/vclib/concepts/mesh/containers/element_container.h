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

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H

#include <vclib/types.h>

namespace vcl::mesh {

/**
 * @brief ElementContainerConcept is a concept satisfied when the type T is an
 * Element Container.
 */
template<typename T>
concept ElementContainerConcept =
    IsElementContainerPred<T>::value && requires (T&& obj) {
        typename RemoveRef<T>::ElementType;
        typename RemoveRef<T>::ParentMeshType;

        { obj.ELEMENT_ID } -> std::same_as<const uint&>;
    };

} // namespace vcl::mesh

#endif // VCL_CONCEPTS_MESH_CONTAINERS_ELEMENT_CONTAINER_H
