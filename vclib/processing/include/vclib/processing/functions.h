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

#ifndef VCL_PROCESSING_FUNCTIONS_H
#define VCL_PROCESSING_FUNCTIONS_H

#include "manager.h"

#include <any>

namespace vcl::proc {

template<template<typename> typename Action, typename MeshType>
auto actionDownCast(const std::shared_ptr<vcl::proc::Action>& action)
{
    return std::dynamic_pointer_cast<Action<MeshType>>(action);
}

std::pair<std::any, MeshTypeId> loadMeshBestFit(
    const std::string&     filename,
    const ParameterVector& parameters,
    auto&                  logger)
{
    std::any    res;
    std::string ext = FileInfo::extension(filename);

    PolyEdgeMesh mesh = ActionManager::loadMeshAction<PolyEdgeMesh>(ext)->load(
        filename, parameters, logger);

    if (isTriangleMesh(mesh)) {
        TriEdgeMesh m;
        m.importFrom(mesh);
        res = std::move(m);
        return {res, MeshTypeId::TRIANGLE_MESH};
    }
    else {
        res = std::move(mesh);
        return {res, MeshTypeId::POLYGON_MESH};
    }
}

inline ParameterVector filterParameters(
    MeshTypeId         type,
    const std::string& name)
{
    auto action = ActionManager::filterAction(name, type);
    switch (type) {
    case MeshTypeId::TRIANGLE_MESH:
        return actionDownCast<FilterActionT, vcl::TriEdgeMesh>(action)
            ->parameters();
    case MeshTypeId::POLYGON_MESH:
        return actionDownCast<FilterActionT, vcl::PolyEdgeMesh>(action)
            ->parameters();
    default: return ParameterVector();
    }
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_FUNCTIONS_H
