// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/toolbar_frames/settings/info_editor_settings_frame.h>

#include <vclib/space/core.h>

#include "ui_info_editor_settings_frame.h"

namespace vcl::qt {

InfoEditorSettingsFrame::InfoEditorSettingsFrame(
    EditorSettings& sts,
    QWidget*        parent) :
        QFrame(parent), mUI(new Ui::InfoEditorSettingsFrame), mSettings(sts)
{
    mUI->setupUi(this);

    assert(mSettings.customSettings.at("color").has_value());
    assert(mSettings.customSettings.at("thickness").has_value());

    Color c = std::any_cast<Color>(mSettings.customSettings.at("color"));

    float thickness =
        std::any_cast<float>(mSettings.customSettings.at("thickness"));

    mUI->editModeFrame->hide();
    mUI->linesWidthSlider->setValue(int(thickness));
    mUI->colorPushButton->setBackgroundColor(
        QColor(c.red(), c.green(), c.blue(), c.alpha()));

    connect(
        mUI->linesWidthSlider,
        &QSlider::valueChanged,
        this,
        &InfoEditorSettingsFrame::onLinesWidthSliderValueChanged);

    connect(
        mUI->colorPushButton,
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onColorChanged(const QColor&)));
}

InfoEditorSettingsFrame::~InfoEditorSettingsFrame()
{
    delete mUI;
}

void InfoEditorSettingsFrame::onLinesWidthSliderValueChanged(int value)
{
    mSettings.customSettings["thickness"] = float(value);
    emit settingsUpdated();
}

void InfoEditorSettingsFrame::onColorChanged(const QColor& c)
{
    mSettings.customSettings["color"] =
        Color(c.red(), c.green(), c.blue(), c.alpha());
    emit settingsUpdated();
}

} // namespace vcl::qt
