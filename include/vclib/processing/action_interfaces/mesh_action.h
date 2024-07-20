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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_MESH_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_MESH_ACTION_H

#include <vclib/processing/meshes.h>
#include <vclib/processing/parameter_vector.h>

#include "action.h"

namespace vcl::proc {

class MeshAction : public Action
{
public:
    MeshAction()          = default;
    virtual ~MeshAction() = default;

    /**
     * @brief Returns the parameters of the action.
     *
     * By default, the action has no parameters.
     *
     * You should override this method if your action has parameters.
     *
     * @return The parameters of the action.
     */
    virtual ParameterVector parameters() const { return ParameterVector(); }

protected:
    /**
     * @brief Calls a function for the mesh, downcasting it to the correct type.
     *
     * The function is supposed to have the following signature:
     * `auto function(const MeshType& mesh, auto&&... args) const`.
     *
     * @param function
     * @param mesh
     * @param args
     */
    auto callFunctionForMesh(const MeshI& mesh, auto&& function)
        const
    {
        switch (mesh.type()) {
        case MeshIType::TRI_MESH: return function(mesh.as<TriMesh>()); break;
        case MeshIType::POLY_MESH: return function(mesh.as<PolyMesh>()); break;
        default: throw std::runtime_error("Unknown mesh type");
        }
    }

    auto callFunctionForMesh(MeshI& mesh, auto&& function) const
    {
        switch (mesh.type()) {
        case MeshIType::TRI_MESH: return function(mesh.as<TriMesh>()); break;
        case MeshIType::POLY_MESH: return function(mesh.as<PolyMesh>()); break;
        default: throw std::runtime_error("Unknown mesh type");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_MESH_ACTION_H
