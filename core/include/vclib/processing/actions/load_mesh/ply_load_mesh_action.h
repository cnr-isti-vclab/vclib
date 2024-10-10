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

#ifndef VCL_PROCESSING_ACTIONS_LOAD_MESH_PLY_LOAD_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_LOAD_MESH_PLY_LOAD_MESH_ACTION_H

#include <vclib/load_save/ply/load.h>
#include <vclib/processing/action_interfaces/load_mesh_action.h>
#include <vclib/processing/functions.h>
#include <vclib/processing/meshes.h>
#include <vclib/processing/parameters.h>

namespace vcl::proc {

class PlyLoadMeshAction : public LoadMeshAction
{
public:
    using LoadMeshAction::load;

    std::string name() const override { return "Load Ply Mesh"; }

    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<PlyLoadMeshAction>(*this);
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(EnumParameter(
            "mesh_type",
            0,
            {"Best Fit", "TriMesh", "PolyMesh"},
            "Mesh Type",
            "Type of the Mesh on which to load the file; Best Fit will try to "
            "load the file in the most suitable mesh type."));

        return params;
    }

    std::vector<FileFormat> formats() const override
    {
        return {FileFormat("ply", "PLY Polygon File Format")};
    }

    std::shared_ptr<MeshI> load(
        const std::string&     filename,
        const ParameterVector& parameters,
        MeshInfo&              loadedInfo,
        AbstractLogger&        log = logger()) const override
    {
        std::shared_ptr<MeshI> mesh;

        switch (parameters.get("mesh_type")->uintValue()) {
        case 0: mesh = loadBestFit(filename, loadedInfo, log); break;
        case 1: mesh = loadPly<TriMesh>(filename, loadedInfo, log); break;
        case 2: mesh = loadPly<PolyMesh>(filename, loadedInfo, log); break;
        default: throw std::runtime_error("Invalid mesh type");
        }

        return mesh;
    }

private:
    template<MeshConcept MeshType>
    void postProcess(
        MeshType&          mesh,
        const std::string& filename,
        MeshInfo&          loadedInfo) const
    {
        postLoad(mesh, loadedInfo);
        loadMeshTextures(
            mesh, FileInfo::pathWithoutFileName(filename), manager());
    }

    std::shared_ptr<MeshI> loadBestFit(
        const std::string& filename,
        MeshInfo&          loadedInfo,
        AbstractLogger&    log) const
    {
        std::shared_ptr<MeshI> mesh;

        // first I load in a PolyMesh, which I know it can store all the info
        // contained in a ply
        PolyMesh pm = vcl::loadPly<PolyMesh>(filename, loadedInfo, log);

        // if the file contains triangle meshes, I convert it to a TriMesh
        if (loadedInfo.isTriangleMesh()) {
            TriMesh tm;
            tm.enableSameOptionalComponentsOf(pm);
            tm.importFrom(pm);
            postProcess(tm, filename, loadedInfo);
            mesh = std::make_shared<TriMesh>(tm);
        }
        else {
            postProcess(pm, filename, loadedInfo);
            mesh = std::make_shared<PolyMesh>(pm);
        }
        return mesh;
    }

    template<MeshConcept MeshType>
    std::shared_ptr<MeshI> loadPly(
        const std::string& filename,
        MeshInfo&          loadedInfo,
        AbstractLogger&    log) const
    {
        MeshType mesh = vcl::loadPly<MeshType>(filename, loadedInfo, log);
        postProcess(mesh, filename, loadedInfo);
        return std::make_shared<MeshType>(mesh);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_LOAD_MESH_PLY_LOAD_MESH_ACTION_H
