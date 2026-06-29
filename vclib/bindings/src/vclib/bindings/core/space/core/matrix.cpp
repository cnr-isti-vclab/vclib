// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/matrix.h>

#include <vclib/bindings/utils.h>
#include <vclib/space/core.h>

#include <pybind11/operators.h>

namespace vcl::bind {

template<uint R, uint C>
void populteMatrix(pybind11::module& m)
{
    namespace py = pybind11;

    using Scalar = double;
    using M      = Matrix<Scalar, R, C>;

    std::string   cName = "Matrix" + std::to_string(R) + std::to_string(C);
    py::class_<M> c(m, cName.c_str(), py::buffer_protocol());
    c.def(py::init<>());
    // allow to initialize Matrix with py::buffer
    c.def(py::init([](const py::buffer& b) {
        return pyBufferToEigen<R, C>(b);
    }));
    py::implicitly_convertible<py::buffer, M>();

    defCopy(c);

    c.def_buffer([](M& p) -> py::buffer_info {
        return py::buffer_info(
            p.data(),                                /* Pointer to buffer */
            sizeof(Scalar),                          /* Size of one scalar */
            py::format_descriptor<Scalar>::format(), /* Python struct-style
               format descriptor */
            2,                                       /* Number of dimensions */
            {R, C},                                  /* Buffer dimensions */
            {sizeof(Scalar), sizeof(Scalar) * R}
            /* Strides (in bytes) for each index */
        );
    });

    c.def("__call__", [](M& p, uint i, uint j) { // operator()
        return p(i, j);
    });

    c.def("__getitem__", [](M& p, std::pair<uint, uint> i) { // operator[]
        return p(i.first, i.second);
    });

    c.def(
        "__setitem__",
        [](M& p, std::pair<uint, uint> i, Scalar v) { // operator[]
            p(i.first, i.second) = v;
        });
}

void initMatrix(pybind11::module& m)
{
    populteMatrix<2, 2>(m);
    populteMatrix<3, 3>(m);
    populteMatrix<4, 4>(m);
}

} // namespace vcl::bind
