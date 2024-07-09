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

#ifndef VCL_PROCESSING_ACTIONS_FILTER_MESH_CREATE_CREATE_CONE_FILTER_H
#define VCL_PROCESSING_ACTIONS_FILTER_MESH_CREATE_CREATE_CONE_FILTER_H

#include <vclib/algorithms/mesh/create/cone.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/processing/actions/interfaces/filter_mesh_action.h>
#include <vclib/processing/parameters.h>

namespace vcl::proc {

class CreateConeFilter : public FilterMeshAction
{
public:
    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<CreateConeFilter>(*this);
    }

    std::string name() const override { return "Create Cone"; }

    std::string description() const override { return "Creates a cone mesh."; }

    vcl::BitSet<uint> categories() const override
    {
        return vcl::BitSet<uint>({CREATE});
    }

    MeshParamVector inputMeshParameters() const final { return {}; }

    MeshParamVector inputOutputMeshParameters() const final { return {}; }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(
            UscalarParameter("bottom_radius", 1, "Bottom Radius", ""));
        params.pushBack(UscalarParameter("top_radius", 1, "Top Radius", ""));
        params.pushBack(UscalarParameter("height", 1, "Height", ""));
        params.pushBack(
            UintParameter("subdivisions", 36, "N. Subdivisions", ""));

        return params;
    }

    virtual OutputValues applyFilter(
        const MeshVector,
        const std::vector<std::shared_ptr<MeshI>>&,
        MeshVector&            outputMeshes,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const override
    {
        auto bottomRadius = parameters.get("bottom_radius")->scalarValue();
        auto topRadius    = parameters.get("top_radius")->scalarValue();
        auto height       = parameters.get("height")->scalarValue();
        auto subdivisions = parameters.get("subdivisions")->uintValue();

        auto meshPtr = std::make_shared<TriMesh>(vcl::createCone<TriMesh>(
            bottomRadius, topRadius, height, subdivisions));

        vcl::updatePerVertexAndFaceNormals(*meshPtr);

        meshPtr->name() = "Cone";

        outputMeshes.pushBack(meshPtr);

        return OutputValues();
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_FILTER_MESH_CREATE_CREATE_CONE_FILTER_H
