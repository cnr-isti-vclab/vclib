// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/render/headless_viewer.h>
#include <vclib/io.h>

#include <iostream>

int main(int argc, char** argv)
{
    vcl::HeadlessViewer tw("Viewer Headless");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.fitScene();

    // Request a screenshot
    std::string screenshotFilename = "screenshot.png";
    std::cout << "Requesting screenshot: " << screenshotFilename << std::endl;
    
    // We must initialize the application explicitly if we want to take a
    // screenshot before the show() method is called. This guarantees that
    // the Drawers have been properly initialized (including their default
    // clear color like DarkGray).
    tw.init();
    
    tw.screenshot(screenshotFilename);
    
    // Run the headless app
    // It will run until the screenshot readback is complete
    std::cout << "Starting rendering loop..." << std::endl;
    tw.show();
    std::cout << "Rendering loop finished. Check " << screenshotFilename
              << " for the output." << std::endl;

    return 0;
}
