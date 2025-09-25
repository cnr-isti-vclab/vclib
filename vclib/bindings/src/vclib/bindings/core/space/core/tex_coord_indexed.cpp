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

#include <vclib/bindings/core/space/core/tex_coord_indexed.h>

#include <vclib/bindings/core/space/core/tex_coord.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initTexCoordIndexed(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar = double;
    using P      = TexCoordIndexed<Scalar>;

    py::class_<P> c(m, "TexCoordIndexed");

    populateTexCoord<P>(c);

    c.def(py::init<Scalar, Scalar, ushort>(), "u"_a, "v"_a, "index"_a);

    c.def(py::init([](const py::list& v, ushort index) {
        if (v.size() != 2) {
            throw std::invalid_argument(
                "Input list must have 2 elements for type vclib.TexCoord");
        }
        P p;
        for (uint i = 0; const auto& d : v) {
            p(i++) = d.cast<double>();
        }
        p.index() = index;
        return p;
    }));

    c.def("index", py::overload_cast<>(&P::index, py::const_));
    c.def("set_index", [](P& t, ushort i) {
        t.index() = i;
    });
}

} // namespace vcl::bind
