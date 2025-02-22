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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_FILTER_MESH_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_FILTER_MESH_ACTION_H

#include <vclib/processing/action_interfaces/action.h>
#include <vclib/processing/parameter_vector.h>
#include <vclib/processing/parameters.h>

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

    using CategoryBitSet = BitSet<Category>;

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
     * @brief Returns a vector indicating the number of input meshes that the
     * filter requires, and their description.
     *
     * Input meshes are the meshes that the filter will use to perform its
     * operation, but that will not be modified (they will be const, and the
     * applyFilter function will receive them in a const vector of const
     * meshes).
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
    virtual ParameterVector parameters() const
    {
        return ParameterVector();
    }

};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_FILTER_MESH_ACTION_H
