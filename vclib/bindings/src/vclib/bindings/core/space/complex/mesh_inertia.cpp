// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/complex/mesh_inertia.h>
#include <vclib/bindings/utils.h>

#include <vclib/meshes.h>
#include <vclib/space/complex.h>

#include <pybind11/stl.h>

#include <stdexcept>
#include <utility>
#include <variant>

namespace vcl::bind {

namespace detail {

template<typename MeshType>
struct IsFaceMeshPred : std::bool_constant<FaceMeshConcept<MeshType>>
{
};

template<typename TypeList>
struct MeshInertiaVariantHelper;

template<FaceMeshConcept... MeshTypes>
struct MeshInertiaVariantHelper<TypeWrapper<MeshTypes...>>
{
    using type = std::variant<vcl::MeshInertia<MeshTypes>...>;
};

using BindedFaceMeshTypes =
    vcl::FilterTypesByCondition<IsFaceMeshPred, BindedMeshTypes>::type;

using MeshInertiaVariant = MeshInertiaVariantHelper<BindedFaceMeshTypes>::type;

struct PyMeshInertia
{
    MeshInertiaVariant inertia;

    template<FaceMeshConcept MeshType>
    explicit PyMeshInertia(const MeshType& mesh) :
            inertia(vcl::MeshInertia<MeshType>(mesh))
    {
    }

    double volume() const
    {
        return std::visit([](const auto& mi) { return mi.volume(); }, inertia);
    }

    void checkNonZeroVolume() const
    {
        if (volume() == 0.0) {
            throw std::runtime_error(
                "Signed volume is zero; center of mass and inertia tensor are "
                "undefined.");
        }
    }

    Point3d centerOfMass() const
    {
        checkNonZeroVolume();
        return std::visit(
            [](const auto& mi) { return mi.centerOfMass(); }, inertia);
    }

    Matrix33d inertiaTensor() const
    {
        checkNonZeroVolume();
        return std::visit(
            [](const auto& mi) {
                return mi.template inertiaTensor<Matrix33d>();
            },
            inertia);
    }

    std::pair<Matrix33d, Point3d> inertiaTensorEigen() const
    {
        checkNonZeroVolume();
        return std::visit(
            [](const auto& mi) {
                Matrix33d eigenVectors;
                Point3d   eigenValues;
                mi.inertiaTensorEigen(eigenVectors, eigenValues);
                return std::make_pair(eigenVectors, eigenValues);
            },
            inertia);
    }
};

} // namespace detail

void initMeshInertia(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;
    using detail::PyMeshInertia;

    py::class_<PyMeshInertia> c(m, "MeshInertia");

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           py::class_<PyMeshInertia>& c,
                           MeshType = MeshType()) {
        c.def(py::init<const MeshType&>(), "mesh"_a);
    };
    defForAllMeshTypes(c, fFaceMeshes);

    c.def("volume", &PyMeshInertia::volume);
    c.def("center_of_mass", &PyMeshInertia::centerOfMass);
    c.def("inertia_tensor", &PyMeshInertia::inertiaTensor);
    c.def("inertia_tensor_eigen", &PyMeshInertia::inertiaTensorEigen);
}

} // namespace vcl::bind
