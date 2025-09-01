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

#include <vclib/bindings/core/space/core/point.h>

#include <vclib/bindings/utils.h>
#include <vclib/space/core.h>

#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace vcl::bind {

template<uint DIM>
void populatePoint(pybind11::module& m)
{
    namespace py = pybind11;

    using Scalar = double;
    using P      = Point<Scalar, DIM>;

    std::string   cName = "Point" + std::to_string(DIM);
    py::class_<P> c(m, cName.c_str(), py::buffer_protocol());
    c.def(py::init<>());
    c.def(py::init([](const py::list& v) {
        if (v.size() != DIM) {
            throw std::invalid_argument(
                "Input list must have " + std::to_string(DIM) +
                " elements for type vclib.Point" + std::to_string(DIM));
        }
        P p;
        for (uint i = 0; const auto& d : v) {
            p(i++) = d.cast<double>();
        }
        return p;
    }));
    py::implicitly_convertible<py::list, P>();
    // allow to initialize Point with py::buffer
    c.def(py::init([](const py::buffer& b) {
        return pyBufferToEigen<1, DIM>(b);
    }));
    py::implicitly_convertible<py::buffer, P>();

    defCopy(c);

    c.def_property_readonly_static("DIM", [](py::object /* self */) {
        return P::DIM;
    });

    c.def_buffer([](P& p) -> py::buffer_info {
        return py::buffer_info(
            p.data(),                                /* Pointer to buffer */
            sizeof(Scalar),                          /* Size of one scalar */
            py::format_descriptor<Scalar>::format(), /* Python struct-style
                                                        format descriptor */
            1,                                       /* Number of dimensions */
            {DIM},                                   /* Buffer dimensions */
            {sizeof(Scalar)} /* Strides (in bytes) for each index */
        );
    });

    if constexpr (DIM == 1) {
        c.def(py::init<Scalar>(), py::arg("x"));
    }
    if constexpr (DIM == 2) {
        c.def(py::init<Scalar, Scalar>(), py::arg("x"), py::arg("y"));
    }
    if constexpr (DIM == 3) {
        c.def(
            py::init<Scalar, Scalar, Scalar>(),
            py::arg("x"),
            py::arg("y"),
            py::arg("z"));

        c.def("cross", [](const P& p1, const P& p2) {
            return p1.cross(p2);
        });
        c.def("ortho_base", &P::orthoBase);
    }
    if constexpr (DIM == 4) {
        c.def(
            py::init<Scalar, Scalar, Scalar, Scalar>(),
            py::arg("x"),
            py::arg("y"),
            py::arg("z"),
            py::arg("w"));
    }

    if constexpr (DIM >= 1) {
        c.def("x", py::overload_cast<>(&P::x, py::const_));
        c.def("set_x", [](P& p, Scalar v) {
            p.x() = v;
        });
    }
    if constexpr (DIM >= 2) {
        c.def("y", py::overload_cast<>(&P::y, py::const_));
        c.def("set_y", [](P& p, Scalar v) {
            p.y() = v;
        });
    }
    if constexpr (DIM >= 3) {
        c.def("z", py::overload_cast<>(&P::z, py::const_));
        c.def("set_z", [](P& p, Scalar v) {
            p.z() = v;
        });
    }
    if constexpr (DIM >= 4) {
        c.def("w", py::overload_cast<>(&P::w, py::const_));
        c.def("set_w", [](P& p, Scalar v) {
            p.w() = v;
        });
    }

    c.def("is_degenerate", &P::isDegenerate);
    c.def("epsilon_equals", &P::epsilonEquals);
    c.def("angle", &P::angle);
    c.def("dist", &P::dist);
    c.def("squared_dist", &P::squaredDist);
    c.def("mul", &P::mul);
    c.def("div", &P::div);
    c.def("size", &P::size);
    c.def("outer_product", &P::outerProduct);

    c.def("dot", [](const P& p1, const P& p2) {
        return p1.dot(p2);
    });
    c.def("norm", &P::norm);

    // operators
    c.def("__call__", [](P& p, uint i) { // operator()
        return p(i);
    });

    c.def("__getitem__", [](P& p, uint i) { // operator[]
        return p(i);
    });

    c.def("__setitem__", [](P& p, uint i, Scalar v) { // operator[]
        p(i) = v;
    });

    defArithmeticOperators(c);

    defComparisonOperators(c);

    defRepr(c);

    // outside class functions

    m.def(
        "epsilon_equals",
        [](const P&      p1,
           const P&      p2,
           const Scalar& epsilon = std::numeric_limits<Scalar>::epsilon()) {
            return vcl::epsilonEquals(p1, p2, epsilon);
        },
        py::arg("p1"),
        py::arg("p2"),
        py::arg("epsilon") = std::numeric_limits<Scalar>::epsilon());

    m.def(
        "min",
        [](const P& p1, const P& p2) {
            return vcl::min(p1, p2);
        },
        py::arg("p1"),
        py::arg("p2"));

    m.def(
        "max",
        [](const P& p1, const P& p2) {
            return vcl::max(p1, p2);
        },
        py::arg("p1"),
        py::arg("p2"));
}

void initPoint(pybind11::module& m)
{
    populatePoint<2>(m);
    populatePoint<3>(m);
    populatePoint<4>(m);
}

} // namespace vcl::bind
