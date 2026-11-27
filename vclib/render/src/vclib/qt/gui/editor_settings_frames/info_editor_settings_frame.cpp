// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/editor_settings_frames/info_editor_settings_frame.h>

#include <vclib/space/core.h>

#include "ui_info_editor_settings_frame.h"

namespace vcl::qt {

InfoEditorSettingsFrame::InfoEditorSettingsFrame(
    InfoEditorSettings& sts,
    QWidget*        parent) :
        QFrame(parent), mUI(new Ui::InfoEditorSettingsFrame), mSettings(sts)
{
    mUI->setupUi(this);

    updateGUI();

    connect(
        mUI->highlightWidthSlider,
        &QSlider::valueChanged,
        this,
        &InfoEditorSettingsFrame::onLinesWidthSliderValueChanged);

    connect(
        mUI->highlightColorPushButton,
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onColorChanged(const QColor&)));

    connect(
        mUI->textSizeSpinBox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &InfoEditorSettingsFrame::onTextSizeChanged);

    connect(
        mUI->textColorPushButton,
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onTextColorChanged(const QColor&)));

    connect(
        mUI->resetDefaultButton,
        SIGNAL(clicked()),
        this,
        SLOT(onResetDefaultClicked()));
}

InfoEditorSettingsFrame::~InfoEditorSettingsFrame()
{
    delete mUI;
}

void InfoEditorSettingsFrame::updateGUI()
{
    bool b1 = mUI->highlightWidthSlider->blockSignals(true);
    bool b2 = mUI->highlightColorPushButton->blockSignals(true);
    bool b3 = mUI->textSizeSpinBox->blockSignals(true);
    bool b4 = mUI->textColorPushButton->blockSignals(true);

    Color c = mSettings.color;
    float thickness = mSettings.thickness;
    Color tc = mSettings.textColor;
    int textSize = mSettings.textSize;

    mUI->editModeFrame->hide();
    mUI->highlightWidthSlider->setValue(int(thickness));
    mUI->highlightColorPushButton->setBackgroundColor(
        QColor(c.red(), c.green(), c.blue(), c.alpha()));
        
    mUI->textSizeSpinBox->setValue(textSize);
    mUI->textColorPushButton->setBackgroundColor(
        QColor(tc.red(), tc.green(), tc.blue(), tc.alpha()));

    mUI->highlightWidthSlider->blockSignals(b1);
    mUI->highlightColorPushButton->blockSignals(b2);
    mUI->textSizeSpinBox->blockSignals(b3);
    mUI->textColorPushButton->blockSignals(b4);
}

void InfoEditorSettingsFrame::onLinesWidthSliderValueChanged(int value)
{
    mSettings.thickness = float(value);
    emit settingsUpdated();
}

void InfoEditorSettingsFrame::onColorChanged(const QColor& c)
{
    mSettings.color =
        Color(c.red(), c.green(), c.blue(), c.alpha());
    emit settingsUpdated();
}

void InfoEditorSettingsFrame::onTextSizeChanged(int value)
{
    mSettings.textSize = value;
    emit settingsUpdated();
}

void InfoEditorSettingsFrame::onTextColorChanged(const QColor& c)
{
    mSettings.textColor =
        Color(c.red(), c.green(), c.blue(), c.alpha());
    emit settingsUpdated();
}

void InfoEditorSettingsFrame::onResetDefaultClicked()
{
    mSettings.resetDefaults();
    updateGUI();
    emit settingsUpdated();
}

} // namespace vcl::qt
