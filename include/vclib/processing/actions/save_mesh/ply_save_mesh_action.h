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

#ifndef VCL_PROCESSING_ACTIONS_SAVE_MESH_PLY_SAVE_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_SAVE_MESH_PLY_SAVE_MESH_ACTION_H

#include <vclib/io/mesh/ply/capability.h>
#include <vclib/io/mesh/ply/save.h>
#include <vclib/processing/actions/common/parameters.h>
#include <vclib/processing/actions/interfaces/save_mesh_action.h>
#include <vclib/processing/functions.h>
#include <vclib/processing/meshes.h>

namespace vcl::proc {

class PlySaveMeshAction : public SaveMeshAction
{
public:
    using SaveMeshAction::save;

    std::string name() const override { return "Save Ply Mesh"; }

    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<PlySaveMeshAction>(*this);
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(BoolParameter(
            "binary",
            true,
            "Binary",
            "If true, the file will be saved as binary."));
        params.pushBack(BoolParameter(
            "save_texture_files",
            true,
            "Save Texture Files",
            "If true, the texture image files will be saved along the mesh "
            "file. Otherwise, only the mesh will be saved."));

        return params;
    }

    std::vector<FileFormat> formats() const override
    {
        return {FileFormat("ply", "PLY Polygon File Format")};
    }

    MeshInfo formatCapability() const override { return plyFormatCapability(); }

    void save(
        const std::string&     filename,
        const MeshI&           mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters) const override
    {
        // transform savePly to a lambda function
        auto fun = [&](auto&& m, auto&& fn, auto&& i, auto&& p) {
            savePly(m, fn, i, p);
        };

        callFunctionForSupportedMesheTypes(
            fun, mesh, filename, info, parameters);
    }

private:
    void savePly(
        const MeshConcept auto& mesh,
        const std::string&      filename,
        const MeshInfo&         info,
        const ParameterVector&  parameters) const
    {
        vcl::SaveSettings settings;
        settings.info   = info;
        settings.binary = parameters.get("binary")->boolValue();
        vcl::savePly(mesh, filename, settings);

        if (parameters.get("save_texture_files")->boolValue()) {
            if (manager()) {
                saveMeshTextures(
                    mesh, FileInfo::pathWithoutFileName(filename), manager());
            }
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_SAVE_MESH_PLY_SAVE_MESH_ACTION_H
