// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_filter_and_delete.h"

int main()
{
    auto
        [upperVertices,
         upperFaces,
         selectedFaces,
         topCapEdges,
         deletedFacesMesh,
         deletedVertsMesh,
         deletedEdgesMesh] = meshFilterAndDelete();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(
            upperVertices, resultsPath + "/023_upper_vertices_bunny.ply");
        vcl::saveMesh(upperFaces, resultsPath + "/023_upper_faces_bunny.ply");
        vcl::saveMesh(
            selectedFaces, resultsPath + "/023_selected_faces_bunny.ply");
        vcl::saveMesh(topCapEdges, resultsPath + "/023_top_cap_edges.ply");
        vcl::saveMesh(
            deletedFacesMesh, resultsPath + "/023_deleted_faces_bunny.ply");
        vcl::saveMesh(
            deletedVertsMesh, resultsPath + "/023_deleted_vertices_bunny.ply");
        vcl::saveMesh(deletedEdgesMesh, resultsPath + "/023_deleted_edges.ply");

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;
}
