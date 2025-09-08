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

#include <vclib/bindings/core/space/complex/tri_poly_index_bimap.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/complex.h>

namespace vcl::bind {

void initTriPolyIndexBimap(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    py::class_<TriPolyIndexBiMap> c(m, "TriPolyIndexBiMap");
    c.def(py::init<>());

    c.def("polygon", &TriPolyIndexBiMap::polygon, "triangle_index"_a);
    c.def(
        "triangle_begin", &TriPolyIndexBiMap::triangleBegin, "polygon_index"_a);
    c.def(
        "triangle_number",
        py::overload_cast<uint>(&TriPolyIndexBiMap::triangleNumber, py::const_),
        "polygon_index"_a);
    c.def("clear", &TriPolyIndexBiMap::clear);
    c.def(
        "reserve",
        &TriPolyIndexBiMap::reserve,
        "n_triangles"_a,
        "n_polygons"_a);
    c.def(
        "insert",
        &TriPolyIndexBiMap::insert,
        "triangle_index"_a,
        "polygon_index"_a);

    c.def(
        "triangle_number",
        py::overload_cast<>(&TriPolyIndexBiMap::triangleNumber, py::const_));
    c.def("polygon_number", &TriPolyIndexBiMap::polygonNumber);
}

} // namespace vcl::bind
