// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/screen_shot_dialog.h>

#include <vclib/qt/gui/dialog_directories.h>

#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>

namespace vcl::qt {

ScreenShotDialog::ScreenShotDialog(
    QWidget*           parent,
    const std::string& settingsFilePath) :
        QFileDialog(
            parent,
            "Save Screenshot",
            dialogDirectory("SaveScreenshot", settingsFilePath),
            "Image Files (*.png *.jpg *.jpeg *.bmp)"),
        mSettingsFilePath(settingsFilePath)
{
    setOption(QFileDialog::DontUseNativeDialog);
    setAcceptMode(QFileDialog::AcceptSave);
    QGridLayout* layout = qobject_cast<QGridLayout*>(this->layout());

    // Screen multiplier spin box
    layout->addWidget(new QLabel("Screen Multiplier:"), 4, 0);
    mMultiplierSpinBox = new QSpinBox();
    mMultiplierSpinBox->setMinimum(1);
    mMultiplierSpinBox->setMaximum(10);
    mMultiplierSpinBox->setValue(1);

    layout->addWidget(mMultiplierSpinBox, 4, 1);
}

ScreenShotDialog::~ScreenShotDialog()
{
}

int ScreenShotDialog::screenMultiplierValue() const
{
    return mMultiplierSpinBox->value();
}

void ScreenShotDialog::accept()
{
    if (selectedFiles().size() > 0) {
        setDialogDirectory(
            "SaveScreenshot",
            QFileInfo(selectedFiles()[0]).absolutePath(),
            mSettingsFilePath);
    }
    QFileDialog::accept();
}

} // namespace vcl::qt
