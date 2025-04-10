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

#include <vclib/bindings/processing/action_manager.h>

#include <vclib/processing/action_instances.h>
#include <vclib/processing/manager.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initActionManager(pybind11::module& m)
{
    namespace py = pybind11;

    pybind11::class_<vcl::proc::ActionManager> c(m, "ActionManager");

    c.def_static(
        "load_image_formats", &vcl::proc::ActionManager::loadImageFormats);

    c.def_static(
        "save_image_formats", &vcl::proc::ActionManager::saveImageFormats);
}

} // namespace vcl::bind
