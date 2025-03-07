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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_ACTION_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_ACTION_H

#include "action.h"

#include <vclib/processing/engine/parameter_vector.h>
#include <vclib/processing/engine/parameters.h>

namespace vcl::proc {

class FilterAction : public Action
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

public:
    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    Type type() const final { return Type::FILTER_ACTION; }

protected:
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

    template<MeshConcept MeshType>
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

    template<MeshConcept MeshType>
    void checkInputVectors(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes) const
    {
        checkInputMeshes(inputMeshes.size());
        checkInputOutputMeshes(inputOutputMeshes.size());
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_ACTION_H
