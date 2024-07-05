/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vclib/algorithms/mesh/check_pointers.h>
#include <vclib/algorithms/mesh/create.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/meshes.h>

TEMPLATE_TEST_CASE(
    "Copy a Mesh",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::PolyMesh,
    vcl::PolyMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf,
    vcl::PolyMeshIndexed,
    vcl::PolyMeshIndexedf)
{
    using Mesh = TestType;

    Mesh m1 = vcl::createCube<Mesh>();

    vcl::updatePerVertexAndFaceNormals(m1);

    m1.template addPerVertexCustomComponent<float>("v_comp");

    // putting some value into v_comp
    for (auto& v : m1.vertices()) {
        v.template customComponent<float>("v_comp") = v.normal().x() * 3;
    }

    WHEN("Copying a mesh")
    {
        Mesh m2 = m1;

        THEN("Right number of vertices and faces")
        {
            REQUIRE(m2.vertexNumber() == m1.vertexNumber());
            REQUIRE(m2.faceNumber() == m1.faceNumber());
        }

        THEN("The mesh is consistent")
        {
            REQUIRE(vcl::checkMeshPointers(m2));
        }

        THEN("Vertices are copied correctly")
        {
            for (size_t i = 0; i < m1.vertexNumber(); ++i) {
                REQUIRE(m2.vertex(i).coord() == m1.vertex(i).coord());
                REQUIRE(m2.vertex(i).normal() == m1.vertex(i).normal());
                REQUIRE(
                    m2.vertex(i).template customComponent<float>("v_comp") ==
                    m1.vertex(i).template customComponent<float>("v_comp"));
            }
        }

        THEN("Faces are copied correctly")
        {
            for (size_t i = 0; i < m1.faceNumber(); ++i) {
                REQUIRE(m1.face(i).vertexNumber() == m2.face(i).vertexNumber());
                for (size_t j = 0; j < m1.face(i).vertexNumber(); ++j) {
                    REQUIRE(
                        m2.face(i).vertexIndex(j) == m1.face(i).vertexIndex(j));
                }
                REQUIRE(m1.face(i).normal() == m2.face(i).normal());
            }
        }
    }

    WHEN("Appending a mesh")
    {
        Mesh m2 = vcl::createTetrahedron<Mesh>();
        vcl::updatePerVertexAndFaceNormals(m2);

        m2.template addPerVertexCustomComponent<float>("v_comp");

        // putting some value into v_comp
        for (auto& v : m2.vertices()) {
            v.template customComponent<float>("v_comp") = v.normal().z() * 3;
        }

        Mesh m3 = m1;
        m3.append(m2);

        uint m1vn = m1.vertexNumber();
        uint m2vn = m2.vertexNumber();
        uint m1fn = m1.faceNumber();
        uint m2fn = m2.faceNumber();

        THEN("Right number of vertices and faces")
        {
            REQUIRE(m3.vertexNumber() == m1vn + m2vn);
            REQUIRE(m3.faceNumber() == m1fn + m2fn);
        }

        THEN("The mesh is consistent")
        {
            REQUIRE(vcl::checkMeshPointers(m3));
        }

        THEN("Vertices are copied correctly")
        {
            for (size_t i = 0; i < m1vn; ++i) {
                REQUIRE(m3.vertex(i).coord() == m1.vertex(i).coord());
                REQUIRE(m3.vertex(i).normal() == m1.vertex(i).normal());
                REQUIRE(
                    m3.vertex(i).template customComponent<float>("v_comp") ==
                    m1.vertex(i).template customComponent<float>("v_comp"));
            }

            for (size_t i = m1vn; i < m1vn + m2vn; ++i) {
                REQUIRE(m3.vertex(i).coord() == m2.vertex(i - m1vn).coord());
                REQUIRE(m3.vertex(i).normal() == m2.vertex(i - m1vn).normal());
                REQUIRE(
                    m3.vertex(i).template customComponent<float>("v_comp") ==
                    m2.vertex(i - m1vn).template customComponent<float>(
                        "v_comp"));
            }
        }

        THEN("Faces are copied correctly")
        {
            for (size_t i = 0; i < m1fn; ++i) {
                REQUIRE(m3.face(i).vertexNumber() == m1.face(i).vertexNumber());
                for (size_t j = 0; j < m1.face(i).vertexNumber(); ++j) {
                    REQUIRE(
                        m3.face(i).vertexIndex(j) == m1.face(i).vertexIndex(j));
                }
                REQUIRE(m3.face(i).normal() == m1.face(i).normal());
            }

            for (size_t i = m1fn; i < m1fn + m2fn; ++i) {
                REQUIRE(
                    m3.face(i).vertexNumber() ==
                    m2.face(i - m1fn).vertexNumber());
                for (size_t j = 0; j < m3.face(i).vertexNumber(); ++j) {
                    REQUIRE(
                        m3.face(i).vertexIndex(j) ==
                        m2.face(i - m1fn).vertexIndex(j) + m1vn);
                }
                REQUIRE(m3.face(i).normal() == m2.face(i - m1fn).normal());
            }
        }
    }
}
