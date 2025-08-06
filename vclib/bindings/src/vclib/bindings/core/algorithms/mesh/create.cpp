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
    using namespace py::literals;

    // export enum for create sphere (must be exported just one time)
    using enum vcl::CreateSphereArgs::CreateSphereMode;

    // CreateSphereMode enum
    py::enum_<vcl::CreateSphereArgs::CreateSphereMode> csenum(
        m, "CreateSphereMode");
    csenum.value("UV", UV);
    csenum.value("NORMALIZED_CUBE", NORMALIZED_CUBE);
    csenum.value("SPHERIFIED_CUBE", SPHERIFIED_CUBE);
    csenum.value("ICOSAHEDRON", ICOSAHEDRON);
    csenum.export_values();

    auto functions = []<FaceMeshConcept MeshType>(
                         pybind11::module& m, MeshType = MeshType()) {
        // cone.h

        std::string name =
            "create_cone_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](double rb, double rt, double h, uint s) {
                return vcl::createCone<MeshType>(rb, rt, h, s);
            },
            "radius_bottom"_a,
            "radius_top"_a,
            "height"_a,
            "subdivisions"_a = 36);

        m.def(
            "create_cone",
            [](MeshType& m, double rb, double rt, double h, uint s) {
                m = vcl::createCone<MeshType>(rb, rt, h, s);
            },
            "mesh"_a,
            "radius_bottom"_a,
            "radius_top"_a,
            "height"_a,
            "subdivisions"_a = 36);

        name =
            "create_cylinder_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](double r, double h, uint s) {
                return vcl::createCylinder<MeshType>(r, h, s);
            },
            "radius"_a,
            "height"_a,
            "subdivisions"_a = 36);

        m.def(
            "create_cylinder",
            [](MeshType& m, double r, double h, uint s) {
                m = vcl::createCylinder<MeshType>(r, h, s);
            },
            "mesh"_a,
            "radius"_a,
            "height"_a,
            "subdivisions"_a = 36);

        // dodecahedron.h

        name = "create_dodecahedron_" +
               camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(name.c_str(), []() {
            return vcl::createDodecahedron<MeshType>();
        });

        m.def(
            "create_dodecahedron",
            [](MeshType& m) {
                m = vcl::createDodecahedron<MeshType>();
            },
            "mesh"_a);

        // hexahedron.h

        name = "create_cube_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const Point3d& min, double edge) {
                return vcl::createCube<MeshType>(min, edge);
            },
            "min"_a  = Point3d(-0.5, -0.5, -0.5),
            "edge"_a = 1);

        m.def(
            "create_cube",
            [](MeshType& m, const Point3d& min, double edge) {
                m = vcl::createCube<MeshType>(min, edge);
            },
            "mesh"_a,
            "min"_a  = Point3d(-0.5, -0.5, -0.5),
            "edge"_a = 1);

        name = "create_hexahedron_" +
               camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const Point3d& min, const Point3d& max) {
                return vcl::createHexahedron<MeshType>(min, max);
            },
            "min"_a = Point3d(-1, -1, -1),
            "max"_a = Point3d(1, 1, 1));

        m.def(
            "create_hexahedron",
            [](MeshType& m, const Point3d& min, const Point3d& max) {
                m = vcl::createHexahedron<MeshType>(min, max);
            },
            "mesh"_a,
            "min"_a = Point3d(-1, -1, -1),
            "max"_a = Point3d(1, 1, 1));

        // icosahedron.h

        name = "create_icosahedron_" +
               camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](bool nv = false) {
                return vcl::createIcosahedron<MeshType>();
            },
            "normalize_vertices"_a = false);

        m.def(
            "create_icosahedron",
            [](MeshType& m, bool nv = false) {
                m = vcl::createIcosahedron<MeshType>(nv);
            },
            "mesh"_a,
            "normalize_vertices"_a = false);

        // sphere.h

        name =
            "create_sphere_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const Point3d&                     center,
               double                             radius,
               CreateSphereArgs::CreateSphereMode mode =
                   CreateSphereArgs::CreateSphereMode::UV,
               uint parallels = 10,
               uint meridians = 20,
               uint divisions = 20) {
                return vcl::createSphere<MeshType>(
                    Sphere(center, radius),
                    CreateSphereArgs {mode, parallels, meridians, divisions});
            },
            "center"_a    = Point3d(0, 0, 0),
            "radius"_a    = 1.0,
            "mode"_a      = UV,
            "parallels"_a = 10,
            "meridians"_a = 20,
            "divisions"_a = 20);

        m.def(
            "create_sphere",
            [](MeshType&                          m,
               const Point3d&                     center,
               double                             radius,
               CreateSphereArgs::CreateSphereMode mode      = UV,
               uint                               parallels = 10,
               uint                               meridians = 20,
               uint                               divisions = 20) {
                m = vcl::createSphere<MeshType>(
                    Sphere(center, radius),
                    CreateSphereArgs {mode, parallels, meridians, divisions});
            },
            "mesh"_a,
            "center"_a    = Point3d(0, 0, 0),
            "radius"_a    = 1.0,
            "mode"_a      = UV,
            "parallels"_a = 10,
            "meridians"_a = 20,
            "divisions"_a = 20);

        // tetrahedron.h

        name = "create_tetrahedron_" +
               camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const Point3d& p0,
               const Point3d& p1,
               const Point3d& p2,
               const Point3d& p3) {
                return vcl::createTetrahedron<MeshType>(p0, p1, p2, p3);
            },
            "p0"_a = Point3d(1, 1, 1),
            "p1"_a = Point3d(-1, 1, -1),
            "p2"_a = Point3d(-1, -1, 1),
            "p3"_a = Point3d(1, -1, -1));

        m.def(
            "create_tetrahedron",
            [](MeshType&      m,
               const Point3d& p0,
               const Point3d& p1,
               const Point3d& p2,
               const Point3d& p3) {
                m = vcl::createTetrahedron<MeshType>(p0, p1, p2, p3);
            },
            "mesh"_a,
            "p0"_a = Point3d(1, 1, 1),
            "p1"_a = Point3d(-1, 1, -1),
            "p2"_a = Point3d(-1, -1, 1),
            "p3"_a = Point3d(1, -1, -1));
    };

    defForAllMeshTypes(m, functions);
}

} // namespace vcl::bind
