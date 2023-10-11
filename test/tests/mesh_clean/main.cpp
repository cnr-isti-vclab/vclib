/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <catch2/catch_test_macros.hpp>

template<vcl::FaceMeshConcept MeshType>
void populateTriMesh(MeshType& tm)
{
    // note: p3 and p4 have same coords
    const vcl::Point3d p0(0,0,0);
    const vcl::Point3d p1(1,0,0);
    const vcl::Point3d p2(0,1,0);
    const vcl::Point3d p3(1,1,0);
    const vcl::Point3d p4(1,1,0); // dup of p3
    const vcl::Point3d p5(1,1,1);
    const vcl::Point3d p6(2,0,0); // unref

    tm.addVertices(p0, p1, p2, p3, p4, p5, p6);

    tm.addFace(0, 1, 2);
    tm.addFace(1, 2, 0); // dup of 0
    tm.addFace(1, 2, 0); // dup of 0
    tm.addFace(3, 1, 0);
    tm.addFace(1, 3, 0); // dup of 3
    tm.addFace(1, 4, 0); // not dup of 3 (different coordinates)
    tm.addFace(2, 1, 0); // dup of 0
    tm.addFace(0, 1, 2); // dup of 0
    tm.addFace(5, 3, 4);
}

template<vcl::FaceMeshConcept MeshType>
void populatePolyMesh(MeshType& pm)
{
    // note: p3 and p4 have same coords
    const vcl::Point3d p0(0,0,0);
    const vcl::Point3d p1(1,0,0);
    const vcl::Point3d p2(0,1,0);
    const vcl::Point3d p3(1,1,0);
    const vcl::Point3d p4(1,1,0);

    pm.addVertices(p0, p1, p2, p3, p4);

    pm.addFace(0, 1, 2);
    pm.addFace(0, 1, 2, 3);
    pm.addFace(0, 1, 2, 4); // not dup of 1 (different coordinates)
    pm.addFace(0, 2, 1, 3); // dup of 1
    pm.addFace(4, 1, 2, 0); // dup of 2
    pm.addFace(0, 2, 1); // dup of 0
}

template<vcl::EdgeMeshConcept MeshType>
void populateEdgeMesh(MeshType& m)
{
    const vcl::Point3d p0(0,0,0);
    const vcl::Point3d p1(1,0,0);
    const vcl::Point3d p2(0,1,0); // unref
    const vcl::Point3d p3(1,1,0);

    m.addVertices(p0, p1, p2, p3);

    m.addEdge(0, 1);
    m.addEdge(0, 3);
    m.addEdge(1, 3);
    m.addEdge(3, 1);
}

TEST_CASE( "Clean Duplicated Faces" ) {

    SECTION( "TriMesh" ) {
        vcl::TriMesh tm;

        populateTriMesh(tm);

        REQUIRE( tm.vertexNumber() == 7 );
        REQUIRE( tm.faceNumber() == 9 );

        uint nr = vcl::removeDuplicatedFaces(tm);
        REQUIRE( nr == 5 );
        REQUIRE( tm.vertexNumber() == 7 );
        REQUIRE( tm.faceNumber() == 4 );
    }

    SECTION( "PolyMesh with triangles" ) {
        vcl::PolyMesh pm;

        populateTriMesh(pm);

        REQUIRE(pm.vertexNumber() == 7 );
        REQUIRE(pm.faceNumber() == 9 );

        uint nr = vcl::removeDuplicatedFaces(pm);
        REQUIRE( nr == 5 );
        REQUIRE(pm.vertexNumber() == 7 );
        REQUIRE(pm.faceNumber() == 4 );
    }

    SECTION( "PolyMesh with polygons" ) {
        vcl::PolyMesh pm;

        populatePolyMesh(pm);

        REQUIRE( pm.vertexNumber() == 5 );
        REQUIRE( pm.faceNumber() == 6 );

        uint nr = vcl::removeDuplicatedFaces(pm);

        REQUIRE( nr == 3 );
        REQUIRE( pm.vertexNumber() == 5 );
        REQUIRE( pm.faceNumber() == 3 );
    }
}

TEST_CASE ( "WaterTightness" ) {
    SECTION( "A TriMesh that is not watertight" ) {
        vcl::TriMesh t = vcl::load<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/brain.ply");

        REQUIRE( t.vertexNumber() == 18844 );
        REQUIRE( t.faceNumber() == 36752 );

        REQUIRE( !vcl::isWaterTight(t) );
    }

    SECTION( "A TriMesh that is watertight" ) {
        vcl::TriMesh t = vcl::load<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bone.ply");

        REQUIRE( t.vertexNumber() == 1872 );
        REQUIRE( t.faceNumber() == 3022 );

        REQUIRE( vcl::isWaterTight(t) );
    }
}

TEST_CASE ( "Duplicated Vertices" ) {
    vcl::TriMesh tm;

    populateTriMesh(tm);

    REQUIRE (tm.face(5).vertex(1) == &tm.vertex(4) );

    uint nv = vcl::removeDuplicatedVertices(tm);

    REQUIRE( nv == 1);

    REQUIRE (tm.face(5).vertex(1) == &tm.vertex(3) );
}

TEST_CASE ( "Unreferenced Vertices" ) {
    SECTION("TriMesh") {
        vcl::TriMesh tm;

        populateTriMesh(tm);

        uint nv = vcl::numberUnreferencedVertices(tm);

        REQUIRE( nv == 1);
    }

    SECTION("EdgeMesh") {
        vcl::EdgeMesh em;

        populateEdgeMesh(em);

        uint nv = vcl::numberUnreferencedVertices(em);

        REQUIRE( nv == 1);
    }
}

TEST_CASE ( "Duplicated Vertices brain.ply" ) {
    vcl::TriMesh t = vcl::load<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/brain.ply");

    uint nv = vcl::removeDuplicatedVertices(t);

    SECTION( "Test number duplicated vertices" ) {
        REQUIRE( nv == 453 );
        REQUIRE( t.vertexNumber() == 18844 - nv );
        REQUIRE( t.vertexContainerSize() == 18844 );
        REQUIRE( t.faceNumber() == 36752 );
    }

    SECTION ( "Test compactness" ) {
        REQUIRE( t.vertexNumber() != t.vertexContainerSize() );

        t.compact();

        REQUIRE( t.vertexNumber() == t.vertexContainerSize() );
        REQUIRE( t.vertexNumber() == 18844 - nv );
    }
}


