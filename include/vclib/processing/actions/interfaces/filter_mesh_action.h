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

#ifndef VCL_PROCESSING_ACTIONS_INTERFACES_FILTER_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_INTERFACES_FILTER_MESH_ACTION_H

#include <vclib/processing/actions/common/mesh_vector.h>
#include <vclib/space/bit_set.h>

#include "mesh_action.h"

namespace vcl::proc {

class FilterMeshAction : public MeshAction
{
public:
    uint type() const final { return ActionType::FILTER_MESH_ACTION; }

    virtual uint numberInputMeshes() const = 0;

    /**
     * @brief Returns a BitSet that tells, for each mesh type, if the action
     * supports it or not for the i-th input mesh.
     *
     * By default, all mesh types are supported.
     *
     * You should override this method if your action does not support all mesh
     * types.
     *
     * @return A BitSet with the supported mesh types.
     */
    virtual vcl::BitSet<short> supportedInputMeshTypes(uint meshIndex) const
    {
        if (meshIndex >= numberInputMeshes()) {
            throw std::runtime_error("Mesh index out of bounds.");
        }
        vcl::BitSet<short> bs;
        bs.set();
        return bs;
    }

    virtual uint numberInputOutputMeshes() const = 0;

    /**
     * @brief Returns a BitSet that tells, for each mesh type, if the action
     * supports it or not for the i-th input/output mesh.
     *
     * By default, all mesh types are supported.
     *
     * You should override this method if your action does not support all mesh
     * types.
     *
     * @return A BitSet with the supported mesh types.
     */
    virtual vcl::BitSet<short> supportedInputOutputMeshTypes(
        uint meshIndex) const
    {
        if (meshIndex >= numberInputOutputMeshes()) {
            throw std::runtime_error("Mesh index out of bounds.");
        }
        vcl::BitSet<short> bs;
        bs.set();
        return bs;
    }

    virtual OutputValues applyFilter(
        const MeshVector                           inputMeshes,
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        MeshVector&                                outputMeshes,
        const ParameterVector&                     parameters) const = 0;

    OutputValues applyFilter(
        const MeshVector                           inputMeshes,
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        MeshVector&                                outputMeshes) const
    {
        return applyFilter(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters());
    }

    OutputValues applyFilter(
        MeshVector&            outputMeshes,
        const ParameterVector& parameters) const
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
            parameters);
    }

    OutputValues applyFilter(MeshVector& outputMeshes) const
    {
        return applyFilter(outputMeshes, parameters());
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_INTERFACES_FILTER_MESH_ACTION_H
