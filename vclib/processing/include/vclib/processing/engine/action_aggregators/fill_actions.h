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

#ifndef VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_FILL_ACTIONS_H
#define VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_FILL_ACTIONS_H

#include <vclib/processing/engine/settings.h>

#include <vclib/types.h>

namespace vcl::proc::detail {

template<template<typename> typename Act>
void fillWithSupportedMeshTypes(auto& array, uint& firstMeshType)
{
    firstMeshType = toUnderlying(MeshTypeId::COUNT);

    uint i    = 0;
    auto fAct = [&]<typename MeshType>() {
        if constexpr (IsInstantiable<Act, MeshType>) {
            array[toUnderlying(meshTypeId<MeshType>())] =
                std::make_shared<Act<MeshType>>();
            if (i < firstMeshType) {
                firstMeshType = i;
            }
        }
        ++i;
    };

    vcl::ForEachType<MeshTypes>::apply(fAct);
}

} // namespace vcl::proc::detail

#endif // VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_FILL_ACTIONS_H
