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

#ifndef SAVE_MESH_H
#define SAVE_MESH_H

#include <vclib/io.h>
#include <vclib/meshes.h>

void saveMeshes(const vcl::TriMesh& mesh0, const vcl::TriMesh& mesh1)
{
    // To save a mesh you can use the save function. The function will
    // automatically detect the file format based on the file extension.
    vcl::save(mesh0, VCLIB_RESULTS_PATH "/000_mesh-io_bunny.obj");

    // You can also specify what kind of data you want to save in the file,
    // using a MeshInfo object in the SaveSettings parameter
    vcl::MeshInfo saveInfo;
    saveInfo.setPerVertexPosition();
    saveInfo.setFaces(false); // do not save faces - only point cloud

    vcl::SaveSettings saveSettings;
    saveSettings.info = saveInfo;
    vcl::save(
        mesh0, VCLIB_RESULTS_PATH "/000_mesh-io_bunny-cloud.obj", saveSettings);

    // You can also specify other save settings trough the SaveSettings object.
    saveInfo.setFaces(true);
    saveInfo.setPerFaceVertexReferences(true);
    saveSettings.info   = saveInfo;
    saveSettings.binary = false; // save in ASCII format
    vcl::save(
        mesh0, VCLIB_RESULTS_PATH "/000_mesh-io_bunny-ascii.ply", saveSettings);
}

#endif // SAVE_MESH_H
