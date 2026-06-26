// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
        "triangle_count",
        py::overload_cast<uint>(&TriPolyIndexBiMap::triangleCount, py::const_),
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
        "triangle_count",
        py::overload_cast<>(&TriPolyIndexBiMap::triangleCount, py::const_));
    c.def("polygon_count", &TriPolyIndexBiMap::polygonCount);
}

} // namespace vcl::bind
