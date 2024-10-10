/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/ext/qt/gui/screen_shot_dialog.h>

#include <QGridLayout>
#include <QLabel>

namespace vcl::qt {

ScreenShotDialog::ScreenShotDialog(QWidget* parent) :
        QFileDialog(
            parent,
            "Save Screen Shot",
            "",
            "Image Files (*.png *.jpg *.jpeg *.bmp)")
{
    setOption(QFileDialog::DontUseNativeDialog);
    setAcceptMode(QFileDialog::AcceptSave);
    QGridLayout* layout = qobject_cast<QGridLayout*>(this->layout());

    // Screen multiplier spin box
    layout->addWidget(new QLabel("Screen Multiplier:"), 4, 0);
    mMultiplierSpinBox = new QDoubleSpinBox();
    mMultiplierSpinBox->setMinimum(1);
    mMultiplierSpinBox->setMaximum(10);
    mMultiplierSpinBox->setValue(1);
    mMultiplierSpinBox->setSingleStep(0.5);

    layout->addWidget(mMultiplierSpinBox, 4, 1);
}

ScreenShotDialog::~ScreenShotDialog()
{
}

float ScreenShotDialog::screenMultiplierValue() const
{
    return mMultiplierSpinBox->value();
}

} // namespace vcl::qt
