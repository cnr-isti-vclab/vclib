// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/imgui/mesh_viewer.h>
#include <vclib/render/mesh_viewer.h>

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>

int main(int argc, char** argv)
{
    vcl::imgui::MeshViewer tw;
    vcl::pushDefaultEditors(tw);

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    // update the mesh to be displayed in the scene
    const auto bb = vcl::boundingBox(drawable);
    vcl::scale(drawable, 0.5f);
    vcl::translate(drawable, vcl::Point3d(bb.size().x(), 0, 0));

    tw.pushDrawableObject(drawable);

    tw.refreshEditors();

    tw.fitScene();

    tw.show();

    return 0;
}
