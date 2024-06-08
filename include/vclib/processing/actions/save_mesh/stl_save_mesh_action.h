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

#ifndef VCL_PROCESSING_ACTIONS_SAVE_MESH_STL_SAVE_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_SAVE_MESH_STL_SAVE_MESH_ACTION_H

#include <vclib/io/mesh/stl/capability.h>
#include <vclib/io/mesh/stl/save.h>
#include <vclib/processing/actions/interfaces/save_mesh_action.h>
#include <vclib/processing/actions/common/parameters.h>
#include <vclib/processing/meshes.h>

namespace vcl::proc {

class StlSaveMeshAction : public SaveMeshAction {
public:
    using SaveMeshAction::save;

    std::string name() const override { return "Save Stl Mesh"; }

    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<StlSaveMeshAction>(*this);
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(BoolParameter("binary", true, "", ""));
        params.pushBack(BoolParameter("magics_mode", false, "", ""));

        return params;
    }

    FileFormat format() const override
    {
        return FileFormat("stl", "");
    }

    MeshInfo formatCapability() const override
    {
        return stlFormatCapability();
    }

    void save(
        const std::string& filename,
        const TriMeshP&    mesh,
        const MeshInfo&    info,
        const ParameterVector& parameters) const override
    {
        saveStl(filename, mesh, info, parameters);
    }

private:
    template<MeshConcept MeshType>
    void saveStl(
        const std::string& filename,
        const MeshType&    mesh,
        const MeshInfo&    info,
        const ParameterVector& parameters) const
    {
        vcl::SaveSettings settings;
        settings.info =  info;
        settings.binary = parameters.get("binary")->boolValue();
        settings.magicsMode = parameters.get("magics_mode")->boolValue();
        vcl::saveStl(mesh, filename, settings);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_SAVE_MESH_STL_SAVE_MESH_ACTION_H
