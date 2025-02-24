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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_MESH_ACTION_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_MESH_ACTION_H

#include "action.h"

#include <vclib/processing/engine/parameter_vector.h>
#include <vclib/processing/engine/parameters.h>

#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/algorithms/mesh/update.h>
#include <vclib/io/file_format.h>
#include <vclib/io/file_info.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl::proc {

template<MeshConcept MeshType>
class FilterMeshAction : public Action
{
public:
    enum class Category {
        CREATE = 0,
        CLEANING_AND_REPAIRING,
        RECONSTRUCTION,
        SMOOTHING,

        COUNT,
    };

    using CategoryBitSet = BitSet<uint>;

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    [[nodiscard]] virtual std::shared_ptr<Action> clone() const = 0;

    virtual std::string name() const = 0;

    /**
     * @brief Returns the categories of the filter.
     *
     * Returns a BitSet that contains, for each category listed in the
     * Category enum, whether the action belongs to that category.
     *
     * @return The categories of the action.
     */
    virtual CategoryBitSet categories() const = 0;

    /**
     * @brief Returns the description of the filter.
     *
     * @return The description of the filter.
     */
    virtual std::string description() const = 0;

    /**
     * @brief Returns a vector indicating the input meshes that the filter
     * requires, and their description.
     *
     * Input meshes are the meshes that the filter will use to perform its
     * operation, but that will not be modified (they will be const, and the
     * applyFilter function will receive them in a const vector of const
     * meshes).
     *
     * The UintParameter objects in the returned vector must contain the name
     * and description of the input meshes. The uint value contained in the
     * UintParameter object won't be used. The number of input mesh requires is
     * the size of the returned vector.
     *
     * @return The input meshes for the filter.
     */
    virtual std::vector<UintParameter> inputMeshes() const = 0;

    /**
     * @brief Returns a vector indicating the number of input/output meshes that
     * the filter requires, and their description.
     *
     * Input/Output meshes are the meshes that the filter will use to perform
     * its operation, and that will be modified (they will be non-const, and the
     * applyFilter function will receive them in a const vector of non-const
     * meshes).
     *
     * The UintParameter objects in the returned vector must contain the name
     * and description of the input/output meshes. The uint value contained in
     * the UintParameter object won't be used. The number of input/output mesh
     * requires is the size of the returned vector.
     *
     * @return The input/output meshes for the filter.
     */
    virtual std::vector<UintParameter> inputOutputMeshes() const = 0;

    /**
     * @brief Returns the parameters of the filter.
     *
     * By default, the filter has no parameters.
     *
     * You should override this method if your filter requires
     * parameters.
     *
     * @return The parameters for the filter.
     */
    virtual ParameterVector parameters() const { return ParameterVector(); }

protected:
    /**
     * @brief Executes the filter to the input meshes and/or input/output
     * meshes, filling eventual output meshes, and returning the output values
     * of the filter, if any.
     *
     * This method is the one that actually executes the filter, and must be
     * implemented by the derived classes.
     *
     * It won't be called directly by the user or the manager, but by the
     * execute method of the base class. You don't need to check the consistency
     * of the input vectors, as this will be done by the execute method of the
     * base class.
     *
     * @param[in] inputMeshes: The input meshes. These meshes will not be
     * modified by the filter. They will be passed to the function as a const
     * vector of const pointers to meshes, which guarantees that the meshes
     * of the vector will not be modified.
     * @param[in/out] inputOutputMeshes: The input/output meshes. These meshes
     * can be used as input and can be modified by the filter. They will be
     * passed to the function as a vector of pointers to meshes, which
     * guarantees that the vector cannot be modified, but the meshes pointed by
     * the elements of the vector can be modified.
     * @param[our] outputMeshes: The output meshes. These meshes will be filled
     * by the filter, if any. The filter will create new meshes and will add
     * them to the vector. The vector will be empty when the function
     * is called, and the filter will fill it with the output meshes.
     * @param[in] parameters: The parameters of the filter.
     * @param[in] log: The logger to use
     * @return The output values of the filter, if any.
     */
    virtual OutputValues executeFilter(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        AbstractLogger&                     log = logger()) const = 0;

public:
    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    Type type() const final { return Type::FILTER_MESH_ACTION; }

    /// execute overrides

    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        AbstractLogger&                     log = logger()) const
    {
        checkInputVectors(inputMeshes, inputOutputMeshes);
        return executeFilter(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters, log);
    }

    // without parameters override
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
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        std::vector<MeshType>&              outputMeshes,
        AbstractLogger&                     log = logger()) const
    {
        return execute(inputMeshes, outputMeshes, parameters(), log);
    }

    // without inputOutputMeshes and outputMeshes override
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
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        AbstractLogger&                     log = logger()) const
    {
        return execute(inputMeshes, parameters(), log);
    }

    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&        outputMeshes,
        const ParameterVector&        parameters,
        AbstractLogger&               log = logger()) const
    {
        return execute({}, inputOutputMeshes, outputMeshes, parameters, log);
    }

    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&        outputMeshes,
        AbstractLogger&               log = logger()) const
    {
        return execute(inputOutputMeshes, outputMeshes, parameters(), log);
    }

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

    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        AbstractLogger&               log = logger()) const
    {
        return execute(inputOutputMeshes, parameters(), log);
    }

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

    OutputValues execute(
        std::vector<MeshType>& outputMeshes,
        AbstractLogger&        log = logger()) const
    {
        return execute(outputMeshes, parameters(), log);
    }

protected:
    std::string identifier() const final
    {
        return identifierFromName(name() + " " + vcl::meshTypeName<MeshType>());
    }

private:
    void checkInputMeshes(uint provided) const
    {
        uint n = inputMeshes().size();
        if (n != provided) {
            throw std::runtime_error(
                "The action " + name() + " requires " + std::to_string(n) +
                " input meshes, but " + std::to_string(provided) +
                " was provided. Use a different execute overload.");
        }
    }

    void checkInputOutputMeshes(uint provided) const
    {
        uint n = inputOutputMeshes().size();
        if (n != provided) {
            throw std::runtime_error(
                "The action " + name() + " requires " + std::to_string(n) +
                " input/output meshes, but " + std::to_string(provided) +
                " was provided. Use a different execute overload.");
        }
    }

    void warnOutputMeshesVector(
        const std::vector<MeshType>& outputMeshes,
        AbstractLogger&              log) const
    {
        if (!outputMeshes.empty()) {
            log.log(
                "The action " + name() +
                    " returned output meshes, but an "
                    "outputMeshes vector was not provided to the execute "
                    "function.",
                log.WARNING_LOG);
        }
    }

    void checkInputVectors(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes) const
    {
        checkInputMeshes(inputMeshes.size());
        checkInputOutputMeshes(inputOutputMeshes.size());
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_MESH_ACTION_H
