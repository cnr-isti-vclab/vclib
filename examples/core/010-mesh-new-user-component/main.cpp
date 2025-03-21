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

#include "bar_mesh.h"
#include "foo_mesh.h"

#include <vclib/meshes/tri_mesh.h>

int main(int argc, char** argv)
{
    // A mesh with a Vertex that has a FooComponent
    FooMesh m;

    m.addVertices(10);

    for (auto& v : m.vertices()) {
        v.foo() = m.index(v) + 42;
    }

    vcl::TriMesh tm;

    // when importing in a trimesh, the foo component is not copied
    // because the trimesh does not have the component in its vertices
    tm.importFrom(m);

    // reverse importFrom, to assert that everything builds correctly
    m.importFrom(tm);

    // A mesh with a Vertex that has a optional BarComponent
    BarMesh bm;

    bm.addVertices(10);

    // enable the bar component in the vertices
    bm.enablePerElementComponent<vcl::ElemId::VERTEX, BAR_COMPONENT>();

    for (auto& v : bm.vertices()) {
        v.bar()       = bm.index(v) + 42;
        v.barVector() = {1, 2, 3};
    }

    for (auto& v : bm.vertices()) {
        std::cout << "Vertex: " << v.index() << std::endl;
        std::cout << "Bar: " << v.bar() << std::endl;
        std::cout << "Bar vector: ";
        for (auto i : v.barVector()) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
    }
}
