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
#include <vclib/processing/meshes.h>

namespace vcl {

class StlSaveMeshAction : public SaveMeshAction {
public:
    std::string name() const override { return "StlSaveMeshAction"; }

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
        const MeshInfo&    info) const override
    {
        saveStl(filename, mesh, info);
    }

private:
    template<MeshConcept MeshType>
    void saveStl(
        const std::string& filename,
        const MeshType&    mesh,
        const MeshInfo&    info) const
    {
        vcl::SaveSettings settings;
        settings.info =  info;
        vcl::saveStl(mesh, filename, settings);
    }
};

} // namespace vcl

#endif // VCL_PROCESSING_ACTIONS_SAVE_MESH_STL_SAVE_MESH_ACTION_H
