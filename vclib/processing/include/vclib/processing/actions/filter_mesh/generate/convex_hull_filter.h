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

#ifndef VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H
#define VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H

#include <vclib/processing/engine.h>

#include <vclib/algorithms/mesh/convex_hull.h>

namespace vcl::proc {

template<MeshConcept MeshType>
class ConvexHullFilter : public FilterActionT<MeshType>
{
    using Base = FilterActionT<MeshType>;

public:
    std::string name() const final { return "Convex Hull"; }

    std::string description() const final
    {
        return "Generates a convex hull mesh from a set of 3D points.";
    }

    Base::CategoryBitSet categories() const final
    {
        return {Base::Category::RECONSTRUCTION};
    }

    std::vector<UintParameter> inputMeshes() const final
    {
        return {UintParameter("input", 0, "Input Mesh", "")};
    }

    std::vector<UintParameter> inputOutputMeshes() const final { return {}; }

    ParameterVector parameters() const final { return {}; }

    virtual OutputValues executeFilter(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        AbstractLogger&                     log = Base::logger()) const final
    {
        const MeshType& input = *inputMeshes.front();
        std::string name = "Convex Hull";
        if constexpr (HasName<MeshType>) {
            name = input.name() + " " + name;
        }
        outputMeshes.push_back(
            convexHull<MeshType>(input.vertices() | vcl::views::coords, log));
        if constexpr (HasName<MeshType>) {
            outputMeshes.back().name() = name;
        }
        return OutputValues();
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H
