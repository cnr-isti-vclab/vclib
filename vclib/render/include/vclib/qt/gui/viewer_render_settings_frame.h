// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_VIEWER_RENDER_SETTINGS_FRAME_H
#define VCL_QT_GUI_VIEWER_RENDER_SETTINGS_FRAME_H

#include <vclib/qt/mesh_viewer_render_app.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

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

    void setPanorama(const std::string& panorama);

    const PBRViewerSettings& pbrSettings() const;

private:
    void disableForm();

    void updatePanoramaLabel();

private slots:
    void renderModeComboBoxCurrentIndexChanged(int index);

    void exposureSpinBoxValueChanged(double value);

    void toneMappingComboBoxCurrentIndexChanged(int index);

    void iblCheckBoxCheckStateChanged(Qt::CheckState state);

    void drawBackgroundPanoramaCheckBoxCheckStateChanged(Qt::CheckState state);

    void loadPanoramaPushButtonClicked();
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_VIEWER_RENDER_SETTINGS_FRAME_H
