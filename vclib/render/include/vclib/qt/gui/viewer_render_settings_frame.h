/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_QT_GUI_VIEWER_RENDER_SETTINGS_FRAME_H
#define VCL_QT_GUI_VIEWER_RENDER_SETTINGS_FRAME_H

#include <vclib/render/settings/pbr_viewer_settings.h>
#include <vclib/qt/mesh_viewer_render_app.h>

#include <QFrame>

class QPushButton;

namespace vcl::qt {

namespace Ui {
class ViewerRenderSettingsFrame;
} // namespace Ui

class ViewerRenderSettingsFrame : public QFrame
{
    Q_OBJECT

    enum class RenderMode { CLASSIC = 0, PBR = 1 };

    Ui::ViewerRenderSettingsFrame* mUI;

    MeshViewerRenderApp* mViewer;

public:
    explicit ViewerRenderSettingsFrame(QWidget* parent = nullptr);
    ~ViewerRenderSettingsFrame();

    void setViewer(MeshViewerRenderApp* viewer);

    void setPbrSettings(const PBRViewerSettings& settings);

    const PBRViewerSettings& pbrSettings() const;

private:
    void disableForm();

private slots:
    void renderModeComboBoxCurrentIndexChanged(int index);

    void exposureSpinBoxValueChanged(double value);

    void toneMappingComboBoxCurrentIndexChanged(int index);

    void iblCheckBoxCheckStateChanged(Qt::CheckState state);

    void drawBackgroundPanoramaCheckBoxCheckStateChanged(Qt::CheckState state);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_VIEWER_RENDER_SETTINGS_FRAME_H
