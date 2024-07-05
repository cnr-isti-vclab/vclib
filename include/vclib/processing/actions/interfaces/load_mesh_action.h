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

#ifndef VCL_PROCESSING_ACTIONS_INTERFACES_LOAD_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_INTERFACES_LOAD_MESH_ACTION_H

#include "mesh_action.h"

#include <vclib/algorithms/mesh/update.h>
#include <vclib/mesh_aux/mesh_info.h>
#include <vclib/processing/actions/common/file_format.h>
#include <vclib/processing/meshes.h>
#include <vclib/processing/settings.h>

namespace vcl::proc {

class LoadMeshAction : public MeshAction
{
public:
    uint type() const final { return ActionType::LOAD_MESH_ACTION; }

    virtual std::vector<FileFormat> formats() const = 0;

    virtual std::shared_ptr<MeshI> load(
        const std::string&     filename,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        AbstractLogger&        log = logger()) const = 0;

    virtual std::shared_ptr<MeshI> load(
        const std::string&     filename,
        const ParameterVector& parameter,
        AbstractLogger&        log = logger()) const
    {
        MeshInfo info;
        auto     mesh = load(filename, parameter, info, log);
        return mesh;
    }

    std::shared_ptr<MeshI> load(
        const std::string& filename,
        AbstractLogger&    log = logger()) const
    {
        return load(filename, parameters(), log);
    }

protected:
    template<MeshConcept MeshType>
    void postLoad(MeshType& mesh, const MeshInfo& loadedInfo) const
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            if (!loadedInfo.hasFaceNormals()) {
                vcl::updatePerFaceNormals(mesh);
            }
            if (!loadedInfo.hasVertexNormals()) {
                vcl::updatePerVertexNormalsFromFaceNormals(mesh);
            }
        }
        vcl::updateBoundingBox(mesh);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_INTERFACES_LOAD_MESH_ACTION_H
