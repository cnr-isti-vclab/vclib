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

#ifndef VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H
#define VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H

#include <vclib/algorithms/mesh/convex_hull.h>
#include <vclib/processing/action_interfaces/filter_mesh_action.h>

namespace vcl::proc {

class ConvexHullFilter : public FilterMeshAction
{
public:
    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<ConvexHullFilter>(*this);
    }

    std::string name() const final { return "Convex Hull"; }

    std::string description() const final
    {
        return "Generates a convex hull mesh from a set of 3D points.";
    }

    BitSet<uint> categories() const final
    {
        return BitSet<uint>({RECONSTRUCTION});
    }

    MeshParamVector inputMeshParameters() const final
    {
        std::pair<MeshParameter, BitSet<short>> par;
        par.first  = MeshParameter("input", "Input Mesh", "");
        par.second.set(); // all meshes are supported

        return {par};
    }

    MeshParamVector inputOutputMeshParameters() const final { return {}; }

    ParameterVector parameters() const final { return {}; }

    virtual OutputValues applyFilter(
        const MeshVector inputMeshes,
        const std::vector<std::shared_ptr<MeshI>>&,
        MeshVector& outputMeshes,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const override
    {
        BitSet<short> supportedMeshTypes = inputMeshParameters().front().second;

        const auto& inputMesh = inputMeshes.front();

        auto fun = [&](const auto& mesh) {
            return convexHull(mesh, log);
        };

        TriMesh ch = callFunctionForSupportedInputMeshTypes(
            *inputMesh, supportedMeshTypes, fun);

        outputMeshes.pushBack(std::make_shared<TriMesh>(std::move(ch)));

        return OutputValues();
    }

private:
    template <MeshConcept MeshType>
    TriMesh convexHull(const MeshType& mesh, AbstractLogger& log) const
    {
        return vcl::convexHull<TriMesh>(
            mesh.vertices() | vcl::views::coords, log);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H