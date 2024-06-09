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

#ifndef VCL_PROCESSING_ACTIONS_LOAD_MESH_OBJ_LOAD_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_LOAD_MESH_OBJ_LOAD_MESH_ACTION_H

#include <vclib/io/mesh/obj/load.h>
#include <vclib/processing/actions/interfaces/load_mesh_action.h>
#include <vclib/processing/actions/common/parameters.h>
#include <vclib/processing/functions.h>
#include <vclib/processing/meshes.h>

namespace vcl::proc {

class ObjLoadMeshAction : public LoadMeshAction {
public:
    using LoadMeshAction::load;

    std::string name() const override { return "Load Obj Mesh"; }

    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<ObjLoadMeshAction>(*this);
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(EnumParameter(
            "mesh_type", 0, {"Best Fit", "TriMesh", "PolyMesh"}, "", ""));

        return params;
    }

    std::vector<FileFormat> formats() const override
    {
        return {FileFormat("obj", "")};
    }

    std::shared_ptr<MeshI> load(
        const std::string&     filename,
        const ParameterVector& parameters,
        MeshInfo&              loadedInfo) const override
    {
        std::shared_ptr<MeshI> mesh;

        switch (parameters.get("mesh_type")->intValue()) {
        case 0:
        case 2:
            mesh = loadObj<PolyMesh>(filename, loadedInfo).clone();
            break;
        case 1:
            mesh = loadObj<TriMesh>(filename, loadedInfo).clone();
            break;
        default: throw std::runtime_error("Invalid mesh type");
        }

        return mesh;
    }

private:
    template<MeshConcept MeshType>
    MeshType loadObj(
        const std::string&     filename,
        MeshInfo&              loadedInfo) const
    {
        MeshType mesh = vcl::loadObj<MeshType>(filename, loadedInfo);
        postLoad(mesh, loadedInfo);
        loadMeshTextures(
            mesh, FileInfo::pathWithoutFileName(filename), manager());
        return mesh;
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_LOAD_MESH_OBJ_LOAD_MESH_ACTION_H
