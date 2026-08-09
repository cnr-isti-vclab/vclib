// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_H
#define VCL_QT_GUI_SETTINGS_DIALOG_H

#include <QDialog>
#include <vclib/render/settings/bounding_box_editor_settings.h>
#include <vclib/render/settings/selection_editor_settings.h>

namespace vcl::qt {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(
        const SelectionEditorSettings&   selSts,
        const BoundingBoxEditorSettings& bboxSts,
        QWidget*                         parent = nullptr);
    ~SettingsDialog();

    const SelectionEditorSettings& selectionSettings() const;
    const BoundingBoxEditorSettings& boundingBoxSettings() const;

signals:
    void applied();

private slots:
    void onApplyClicked();
    void onSaveDefaultsClicked();

private:
    Ui::SettingsDialog*       mUI;
    SelectionEditorSettings   mSelSts;
    BoundingBoxEditorSettings mBBoxSts;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_H
