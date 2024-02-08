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

#include <iostream>

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

class MyVertex :
        public vcl::Vertex<
            vcl::vert::BitFlags,
            vcl::vert::Coordinate3d,
            vcl::vert::Normal3f,
            vcl::vert::Color>
{
};

int main()
{
    vcl::TriMesh m;

    using TriMeshPoint = vcl::TriMesh::VertexType::CoordType;

    // adding some vertices
    m.addVertex();
    m.addVertex();
    m.addVertex();

    // setting coordinate
    m.vertex(0).coord() = TriMeshPoint(0, 0, 0);
    m.vertex(1).coord() = TriMeshPoint(0, 1, 0);
    m.vertex(2).coord() = TriMeshPoint(1, 0, 0);

    m.vertex(0).component<vcl::COORDINATE>().coord() = TriMeshPoint(2, 0, 0);

    vcl::TriMesh::Vertex myv;
    myv.coord() = TriMeshPoint(3, 1, 2);

    m.enablePerVertexTexCoord();

    m.vertex(1) = myv;

    m.vertex(0).texCoord().set(0.1, 0.2);
    m.vertex(1).texCoord().set(0.52, 0.35);

    // setting vertex normals
    // in this mesh vertex normaks are not optional, I don't need to enable them
    m.vertex(0).normal() = TriMeshPoint(1, 0, 0);

    m.enablePerVertexColor();

    // setting vertex colors
    m.vertex(0).color() = vcl::Color(3, 7, 4);
    m.vertex(1).color() = vcl::Color(3, 63, 44);
    m.vertex(2).color() = vcl::Color(3, 9, 2);

    // adding a per vertex custom component of chars, called 'prova'
    m.addPerVertexCustomComponent<char>("prova");

    // using generic element type alias trough element type index
    // this is just for test - it is possible to just use
    // using VT = vcl::TriMesh::VertexType;
    // or just use `auto` in range for
    using VT = vcl::TriMesh::ElementType<vcl::ElemId::VERTEX>;

    for (VT& v : m.vertices())
        v.customComponent<char>("prova") =
            'a'; // set the custom component for each vertex

    // enable per face quality, which is optional
    m.enablePerFaceQuality();

    // add some faces
    m.addFace();
    m.addFace();
    m.addFace();
    m.addFace();
    m.addFace();

    // enable face colors after faces are added
    m.enablePerFaceColor();

    // set vertices of f0
    m.face(0).setVertices(&m.vertex(0), &m.vertex(1), &m.vertex(2));
    m.face(0).quality() = 4;

    // setting some colors to all the faces of the mesh
    uint i = 17;
    for (vcl::TriMesh::Face& f :
         m.faces()) // iterates over all the non-deleted faces of m
        f.color() = vcl::Color(4 + 2 * i++, 134, 98);

    // add some other vertices
    m.addVertex();
    m.addVertex();
    m.addVertex();

    std::cerr << "f0 quality: " << m.face(0).quality() << "\n";
    for (const vcl::TriMesh::Face& f : m.faces())
        std::cerr << m.index(f) << " Color: " << f.color() << "\n";

    std::cerr << "VN: " << m.vertexNumber() << "\n";

    // iterate over vertices
    for (const vcl::TriMesh::Vertex& v : m.vertices()) {
        std::cerr << "V " << m.index(v) << "\n";
    }

    // test import data from another vertex type
    MyVertex vert;
    vert.importFrom(m.vertex(0));

    return 0;
}
