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

#ifndef VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_FILTER_ACTIONS_H
#define VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_FILTER_ACTIONS_H

#include "fill_actions.h"

#include <vclib/processing/engine/action_interfaces/filter_action_t.h>

namespace vcl::proc {

class FilterActions : public FilterAction
{
    static const uint MESH_TYPE_NUMBER = toUnderlying(MeshTypeId::COUNT);

    std::array<std::shared_ptr<FilterAction>, MESH_TYPE_NUMBER> mFilterActions;
    uint mFirstMeshType = MESH_TYPE_NUMBER;

public:
    template<template<typename> typename Act>
    void fillWithSupportedMeshTypes()
    {
        detail::fillWithSupportedMeshTypes<Act>(mFilterActions, mFirstMeshType);
    }

    std::string name() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->name();
    }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }

    CategoryBitSet categories() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->categories();
    }

    std::string description() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->description();
    }

    std::vector<UintParameter> inputMeshes() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->inputMeshes();
    }

    std::vector<UintParameter> inputOutputMeshes() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->inputOutputMeshes();
    }

    ParameterVector parameters() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->parameters();
    }

    BitSet32 supportedMeshTypes() const
    {
        checkActionHasBeenFilled();
        BitSet32 bitset;
        for (uint i = 0; i < MESH_TYPE_NUMBER; i++) {
            if (mFilterActions[i] != nullptr) {
                bitset[i] = true;
            }
        }
        return bitset;
    }

    template<MeshConcept MeshType>
    std::shared_ptr<FilterActionT<MeshType>> action() const
    {
        checkActionForMeshType<MeshType>();
        return std::dynamic_pointer_cast<FilterActionT<MeshType>>(
            mFilterActions[toUnderlying(meshTypeId<MeshType>())]);
    }

    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        AbstractLogger&                     log = logger()) const
    {
        checkActionForMeshType<MeshType>();
        return action<MeshType>()->execute(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters, log);
    }

    // without parameters override
    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        AbstractLogger&                     log = logger()) const
    {
        return execute(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters(), log);
    }

    // without inputOutputMeshes override
    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        AbstractLogger&                     log = logger()) const
    {
        checkInputOutputMeshes(0);
        return execute(inputMeshes, {}, outputMeshes, parameters, log);
    }

    // without inputOutputMeshes and parameters override
    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        std::vector<MeshType>&              outputMeshes,
        AbstractLogger&                     log = logger()) const
    {
        return execute(inputMeshes, outputMeshes, parameters(), log);
    }

    // without inputOutputMeshes and outputMeshes override
    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const ParameterVector&              parameters,
        AbstractLogger&                     log = logger()) const
    {
        std::vector<MeshType> outputMeshes;
        auto out = execute(inputMeshes, outputMeshes, parameters, log);
        warnOutputMeshesVector(outputMeshes, log);
        return out;
    }

    // without inputOutputMeshes, outputMeshes and parameters override
    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        AbstractLogger&                     log = logger()) const
    {
        return execute(inputMeshes, parameters(), log);
    }

    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&        outputMeshes,
        const ParameterVector&        parameters,
        AbstractLogger&               log = logger()) const
    {
        return execute({}, inputOutputMeshes, outputMeshes, parameters, log);
    }

    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&        outputMeshes,
        AbstractLogger&               log = logger()) const
    {
        return execute(inputOutputMeshes, outputMeshes, parameters(), log);
    }

    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        const ParameterVector&        parameters,
        AbstractLogger&               log = logger()) const
    {
        std::vector<MeshType> outputMeshes;
        auto out = execute(inputOutputMeshes, outputMeshes, parameters, log);
        warnOutputMeshesVector(outputMeshes, log);
        return out;
    }

    template<MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        AbstractLogger&               log = logger()) const
    {
        return execute(inputOutputMeshes, parameters(), log);
    }

    template<MeshConcept MeshType>
    OutputValues execute(
        std::vector<MeshType>& outputMeshes,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        return execute(
            std::vector<const MeshType*>(),
            std::vector<MeshType*>(),
            outputMeshes,
            parameters,
            log);
    }

    template<MeshConcept MeshType>
    OutputValues execute(
        std::vector<MeshType>& outputMeshes,
        AbstractLogger&        log = logger()) const
    {
        return execute(outputMeshes, parameters(), log);
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
        if (mFilterActions[id] == nullptr) {
            throw std::runtime_error(
                "The action cannot be instantiated for the given MeshType.");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_FILTER_ACTIONS_H
