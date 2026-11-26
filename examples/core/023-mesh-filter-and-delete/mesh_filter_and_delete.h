// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MESH_FILTER_AND_DELETE_H
#define MESH_FILTER_AND_DELETE_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <cmath>
#include <vector>

// This example demonstrates mesh filtering and conditional deletion algorithms:
// 1. Creating new meshes by filtering vertices (predicate and selection)
// 2. Creating new meshes by filtering faces (predicate, boolean mask, and
//    selection)
// 3. Creating new EdgeMeshes by filtering edges (predicate and selection)
// 4. Deleting faces, vertices, and edges in-place with delete*If

inline auto meshFilterAndDelete()
{
    std::cout << "=== VCLib Example 023: Mesh Filter and Delete ===\n\n";

    /****** Loading input mesh ******/

    vcl::TriMesh bunny =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    bunny.name() = "Original Bunny";
    vcl::updateVertexAndFaceNormals(bunny);

    std::cout << "Loaded bunny: " << bunny.vertexCount() << " vertices, "
              << bunny.faceCount() << " faces\n";

    /****** 1. Vertex Filtering ******/

    std::cout << "\n=== 1. Vertex Filtering ===" << std::endl;

    // Filter vertices using a predicate function
    vcl::TriMesh upperVertices =
        vcl::vertexMeshFilter(bunny, [](vcl::TriMesh::ConstVertex v) {
            return v.position().y() > 0.1;
        });
    upperVertices.name() = "Upper Vertices";

    std::cout << "Upper vertices (y > 0.1): " << upperVertices.vertexCount()
              << " vertices, " << upperVertices.faceCount() << " faces\n";

    // Filter vertices by selection
    for (auto v : bunny.vertices()) {
        v.selected() = v.position().x() > 0.3;
    }
    vcl::TriMesh rightSelectedVertices = vcl::vertexSelectionMeshFilter(bunny);
    rightSelectedVertices.name()       = "Right Selected Vertices";

    std::cout << "Selected vertices (x > 0.3): "
              << rightSelectedVertices.vertexCount() << " vertices\n";

    /****** 2. Face Filtering ******/

    std::cout << "\n=== 2. Face Filtering ===" << std::endl;

    // Filter faces using a predicate function
    vcl::TriMesh upwardFaces = vcl::faceMeshFilter(
        bunny, [](vcl::TriMesh::ConstFace f) { return f.normal().y() > 0.0; });
    upwardFaces.name() = "Upward Faces";

    std::cout << "Upward-facing mesh (normal.y > 0): "
              << upwardFaces.vertexCount() << " vertices, "
              << upwardFaces.faceCount() << " faces\n";

    // Filter faces using a boolean mask range
    std::vector<bool> faceMask;
    faceMask.reserve(bunny.faceCount());
    for (auto f : bunny.faces()) {
        faceMask.push_back(vcl::faceBarycenter(f).y() > 0.1);
    }

    vcl::TriMesh upperFaces = vcl::faceMeshFilter(bunny, faceMask);
    upperFaces.name()       = "Upper Faces";

    std::cout << "Upper faces (barycenter.y > 0.1): "
              << upperFaces.vertexCount() << " vertices, "
              << upperFaces.faceCount() << " faces\n";

    // Filter faces by selection
    vcl::clearFaceSelection(bunny);
    for (auto f : bunny.faces()) {
        if (f.normal().y() > 0.5) {
            f.selected() = true;
        }
    }
    vcl::TriMesh selectedFaces = vcl::faceSelectionMeshFilter(bunny);
    selectedFaces.name()       = "Selected Faces";

    std::cout << "Selected faces (normal.y > 0.5): "
              << selectedFaces.vertexCount() << " vertices, "
              << selectedFaces.faceCount() << " faces\n";

    /****** 3. Edge Filtering ******/

    std::cout << "\n=== 3. Edge Filtering ===" << std::endl;

    // Build an EdgeMesh from the crease edges of a cylinder
    vcl::TriMesh cylinder = vcl::createCylinder<vcl::TriMesh>(1.0, 2.0, 24);
    cylinder.enableFaceAdjacentFaces();
    vcl::updateFaceAdjacentFaces(cylinder);
    vcl::selectCreaseFaceEdges(cylinder, -vcl::toRad(60.0), vcl::toRad(60.0));

    vcl::EdgeMesh creaseEdges =
        vcl::faceEdgeSelectionMeshFilter<vcl::EdgeMesh>(cylinder);
    std::cout << "Extracted crease EdgeMesh: " << creaseEdges.vertexCount()
              << " vertices, " << creaseEdges.edgeCount() << " edges\n";

    // Filter edges with a predicate (only edges in the upper cap)
    vcl::EdgeMesh topCapEdges =
        vcl::edgeMeshFilter(creaseEdges, [](vcl::EdgeMesh::ConstEdge e) {
            return e.vertex(0).position().y() > 0.0 &&
                   e.vertex(1).position().y() > 0.0;
        });
    topCapEdges.name() = "Top Cap Edges";

    std::cout << "Filtered top cap edges: " << topCapEdges.vertexCount()
              << " vertices, " << topCapEdges.edgeCount() << " edges\n";

    // Filter edges by selection
    for (auto e : creaseEdges.edges()) {
        e.selected() = e.vertex(0).position().x() > 0.0;
    }
    vcl::EdgeMesh selectedEdges = vcl::edgeSelectionMeshFilter(creaseEdges);
    selectedEdges.name()        = "Selected Edges";

    std::cout << "Selected edges: " << selectedEdges.vertexCount()
              << " vertices, " << selectedEdges.edgeCount() << " edges\n";

    /****** 4. In-place Conditional Deletion (delete*If) ******/

    std::cout << "\n=== 4. In-place Conditional Deletion ===" << std::endl;

    // 4a. Delete faces in-place using a predicate
    vcl::TriMesh deletedFacesMesh = bunny;
    deletedFacesMesh.name()       = "Deleted Faces Bunny";

    vcl::uint nDelFaces =
        vcl::deleteFacesIf(deletedFacesMesh, [](vcl::TriMesh::ConstFace f) {
            return f.normal().y() < 0.0;
        });
    vcl::removeUnreferencedVertices(deletedFacesMesh);
    deletedFacesMesh.compact();

    std::cout << "Deleted " << nDelFaces << " downward faces -> remaining: "
              << deletedFacesMesh.vertexCount() << " vertices, "
              << deletedFacesMesh.faceCount() << " faces\n";

    // 4b. Delete vertices in-place and clean invalidated faces
    vcl::TriMesh deletedVertsMesh = bunny;
    deletedVertsMesh.name()       = "Deleted Vertices Bunny";

    vcl::uint nDelVerts = vcl::deleteVerticesIf(
        deletedVertsMesh,
        [](vcl::TriMesh::ConstVertex v) { return v.position().y() < 0.1; });
    // Remove faces whose vertex references were invalidated (NULL_INDEX)
    vcl::uint nCleanedFaces =
        vcl::deleteFacesIf(deletedVertsMesh, [](vcl::TriMesh::ConstFace f) {
            for (auto vi : f.vertexIndices()) {
                if (vi == vcl::TriMesh::NULL_INDEX) {
                    return true;
                }
            }
            return false;
        });
    deletedVertsMesh.compact();

    std::cout << "Deleted " << nDelVerts << " vertices and " << nCleanedFaces
              << " invalidated faces -> remaining: "
              << deletedVertsMesh.vertexCount() << " vertices, "
              << deletedVertsMesh.faceCount() << " faces\n";

    // 4c. Delete edges in-place using a boolean mask range
    vcl::EdgeMesh deletedEdgesMesh = creaseEdges;
    deletedEdgesMesh.name()        = "Deleted Edges";

    std::vector<bool> edgeDeleteMask;
    edgeDeleteMask.reserve(deletedEdgesMesh.edgeCount());
    for (auto e : deletedEdgesMesh.edges()) {
        edgeDeleteMask.push_back(e.vertex(0).position().y() < 0.0);
    }

    vcl::uint nDelEdges = vcl::deleteEdgesIf(deletedEdgesMesh, edgeDeleteMask);
    vcl::removeUnreferencedVertices(deletedEdgesMesh);
    deletedEdgesMesh.compact();

    std::cout << "Deleted " << nDelEdges << " bottom edges -> remaining: "
              << deletedEdgesMesh.vertexCount() << " vertices, "
              << deletedEdgesMesh.edgeCount() << " edges\n";

    return std::make_tuple(
        std::move(upperVertices),
        std::move(upperFaces),
        std::move(selectedFaces),
        std::move(topCapEdges),
        std::move(deletedFacesMesh),
        std::move(deletedVertsMesh),
        std::move(deletedEdgesMesh));
}

#endif // MESH_FILTER_AND_DELETE_H
