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

#ifndef VCL_PROCESSING_ACTIONS_INTERFACES_SAVE_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_INTERFACES_SAVE_MESH_ACTION_H

#include "mesh_action.h"

#include <vclib/mesh/utils/mesh_info.h>
#include <vclib/processing/actions/common/file_format.h>
#include <vclib/processing/meshes.h>
#include <vclib/processing/settings.h>

namespace vcl::proc {

class SaveMeshAction : public MeshAction {
public:
    uint type() const final { return ActionType::SAVE_MESH_ACTION; }

    uint inputMeshNumber() const final { return 1; }

    uint outputMeshNumber() const final { return 0; }

    virtual std::vector<FileFormat> formats() const = 0;
    virtual MeshInfo formatCapability() const = 0;

    virtual void save(
        const std::string&     filename,
        const MeshI&           mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters) const = 0;

    void save(const std::string& filename, const MeshI& mesh) const
    {
        save(filename, mesh, formatCapability(), parameters());
    }

    void save(
        const std::string&  filename,
        const MeshI& mesh,
        const MeshInfo&     info) const
    {
        save(filename, mesh, info, parameters());
    }

    void save(
        const std::string&     filename,
        const MeshI&    mesh,
        const ParameterVector& parameters) const
    {
        save(filename, mesh, formatCapability(), parameters);
    }

protected:
    void callFunctionForSupportedMesheTypes(
        auto&& function,
        const std::string&     filename,
        const MeshI&           mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters) const
    {
        auto supportedMeshTypes = supportedInputMeshTypes(0);
        if (! supportedMeshTypes[mesh.type()]) {
            throw std::runtime_error(
                "The action " + name() + " does not support the " +
                mesh.typeName() + " type.");
        }

        switch(mesh.type())
        {
        case MeshIType::TRI_MESH:
            function(filename, mesh.as<TriMesh>(), info, parameters);
            break;
        case MeshIType::POLY_MESH:
            function(filename, mesh.as<PolyMesh>(), info, parameters);
            break;
        default:
            throw std::runtime_error("Unknown mesh type");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_INTERFACES_SAVE_MESH_ACTION_H
