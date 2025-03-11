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

#ifndef VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_CONVERT_ACTIONS_H
#define VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_CONVERT_ACTIONS_H

#include "fill_actions.h"

#include <vclib/processing/engine/action_interfaces/convert_action_t.h>

namespace vcl::proc {

class ConvertActions : public ConvertAction
{
    static const uint MESH_TYPE_NUMBER = toUnderlying(MeshTypeId::COUNT);

    std::array<std::shared_ptr<ConvertAction>, MESH_TYPE_NUMBER> mConvertActions;
    uint mFirstMeshType = MESH_TYPE_NUMBER;

public:
    template<template<typename> typename Act>
    void fillWithSupportedMeshTypes()
    {
        detail::fillWithSupportedMeshTypes<Act>(mConvertActions, mFirstMeshType);
    }

    std::string name() const final
    {
        checkActionHasBeenFilled();
        return mConvertActions[mFirstMeshType]->name();
    }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }

    BitSet32 supportedMeshTypes() const
    {
        checkActionHasBeenFilled();
        BitSet32 bitset;
        for (uint i = 0; i < MESH_TYPE_NUMBER; i++) {
            if (mConvertActions[i] != nullptr) {
                bitset[i] = true;
            }
        }
        return bitset;
    }

    template<MeshConcept MeshType>
    std::shared_ptr<ConvertActionT<MeshType>> action() const
    {
        checkActionForMeshType<MeshType>();
        return std::dynamic_pointer_cast<ConvertActionT<MeshType>>(
            mConvertActions[toUnderlying(meshTypeId<MeshType>())]);
    }

    template<MeshConcept MeshType>
    std::pair<MeshTypeId, std::any> convert(
        const MeshType&                  inputMesh,
        AbstractLogger&                  log = logger()) const
    {
        checkActionForMeshType<MeshType>();
        return action<MeshType>()->convert(
            inputMesh, log);
    }

private:
    void checkActionHasBeenFilled() const
    {
        if (mFirstMeshType >= MESH_TYPE_NUMBER) {
            throw std::runtime_error("The action has not been filled.");
        }
    }

    template<MeshConcept MeshType>
    void checkActionForMeshType() const
    {
        checkActionHasBeenFilled();
        checkMeshTypeId<MeshType>();
        uint id = toUnderlying(meshTypeId<MeshType>());
        if (mConvertActions[id] == nullptr) {
            throw std::runtime_error(
                "The action cannot be instantiated for the given MeshType.");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_CONVERT_ACTIONS_H
