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

#include <vclib/qt/gui/mesh_render_settings_frame/cross_section_settings_frame.h>

#include "ui_cross_section_settings_frame.h"

namespace vcl::qt {

CrossSectionSettingsFrame::CrossSectionSettingsFrame(
    const CrossSectionSettings& css,
    QWidget*                    parent) :
        QFrame(parent), mUI(new Ui::CrossSectionSettingsFrame), mCSS(css)
{
    mUI->setupUi(this);

    mFloatSliders[X] = mUI->xFloatRangeSlider;
    mFloatSliders[Y] = mUI->yFloatRangeSlider;
    mFloatSliders[Z] = mUI->zFloatRangeSlider;

    mMinSpinBoxes[X] = mUI->xMinSpinBox;
    mMinSpinBoxes[Y] = mUI->yMinSpinBox;
    mMinSpinBoxes[Z] = mUI->zMinSpinBox;

    mMaxSpinBoxes[X] = mUI->xMaxSpinBox;
    mMaxSpinBoxes[Y] = mUI->yMaxSpinBox;
    mMaxSpinBoxes[Z] = mUI->zMaxSpinBox;

    connect(
        mUI->crossSectionEnabledCheckBox,
        SIGNAL(checkStateChanged(Qt::CheckState)),
        this,
        SLOT(onCrossSectionEnabledChanged(Qt::CheckState)));

    connect(
        mUI->perVertexRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onPerVertexToggled(bool)));

    connect(
        mUI->perFragmentRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onPerFragmentToggled(bool)));

    for (uint i = X; i < AXIS_COUNT; ++i) {
        connect(
            mFloatSliders[i],
            &FloatRangeSlider::lowerValueChanged,
            [this, i](float value) {
                onFloatRangeSliderLowerValueChanged(
                    static_cast<Axis>(i), value);
            });

        connect(
            mFloatSliders[i],
            &FloatRangeSlider::upperValueChanged,
            [this, i](float value) {
                onFloatRangeSliderUpperValueChanged(
                    static_cast<Axis>(i), value);
            });

        connect(
            mMinSpinBoxes[i],
            &QDoubleSpinBox::valueChanged,
            [this, i](double value) {
                onMinSpinBoxValueChanged(static_cast<Axis>(i), value);
            });

        connect(
            mMaxSpinBoxes[i],
            &QDoubleSpinBox::valueChanged,
            [this, i](double value) {
                onMaxSpinBoxValueChanged(static_cast<Axis>(i), value);
            });
    }

    updateFrameFromSettings();
}

CrossSectionSettingsFrame::CrossSectionSettingsFrame(QWidget* parent) :
        CrossSectionSettingsFrame(CrossSectionSettings(), parent)
{
}

CrossSectionSettingsFrame::~CrossSectionSettingsFrame()
{
    delete mUI;
}

const CrossSectionSettings& CrossSectionSettingsFrame::crossSectionSettings()
    const
{
    return mCSS;
}

void CrossSectionSettingsFrame::setCrossSectionSettings(
    const CrossSectionSettings& settings)
{
    mCSS = settings;
    updateFrameFromSettings();
}

void CrossSectionSettingsFrame::updateFrameFromSettings()
{
    using enum CrossSectionSettings::CrossSectionType;

    // checkbox
    mUI->crossSectionEnabledCheckBox->blockSignals(true);

    if (mCSS.type() == NONE) {
        mUI->crossSectionEnabledCheckBox->setCheckState(
            Qt::CheckState::Unchecked);
        mUI->controlsFrame->setEnabled(false);
    }
    else {
        mUI->crossSectionEnabledCheckBox->setCheckState(Qt::CheckState::Checked);
        mUI->controlsFrame->setEnabled(true);
    }

    mUI->crossSectionEnabledCheckBox->blockSignals(false);

    // radio buttons
    mUI->perVertexRadioButton->blockSignals(true);
    mUI->perFragmentRadioButton->blockSignals(true);

    if (mCSS.type() == PER_VERTEX)
        mUI->perVertexRadioButton->setChecked(true);
    else if (mCSS.type() == PER_FRAGMENT)
        mUI->perFragmentRadioButton->setChecked(true);

    mUI->perVertexRadioButton->blockSignals(false);
    mUI->perFragmentRadioButton->blockSignals(false);

    // sliders and spinboxes
    const auto& bb = mCSS.boundingBox();
    const auto& l  = mCSS.lower();
    const auto& u  = mCSS.upper();

    for (uint i = X; i < AXIS_COUNT; ++i) {
        updateSlider(
            mFloatSliders[i],
            mMinSpinBoxes[i],
            mMaxSpinBoxes[i],
            bb.min()[i],
            bb.max()[i],
            l[i],
            u[i]);
    }
}

void CrossSectionSettingsFrame::updateSlider(
    FloatRangeSlider* slider,
    QDoubleSpinBox*   minSpinBox,
    QDoubleSpinBox*   maxSpinBox,
    float             min,
    float             max,
    float             lower,
    float             upper)
{
    slider->blockSignals(true);

    slider->setRange(min, max);
    slider->setLowerValue(lower);
    slider->setUpperValue(upper);

    slider->blockSignals(false);

    minSpinBox->blockSignals(true);

    minSpinBox->setMinimum(min);
    minSpinBox->setMaximum(upper);
    minSpinBox->setValue(lower);

    minSpinBox->blockSignals(false);

    maxSpinBox->blockSignals(true);

    maxSpinBox->setMinimum(lower);
    maxSpinBox->setMaximum(max);
    maxSpinBox->setValue(upper);

    maxSpinBox->blockSignals(false);
}

void CrossSectionSettingsFrame::onCrossSectionEnabledChanged(
    Qt::CheckState arg1)
{
    using enum CrossSectionSettings::CrossSectionType;

    if (arg1 != Qt::CheckState::Checked) {
        mCSS.type() = NONE;
    }
    else {
        if (mUI->perVertexRadioButton->isChecked())
            mCSS.type() = PER_VERTEX;
        else
            mCSS.type() = PER_FRAGMENT;
    }

    mUI->controlsFrame->setEnabled(arg1 == Qt::CheckState::Checked);
    emit crossSectionSettingsUpdated();
}

void CrossSectionSettingsFrame::onPerVertexToggled(bool checked)
{
    using enum CrossSectionSettings::CrossSectionType;
    if (checked) {
        mCSS.type() = PER_VERTEX;
        emit crossSectionSettingsUpdated();
    }
}

void CrossSectionSettingsFrame::onPerFragmentToggled(bool checked)
{
    using enum CrossSectionSettings::CrossSectionType;
    if (checked) {
        mCSS.type() = PER_FRAGMENT;
        emit crossSectionSettingsUpdated();
    }
}

void CrossSectionSettingsFrame::onFloatRangeSliderLowerValueChanged(
    Axis  axis,
    float value)
{
    auto l  = mCSS.lower();
    l[axis] = value;
    mCSS.setLower(l);

    mMinSpinBoxes[axis]->blockSignals(true);
    mMinSpinBoxes[axis]->setValue(value);
    mMinSpinBoxes[axis]->blockSignals(false);
    mMaxSpinBoxes[axis]->setMinimum(value);

    emit crossSectionSettingsUpdated();
}

void CrossSectionSettingsFrame::onFloatRangeSliderUpperValueChanged(
    Axis  axis,
    float value)
{
    auto u  = mCSS.upper();
    u[axis] = value;
    mCSS.setUpper(u);

    mMaxSpinBoxes[axis]->blockSignals(true);
    mMaxSpinBoxes[axis]->setValue(value);
    mMaxSpinBoxes[axis]->blockSignals(false);
    mMinSpinBoxes[axis]->setMaximum(value);

    emit crossSectionSettingsUpdated();
}

void CrossSectionSettingsFrame::onMinSpinBoxValueChanged(
    Axis   axis,
    double value)
{
    mFloatSliders[axis]->setLowerValue(static_cast<float>(value));
}

void CrossSectionSettingsFrame::onMaxSpinBoxValueChanged(
    Axis   axis,
    double value)
{
    mFloatSliders[axis]->setUpperValue(static_cast<float>(value));
}

} // namespace vcl::qt
