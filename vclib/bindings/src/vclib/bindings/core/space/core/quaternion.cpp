// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/quaternion.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

#include <pybind11/operators.h>

namespace vcl::bind {

void initQuaternion(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar = double;
    using Q      = Quaternion<Scalar>;

    py::class_<Q> c(m, "Quaternion");
    c.def(py::init<>());
    c.def(
        py::init<Scalar, Scalar, Scalar, Scalar>(),
        "w"_a,
        "x"_a,
        "y"_a,
        "z"_a);
    c.def(py::init<Scalar, const Point3d&>(), "angle"_a, "axis"_a);
    c.def(py::init<const Matrix33d&>(), "rot_matrix"_a);
    c.def(py::init<const Matrix44d&>(), "rot_matrix"_a);
    c.def(py::init<const Point3d&, const Point3d&>(), "a"_a, "b"_a);

    defCopy(c);

    c.def("size", &Q::size);
    c.def("__len__", &Q::size);

    c.def("w", [](const Q& q) { return q.w(); });
    c.def("set_w", [](Q& q, Scalar v) { q.w() = v; }, "w"_a);
    c.def("x", [](const Q& q) { return q.x(); });
    c.def("set_x", [](Q& q, Scalar v) { q.x() = v; }, "x"_a);
    c.def("y", [](const Q& q) { return q.y(); });
    c.def("set_y", [](Q& q, Scalar v) { q.y() = v; }, "y"_a);
    c.def("z", [](const Q& q) { return q.z(); });
    c.def("set_z", [](Q& q, Scalar v) { q.z() = v; }, "z"_a);

    c.def("set", &Q::set, "w"_a, "x"_a, "y"_a, "z"_a);
    c.def("set_identity", [](Q& q) { q.setIdentity(); });
    c.def("set_from_angle_axis", &Q::setFromAngleAxis, "angle"_a, "axis"_a);
    c.def(
        "set_from_two_vectors",
        [](Q& q, const Point3d& a, const Point3d& b) {
            q.setFromTwoVectors(a, b);
        },
        "a"_a,
        "b"_a);

    c.def("norm", [](const Q& q) { return q.norm(); });
    c.def("squared_norm", [](const Q& q) { return q.squaredNorm(); });
    c.def("normalize", [](Q& q) { q.normalize(); });
    c.def("normalized", [](const Q& q) { return Q(q.normalized()); });
    c.def("conjugate", [](const Q& q) { return Q(q.conjugate()); });
    c.def("inverse", [](const Q& q) { return Q(q.inverse()); });
    c.def(
        "dot", [](const Q& q1, const Q& q2) { return q1.dot(q2); }, "other"_a);
    c.def(
        "angular_distance",
        [](const Q& q1, const Q& q2) { return q1.angularDistance(q2); },
        "other"_a);
    c.def(
        "slerp",
        [](const Q& q1, Scalar t, const Q& q2) { return Q(q1.slerp(t, q2)); },
        "t"_a,
        "other"_a);
    c.def("to_rotation_matrix", [](const Q& q) {
        return Matrix33d(q.toRotationMatrix());
    });
    c.def("coeffs", [](const Q& q) { return Point4d(q.coeffs()); });
    c.def("vec", [](const Q& q) { return Point3d(q.vec()); });

    c.def("hash", &Q::hash);
    c.def("__hash__", &Q::hash);

    c.def("__mul__", [](const Q& q1, const Q& q2) { return Q(q1 * q2); });
    c.def("__mul__", [](const Q& q, const Point3d& p) { return q * p; });

    c.def("__eq__", [](const Q& a, const Q& b) {
        return a.coeffs() == b.coeffs();
    });
    c.def("__ne__", [](const Q& a, const Q& b) {
        return a.coeffs() != b.coeffs();
    });
}

} // namespace vcl::bind
