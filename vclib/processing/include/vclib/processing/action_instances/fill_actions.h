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

#ifndef VCL_PROCESSING_ACTION_INSTANCES_FILL_ACTIONS_H
#define VCL_PROCESSING_ACTION_INSTANCES_FILL_ACTIONS_H

#include <vclib/processing/engine/action_interfaces/action.h>
#include <vclib/processing/engine/settings.h>

namespace vcl::proc {

/**
 * @brief Given a list of actions given in a TemplatedTypeWrapper, this function
 * fills the given vector with the action instances that can be instantiated for
 * all the supported mesh types in the processing module.
 *
 * @param[in/out] vec: vector to fill with the action instances
 */
template<template<typename> typename... Actions>
void fillActionsForSupportedMeshTypes(
    std::vector<std::shared_ptr<Action>>& vec,
    TemplatedTypeWrapper<Actions...>)
{
    // lambda called for each mesh type
    auto fMesh = [&]<typename MeshType>() {
        // lambda called for each action
        auto fAct = [&]<template<typename> typename Act>() {
            // if the action can be instantiated with the mesh type
            if constexpr (IsInstantiable<Act, MeshType>) {
                vec.push_back(std::make_shared<Act<MeshType>>());
            }
        };

        // call the lambda for each action
        (fAct.template operator()<Actions>(), ...);
    };

    // call the lambda for each mesh type
    vcl::ForEachType<MeshTypes>::apply(fMesh);
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INSTANCES_FILL_ACTIONS_H
