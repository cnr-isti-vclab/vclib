// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SCREEN_SHOT_DIALOG_H
#define VCL_QT_GUI_SCREEN_SHOT_DIALOG_H

#include <QFileDialog>
#include <QSpinBox>

namespace vcl::qt {

class ScreenShotDialog : public QFileDialog
{
    Q_OBJECT

    QSpinBox* mMultiplierSpinBox = nullptr;

    std::string mSettingsFilePath;

public:
    explicit ScreenShotDialog(
        QWidget*           parent           = nullptr,
        const std::string& settingsFilePath = "");
    ~ScreenShotDialog();

    int screenMultiplierValue() const;

public slots:
    void accept() override;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SCREEN_SHOT_DIALOG_H
