// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
