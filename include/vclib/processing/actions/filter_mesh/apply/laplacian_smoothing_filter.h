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

#ifndef VCL_PROCESSING_ACTIONS_FILTER_MESH_APPLY_LAPLACIAN_SMOOTHING_FILTER_H
#define VCL_PROCESSING_ACTIONS_FILTER_MESH_APPLY_LAPLACIAN_SMOOTHING_FILTER_H

#include <vclib/algorithms/mesh/smooth.h>

#include <vclib/processing/action_interfaces/filter_mesh_action.h>
#include <vclib/processing/parameters.h>

namespace vcl::proc {

class LaplacianSmoothingFilter : public FilterMeshAction
{
public:
    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<LaplacianSmoothingFilter>(*this);
    }

    std::string name() const override { return "Laplacian Smoothing"; }

    std::string description() const override
    {
        return "Laplacian smooth. Average each vertex position with weighted "
               "positions of neighbour vertices.<br><b>"
               "Laplacian Mesh Processing</b> by <i>Olga Sorkine</i>. "
               "EUROGRAPHICS 2005<br>"
               "<a href='http://dx.doi.org/10.2312/egst.20051044'>doi:10.2312/"
               "egst.20051044</a>";
    }

    vcl::BitSet<uint> categories() const override
    {
        return vcl::BitSet<uint>({SMOOTHING});
    }

    MeshParamVector inputMeshParameters() const final { return {}; }

    MeshParamVector inputOutputMeshParameters() const final
    {
        std::pair<MeshParameter, BitSet<short>> par;
        par.first  = MeshParameter("input_output", "Input/Output Mesh", "");
        par.second = BitSet<short>({MeshIType::TRI_MESH, MeshIType::POLY_MESH});

        return {par};
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(UintParameter(
            "smoothing_steps",
            3,
            "Smoothing steps",
            "The number of times that the whole algorithm (normal smoothing + "
            "vertex fitting) is "
            "iterated."));
        params.pushBack(BoolParameter(
            "cotangent_weighting",
            true,
            "Cotangent weighting",
            "Use cotangent weighting scheme for the averaging of the position. "
            "Otherwise the simpler umbrella scheme (1 if the edge is present) "
            "is used."));
        params.pushBack(BoolParameter(
            "only_selected",
            false,
            "Only selected",
            "If checked, the filter is performed only on the selected area."));

        return params;
    }

    virtual OutputValues applyFilter(
        const MeshVector,
        const std::vector<std::shared_ptr<MeshI>>& inputOutputMeshes,
        MeshVector&,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const override
    {
        BitSet<short> supportedMeshTypes =
            inputOutputMeshParameters().front().second;

        uint smoothingSteps = parameters.get("smoothing_steps")->uintValue();
        bool cotangentWeighting =
            parameters.get("cotangent_weighting")->boolValue();
        bool onlySelected = parameters.get("only_selected")->boolValue();

        std::shared_ptr<MeshI> mesh = inputOutputMeshes[0];

        auto fun = [&](auto& mesh) {
            laplacianSmooth(
                mesh, smoothingSteps, cotangentWeighting, onlySelected);
        };

        callFunctionForSupportedInputOutputMeshTypes(
            *mesh,
            supportedMeshTypes,
            fun);

        return OutputValues();
    }

private:
    void laplacianSmooth(
        MeshConcept auto& mesh,
        uint              smoothingSteps,
        bool              cotangentWeighting,
        bool              onlySelected) const
    {
        vcl::laplacianSmoothing(
            mesh, smoothingSteps, onlySelected, cotangentWeighting);

        updateBoxAndNormals(mesh);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_FILTER_MESH_APPLY_LAPLACIAN_SMOOTHING_FILTER_H
