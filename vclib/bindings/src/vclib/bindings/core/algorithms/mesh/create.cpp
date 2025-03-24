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

#include <vclib/bindings/core/algorithms/mesh/create.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/create.h>
#include <vclib/algorithms/mesh/type_name.h>

namespace vcl::bind {

void initCreateAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;

    auto fCreateCone = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        std::string name =
            "create_cone_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](double rb, double rt, double h, uint s) {
                return vcl::createCone<MeshType>(rb, rt, h, s);
            },
            py::arg("radius_bottom"),
            py::arg("radius_top"),
            py::arg("height"),
            py::arg("subdivisions") = 36);
    };

    defForAllMeshTypes(m, fCreateCone);

    auto fCreateCylinder = []<FaceMeshConcept MeshType>(
                               pybind11::module& m, MeshType = MeshType()) {
        std::string name =
            "create_cylinder_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](double r, double h, uint s) {
                return vcl::createCylinder<MeshType>(r, h, s);
            },
            py::arg("radius"),
            py::arg("height"),
            py::arg("subdivisions") = 36);
    };

    defForAllMeshTypes(m, fCreateCylinder);

    auto fCreateDodecahedron = []<FaceMeshConcept MeshType>(
                                   pybind11::module& m, MeshType = MeshType()) {
        std::string name = "create_dodecahedron_" +
                           camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(name.c_str(), []() {
            return vcl::createDodecahedron<MeshType>();
        });
    };

    defForAllMeshTypes(m, fCreateDodecahedron);

    auto fCreateCube = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        std::string name =
            "create_cube_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const Point3d& min, double edge) {
                return vcl::createCube<MeshType>(min, edge);
            },
            py::arg("min")  = Point3d(-0.5, -0.5, -0.5),
            py::arg("edge") = 1);
    };

    defForAllMeshTypes(m, fCreateCube);

    auto fCreateHexahedron = []<FaceMeshConcept MeshType>(
                                 pybind11::module& m, MeshType = MeshType()) {
        std::string name = "create_hexahedron_" +
                           camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const Point3d& min, const Point3d& max) {
                return vcl::createHexahedron<MeshType>(min, max);
            },
            py::arg("min"),
            py::arg("max"));
    };

    defForAllMeshTypes(m, fCreateHexahedron);

    auto fCreateIcosahedron = []<FaceMeshConcept MeshType>(
                                  pybind11::module& m, MeshType = MeshType()) {
        std::string name = "create_icosahedron_" +
                           camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(name.c_str(), []() {
            return vcl::createIcosahedron<MeshType>();
        });
    };

    defForAllMeshTypes(m, fCreateIcosahedron);

    auto fCreateSphere = []<FaceMeshConcept MeshType>(
                             pybind11::module& m, MeshType = MeshType()) {
        std::string name =
            "create_sphere_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const Point3d& center, double radius) {
                return vcl::createSphere<MeshType>(Sphere(center, radius));
            },
            py::arg("center"),
            py::arg("radius"));
    };

    defForAllMeshTypes(m, fCreateSphere);

    auto fCreateTetrahedron = []<FaceMeshConcept MeshType>(
                                  pybind11::module& m, MeshType = MeshType()) {
        std::string name = "create_tetrahedron_" +
                           camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(name.c_str(), []() {
            return vcl::createTetrahedron<MeshType>();
        });
    };

    defForAllMeshTypes(m, fCreateTetrahedron);
}

} // namespace vcl::bind
