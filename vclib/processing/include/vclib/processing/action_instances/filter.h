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

#ifndef VCL_PROCESSING_ACTION_INSTANCES_FILTER_MESH_H
#define VCL_PROCESSING_ACTION_INSTANCES_FILTER_MESH_H

#include "fill_actions.h"

#include <vclib/processing/actions/filter_mesh.h>

#include <memory>
#include <vector>

namespace vcl::proc {

namespace detail {

inline std::vector<std::shared_ptr<Action>> applyFilterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = TemplatedTypeWrapper<LaplacianSmoothingFilter>;

    fillAggregatedActions<FilterActions>(vec, Actions());

    return vec;
}

inline std::vector<std::shared_ptr<Action>> createFilterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = TemplatedTypeWrapper<CreateConeFilter>;

    fillAggregatedActions<FilterActions>(vec, Actions());

    return vec;
}

inline std::vector<std::shared_ptr<Action>> generateFilterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = TemplatedTypeWrapper<ConvexHullFilter>;

    fillAggregatedActions<FilterActions>(vec, Actions());

    return vec;
}

} // namespace detail

inline std::vector<std::shared_ptr<Action>> filterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    auto a = detail::applyFilterActions();
    vec.insert(vec.begin(), a.begin(), a.end());

    auto c = detail::createFilterActions();
    vec.insert(vec.begin(), c.begin(), c.end());

    auto g = detail::generateFilterActions();
    vec.insert(vec.begin(), g.begin(), g.end());

    return vec;
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INSTANCES_FILTER_MESH_H
