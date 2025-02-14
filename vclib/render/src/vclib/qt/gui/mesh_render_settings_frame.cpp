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

#include <vclib/qt/gui/mesh_render_settings_frame.h>

#include "ui_mesh_render_settings_frame.h"

#include <vclib/qt/gui/mesh_render_settings_frame/edges_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/points_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/surface_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/wireframe_frame.h>

#include <QColorDialog>
#include <QStandardItemModel>

namespace vcl::qt {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::MeshRenderSettingsFrame)
{
    mUI->setupUi(this);

    auto* pointsFrame = new PointsFrame(mMRS, this);
    mUI->tabWidget->addTab(pointsFrame, "Points");
    frames.push_back(pointsFrame);

    auto* surfaceFrame = new SurfaceFrame(mMRS, this);
    mUI->tabWidget->addTab(surfaceFrame, "Surface");
    frames.push_back(surfaceFrame);

    auto* wireframeFrame = new WireframeFrame(mMRS, this);
    mUI->tabWidget->addTab(wireframeFrame, "Wireframe");
    frames.push_back(wireframeFrame);

    auto* edgesFrame = new EdgesFrame(mMRS, this);
    mUI->tabWidget->addTab(edgesFrame, "Edges");
    frames.push_back(edgesFrame);

    for (auto* frame : frames) {
        connect(
            frame, SIGNAL(settingsUpdated()), this, SIGNAL(settingsUpdated()));
    }
}

MeshRenderSettingsFrame::~MeshRenderSettingsFrame()
{
    delete mUI;
}

const MeshRenderSettings& MeshRenderSettingsFrame::meshRenderSettings() const
{
    return mMRS;
}

void MeshRenderSettingsFrame::setMeshRenderSettings(
    const MeshRenderSettings& settings,
    bool                      changeCurrentTab /*= false*/)
{
    mMRS = settings;
    updateGuiFromSettings(changeCurrentTab);
}

void MeshRenderSettingsFrame::updateGuiFromSettings(bool changeCurrentTab)
{
    using MRI = MeshRenderInfo;

    for (auto* frame : frames) {
        frame->updateFrameFromSettings();
    }

    if (mMRS.canBeVisible()) {
        mUI->tabWidget->setEnabled(true);
    }
    else {
        mUI->tabWidget->setEnabled(false);
    }

    mUI->tabWidget->setTabVisible(
        POINTS_FRAME, mMRS.canPoints(MRI::Points::VISIBLE));
    mUI->tabWidget->setTabVisible(
        SURFACE_FRAME, mMRS.canSurface(MRI::Surface::VISIBLE));
    mUI->tabWidget->setTabVisible(
        WIREFRAME_FRAME, mMRS.canWireframe(MRI::Wireframe::VISIBLE));
    mUI->tabWidget->setTabVisible(
        EDGES_FRAME, mMRS.canEdges(MRI::Edges::VISIBLE));

    if (changeCurrentTab) {
        if (mMRS.canSurface(MRI::Surface::VISIBLE)) { // high priority: surface
            mUI->tabWidget->setCurrentIndex(SURFACE_FRAME);
        }
        else if (mMRS.canEdges(MRI::Edges::VISIBLE)) { // second priority: edges
            mUI->tabWidget->setCurrentIndex(EDGES_FRAME);
        }
        else if (mMRS.canPoints(MRI::Points::VISIBLE)) { // lowest: points
            mUI->tabWidget->setCurrentIndex(POINTS_FRAME);
        }
    }
}

} // namespace vcl::qt
