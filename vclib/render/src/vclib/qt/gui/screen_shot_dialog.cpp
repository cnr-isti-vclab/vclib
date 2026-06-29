// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/screen_shot_dialog.h>

#include <QGridLayout>
#include <QLabel>

namespace vcl::qt {

ScreenShotDialog::ScreenShotDialog(QWidget* parent) :
        QFileDialog(
            parent,
            "Save Screenshot",
            "",
            "Image Files (*.png *.jpg *.jpeg *.bmp)")
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

} // namespace vcl::qt
