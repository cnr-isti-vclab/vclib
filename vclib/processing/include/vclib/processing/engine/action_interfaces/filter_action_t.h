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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_ACTION_T_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_ACTION_T_H

#include "filter_action.h"

#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/algorithms/mesh/update.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl::proc {

template<MeshConcept Mesh>
class FilterActionT : public FilterAction
{
public:
    using MeshType = Mesh;

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    // From FilterAction class

    virtual CategoryBitSet categories() const = 0;

    virtual std::string description() const = 0;

    virtual std::vector<UintParameter> inputMeshes() const = 0;

    virtual std::vector<UintParameter> inputOutputMeshes() const = 0;

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

    MeshTypeId meshType() const final { return meshTypeId<MeshType>(); }

    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        AbstractLogger&                     log = logger()) const
    {
        checkInputVectors(inputMeshes, inputOutputMeshes);
        auto res = executeFilter(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters, log);
        for (MeshType* m : inputOutputMeshes) {
            postExecute(*m);
        }
        for (MeshType& m : outputMeshes) {
            postExecute(m);
        }
        return res;
    }

protected:
    void postExecute(MeshType& mesh) const
    {
        if constexpr (HasFaces<MeshType>) {
            vcl::updatePerVertexAndFaceNormals(mesh);
        }
        vcl::updateBoundingBox(mesh);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_FILTER_ACTION_T_H
