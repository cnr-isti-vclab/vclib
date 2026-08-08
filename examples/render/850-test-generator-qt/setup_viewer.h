// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_SETUP_VIEWER_H
#define VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_SETUP_VIEWER_H

#include "get_drawable_mesh.h"

#include <vclib/render/headless_mesh_viewer.h>
#include <vclib/meshes.h>
#include <vector>
#include <string>
#include <functional>
#include <utility>

using CustomAction = std::pair<std::string, std::function<void()>>;

void addSelectionEditorAndCustomActions(
    vcl::HeadlessMeshViewer&   mv,
    std::vector<CustomAction>& actions);

inline std::vector<CustomAction> setupViewer(vcl::HeadlessMeshViewer& mv)
{
    // -------------------------------------------------------------------------
    // Modify this function to setup your desired test environment.
    // This is the equivalent of the `setup` lambda in `runRenderTest`.
    // You can also return custom actions (lambdas) that will be added to the UI
    // to interactively trigger them.
    // -------------------------------------------------------------------------
    
    // adding a mesh
    auto mesh = getDrawableMesh<vcl::TriMesh>();
    mv.pushDrawableObject(std::move(mesh));

    mv.fitScene();

    // adding custom actions:
    std::vector<CustomAction> actions;

    // if you don't need the selection editor, you can comment this line
    addSelectionEditorAndCustomActions(mv, actions);

    // if you need to add additional custom actions, you can do it here:
    // make sure to not use [&] in the lambda
    CustomAction action1 = {"Custom Action 1", [=]() { /* your code here */ }};
    // and then actions.push

    return actions;
}

inline void addSelectionEditorAndCustomActions(
    vcl::HeadlessMeshViewer&   mv,
    std::vector<CustomAction>& actions)
{
    // adding the Selection Editor
    auto selectionEditor = mv.pushEditor<vcl::SelectionEditor>(true);

    actions.push_back(
        {"Enable Vertex Selection", [=]() {
             selectionEditor->settings().customSettings["selectVertices"] =
                 true;
         }});
    actions.push_back(
        {"Enable Face Selection", [=]() {
             selectionEditor->settings().customSettings["selectFaces"] = true;
         }});
    actions.push_back(
        {"Enable Only Visible Selection", [=]() {
             selectionEditor->settings().customSettings["onlyVisible"] = true;
         }});
    actions.push_back(
        {"Disable Vertex Selection", [=]() {
             selectionEditor->settings().customSettings["selectVertices"] =
                 false;
         }});
    actions.push_back(
        {"Disable Face Selection", [=]() {
             selectionEditor->settings().customSettings["selectFaces"] = false;
         }});
    actions.push_back(
        {"Disable Only Visible Selection", [=]() {
             selectionEditor->settings().customSettings["onlyVisible"] = false;
         }});
}

#endif // VCLIB_RENDER_EXAMPLES_TEST_GENERATOR_SETUP_VIEWER_H
