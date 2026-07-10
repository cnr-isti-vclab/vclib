// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/drawable/mesh.h>

#include <vclib/bindings/core/mesh/mesh.h>

#include <vclib/bindings/utils.h>

#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>

namespace vcl::bind {

template<MeshConcept MeshType>
void initDrawableMesh(pybind11::module& m)
{
    std::string typeName = "Drawable" + vcl::meshTypeName<MeshType>();

    pybind11::class_<vcl::DrawableMesh<MeshType>, MeshType> c(
        m, typeName.c_str());
}

void initDrawableMeshes(pybind11::module& m)
{
    namespace py = pybind11;

    // define classes
    auto defDM =
        []<MeshConcept MeshType>(pybind11::module_& m, MeshType = MeshType()) {
            initDrawableMesh<MeshType>(m);
        };
    defForAllMeshTypes(m, defDM);

    // define make_drawable function
    auto defMakeDrawable = []<MeshConcept MeshType>(
                               pybind11::module_& m, MeshType = MeshType()) {
        m.def(
            "make_drawable",
            py::overload_cast<const MeshType&>(&vcl::makeDrawable<MeshType>),
            pybind11::arg("mesh"));
    };
    defForAllMeshTypes(m, defMakeDrawable);
}

} // namespace vcl::bind

