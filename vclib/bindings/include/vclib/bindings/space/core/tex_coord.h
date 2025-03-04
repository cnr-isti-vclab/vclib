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

#ifndef VCL_BINDINGS_SPACE_CORE_TEX_COORD_H
#define VCL_BINDINGS_SPACE_CORE_TEX_COORD_H

#include <vclib/bindings/utils.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<typename TexCoordType>
void populateTexCoord(pybind11::class_<TexCoordType>& c)
{
    namespace py = pybind11;

    c.def(py::init<>());

    defCopy(c);

    c.def("u", py::overload_cast<>(&TexCoordType::u, py::const_));
    c.def("v", py::overload_cast<>(&TexCoordType::v, py::const_));
    c.def("set_u", &TexCoordType::setU);
    c.def("set_v", &TexCoordType::setV);

    defComparisonOperators(c);
}

void initTexCoord(pybind11::module& m);

} // namespace vcl::bind

#endif // VCL_BINDINGS_SPACE_CORE_TEX_COORD_H
