/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/processing/engine.h>

#include <vclib/algorithms/mesh.h>

namespace vcl::proc {

template<MeshConcept MeshType>
class CreateConeFilter : public FilterAction<MeshType>
{
    using Base = FilterAction<MeshType>;

public:
    std::shared_ptr<Action> clone() const final
    {
        return std::make_shared<CreateConeFilter>(*this);
    }

    std::string name() const final { return "Create Cone"; }

    std::string description() const final { return "Creates a cone mesh."; }

    vcl::BitSet<uint> categories() const override
    {
        return {Base::Category::CREATE};
    }

    std::vector<UintParameter> inputMeshes() const final { return {}; }

    std::vector<UintParameter> inputOutputMeshes() const final { return {}; }

    ParameterVector parameters() const final
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

    virtual OutputValues executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>&,
        std::vector<MeshType>& outputMeshes,
        const ParameterVector& parameters,
        AbstractLogger&        log = Base::logger()) const final
    {
        auto bottomRadius = parameters.get("bottom_radius")->scalarValue();
        auto topRadius    = parameters.get("top_radius")->scalarValue();
        auto height       = parameters.get("height")->scalarValue();
        auto subdivisions = parameters.get("subdivisions")->uintValue();

        auto cone = vcl::createCone<MeshType>(
            bottomRadius, topRadius, height, subdivisions);

        vcl::updatePerVertexAndFaceNormals(cone);

        cone.name() = "Cone";

        outputMeshes.push_back(std::move(cone));

        return OutputValues();
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_FILTER_MESH_CREATE_CREATE_CONE_FILTER_H
