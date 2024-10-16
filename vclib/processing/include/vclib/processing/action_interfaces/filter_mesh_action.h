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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_FILTER_MESH_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_FILTER_MESH_ACTION_H

#include <vclib/algorithms/mesh/update.h>
#include <vclib/processing/mesh_vector.h>
#include <vclib/processing/parameters/mesh_parameter.h>
#include <vclib/space/core/bit_set.h>

#include "mesh_action.h"

namespace vcl::proc {

class FilterMeshAction : public MeshAction
{
public:
    using MeshParamVector =
        std::vector<std::pair<MeshParameter, BitSet<short>>>;

    enum MeshActionCategory {
        CREATE = 0,
        CLEANING_AND_REPAIRING,
        RECONSTRUCTION,
        SMOOTHING,

        N_CATEGORIES,
    };

    // pure virtual member functions - must be implemented in derived classes

    /**
     * @brief Returns the categories of the action.
     *
     * Returns a BitSet that contains, for each category listed in the
     * MeshActionCategory enum, whether the action belongs to that category.
     *
     * @return The categories of the action.
     */
    virtual BitSet<uint> categories() const = 0;

    /**
     * @brief Returns the description of the action.
     *
     * @return The description of the action.
     */
    virtual std::string description() const = 0;

    /**
     * @brief Returns a vector that describes the input meshes required by the
     * action.
     *
     * The vector contains pairs of MeshParameter and BitSet<short>. The
     * MeshParameter describes the input mesh, while the BitSet<short> tells
     * which mesh types are supported for that input mesh.
     *
     * @return
     */
    virtual MeshParamVector inputMeshParameters() const = 0;

    /**
     * @brief Returns a vector that describes the input/output meshes required
     * by the action.
     *
     * The vector contains pairs of MeshParameter and BitSet<short>. The
     * MeshParameter describes the input/output mesh, while the BitSet<short>
     * tells which mesh types are supported for that input/output mesh.
     *
     * @return
     */
    virtual MeshParamVector inputOutputMeshParameters() const = 0;

    virtual OutputValues applyFilter(
        const MeshVector                           inputMeshes,
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        MeshVector&                                outputMeshes,
        const ParameterVector&                     parameters,
        AbstractLogger&                            log = logger()) const = 0;

    // member functions - provided for convenience - they mostly use the pure
    // virtual member functions

    uint type() const final { return ActionType::FILTER_MESH_ACTION; }

    uint numberInputMeshes() const { return inputMeshParameters().size(); }

    /**
     * @brief Returns a BitSet that tells, for each mesh type, if the action
     * supports it or not for the i-th input mesh.
     *
     * @return A BitSet with the supported mesh types.
     */
    vcl::BitSet<short> supportedInputMeshTypes(uint meshIndex) const
    {
        if (meshIndex >= numberInputMeshes()) {
            throw std::runtime_error("Mesh index out of bounds.");
        }
        return inputMeshParameters()[meshIndex].second;
    }

    uint numberInputOutputMeshes() const
    {
        return inputOutputMeshParameters().size();
    }

    /**
     * @brief Returns a BitSet that tells, for each mesh type, if the action
     * supports it or not for the i-th input/output mesh.
     *
     * @return A BitSet with the supported mesh types.
     */
    vcl::BitSet<short> supportedInputOutputMeshTypes(uint meshIndex) const
    {
        if (meshIndex >= numberInputOutputMeshes()) {
            throw std::runtime_error("Mesh index out of bounds.");
        }
        return inputOutputMeshParameters()[meshIndex].second;
    }

    OutputValues applyFilter(
        const MeshVector                           inputMeshes,
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        MeshVector&                                outputMeshes,
        AbstractLogger&                            log = logger()) const
    {
        return applyFilter(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters(), log);
    }

    OutputValues applyFilter(
        const MeshVector inputMeshes,
        MeshVector&      outputMeshes,
        AbstractLogger&  log = logger()) const
    {
        std::vector<std::shared_ptr<MeshI>> ioMeshes;

        return applyFilter(
            inputMeshes, ioMeshes, outputMeshes, parameters(), log);
    }

    OutputValues applyFilter(
        MeshVector&            outputMeshes,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        if (numberInputMeshes() > 0) {
            throw std::runtime_error(
                "This action requires input meshes. You called the wrong "
                "overload of the applyFilter member function.");
        }

        if (numberInputOutputMeshes() > 0) {
            throw std::runtime_error(
                "This action requires input/output meshes. You called the "
                "wrong overload of the applyFilter member function.");
        }

        return applyFilter(
            MeshVector(),
            std::vector<std::shared_ptr<MeshI>>(),
            outputMeshes,
            parameters,
            log);
    }

    OutputValues applyFilter(MeshVector& outputMeshes) const
    {
        return applyFilter(outputMeshes, parameters());
    }

    OutputValues applyFilter(
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        MeshVector&                                outputMeshes,
        const ParameterVector&                     parameters,
        AbstractLogger&                            log = logger())
    {
        if (numberInputMeshes() > 0) {
            throw std::runtime_error(
                "This action requires input meshes. You called the wrong "
                "overload of the applyFilter member function.");
        }

        if (numberInputOutputMeshes() == 0) {
            throw std::runtime_error(
                "This action does not require input/output meshes. You called "
                "the wrong overload of the applyFilter member function.");
        }

        return applyFilter(
            MeshVector(), inputOutputMeshes, outputMeshes, parameters, log);
    }

    OutputValues applyFilter(
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        MeshVector&                                outputMeshes,
        AbstractLogger&                            log = logger())
    {
        return applyFilter(inputOutputMeshes, outputMeshes, parameters(), log);
    }

    OutputValues applyFilter(
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        const ParameterVector&                     parameters,
        AbstractLogger&                            log = logger())
    {
        MeshVector outputMeshes;

        return applyFilter(inputOutputMeshes, outputMeshes, parameters, log);
    }

    OutputValues applyFilter(
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        AbstractLogger&                            log = logger())
    {
        return applyFilter(inputOutputMeshes, parameters(), log);
    }

protected:
    auto callFunctionForSupportedInputMeshTypes(
        const MeshI&         mesh,
        const BitSet<short>& supportedMeshTypes,
        auto&&               function) const
    {
        if (!supportedMeshTypes[mesh.type()]) {
            throw std::runtime_error(
                "The action " + name() + " does not support the " +
                mesh.typeName() + " type.");
        }

        return callFunctionForMesh(mesh, function);
    }

    auto callFunctionForSupportedInputOutputMeshTypes(
        MeshI&               mesh,
        const BitSet<short>& supportedMeshTypes,
        auto&&               function) const
    {
        if (!supportedMeshTypes[mesh.type()]) {
            throw std::runtime_error(
                "The action " + name() + " does not support the " +
                mesh.typeName() + " type.");
        }

        return callFunctionForMesh(mesh, function);
    }

    template<MeshConcept MeshType>
    void updateBoxAndNormals(MeshType& mesh) const
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            vcl::updatePerVertexAndFaceNormals(mesh);
        }
        vcl::updateBoundingBox(mesh);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_FILTER_MESH_ACTION_H
