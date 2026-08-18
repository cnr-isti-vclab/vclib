// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/viewer_settings_frame.h>

#include "ui_viewer_settings_frame.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardItemModel>

namespace vcl::qt {

ViewerSettingsFrame::ViewerSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::ViewerSettingsFrame)
{
    mUI->setupUi(this);

    mUI->renderModeComboBox->addItem("Classic");
    mUI->renderModeComboBox->addItem("PBR");

    using enum ViewerSettings::ToneMapping;

    for (uint i = 0; i < toUnderlying(COUNT); ++i) {
        mUI->toneMappingComboBox->addItem(
            ViewerSettings::TONE_MAPPING_STRINGS[i]);
    }

    connect(
        mUI->renderModeComboBox,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(renderModeComboBoxCurrentIndexChanged(int)));

    connect(
        mUI->exposureSpinBox,
        SIGNAL(valueChanged(double)),
        this,
        SLOT(exposureSpinBoxValueChanged(double)));

    connect(
        mUI->toneMappingComboBox,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(toneMappingComboBoxCurrentIndexChanged(int)));

    connect(
        mUI->iblCheckBox,
        SIGNAL(checkStateChanged(Qt::CheckState)),
        this,
        SLOT(iblCheckBoxCheckStateChanged(Qt::CheckState)));

    connect(
        mUI->drawBackgroundPanoramaCheckBox,
        SIGNAL(checkStateChanged(Qt::CheckState)),
        this,
        SLOT(drawBackgroundPanoramaCheckBoxCheckStateChanged(Qt::CheckState)));

    connect(
        mUI->loadPanoramaPushButton,
        SIGNAL(clicked()),
        this,
        SLOT(loadPanoramaPushButtonClicked()));

    connect(
        mUI->resetDefaultButton,
        SIGNAL(clicked()),
        this,
        SLOT(onResetDefaultClicked()));
}

ViewerSettingsFrame::~ViewerSettingsFrame()
{
    delete mUI;
}

void ViewerSettingsFrame::setViewerSettings(const ViewerSettings& settings)
{
    mSettings = settings;

    bool blocked = mUI->renderModeComboBox->blockSignals(true);
    if (mSettings.renderMode == RenderMode::CLASSIC) {
        mUI->renderModeComboBox->setCurrentIndex(0);
    }
    else if (mSettings.renderMode == RenderMode::PBR) {
        mUI->renderModeComboBox->setCurrentIndex(1);
    }
    mUI->renderModeComboBox->blockSignals(blocked);

    blocked = mUI->exposureSpinBox->blockSignals(true);
    mUI->exposureSpinBox->setValue(mSettings.exposure);
    mUI->exposureSpinBox->blockSignals(blocked);

    blocked = mUI->iblCheckBox->blockSignals(true);
    mUI->iblCheckBox->setChecked(mSettings.imageBasedLighting);
    mUI->iblCheckBox->blockSignals(blocked);

    blocked = mUI->drawBackgroundPanoramaCheckBox->blockSignals(true);
    mUI->drawBackgroundPanoramaCheckBox->setChecked(
        mSettings.renderBackgroundPanorama);
    mUI->drawBackgroundPanoramaCheckBox->blockSignals(blocked);

    blocked = mUI->toneMappingComboBox->blockSignals(true);
    mUI->toneMappingComboBox->setCurrentIndex(
        toUnderlying(mSettings.toneMapping));
    mUI->toneMappingComboBox->blockSignals(blocked);

    mUI->pbrSettingsFrame->setVisible(true);
    updatePanoramaLabel();
}

const ViewerSettings& ViewerSettingsFrame::viewerSettings() const
{
    return mSettings;
}

void ViewerSettingsFrame::updatePanoramaLabel()
{
    bool hasPanorama = !mSettings.panoramaPath.empty();

    if (hasPanorama) {
        QString   fullPath = QString::fromStdString(mSettings.panoramaPath);
        QFileInfo fileInfo(fullPath);
        mUI->panoramaLabel->setText(
            QString("Panorama: ") + fileInfo.fileName());
        mUI->panoramaLabel->setToolTip(fullPath);
    }
    else {
        mUI->panoramaLabel->setText("Panorama: None");
        mUI->panoramaLabel->setToolTip("");
    }

    mUI->iblCheckBox->setEnabled(hasPanorama);
    mUI->drawBackgroundPanoramaCheckBox->setEnabled(hasPanorama);
}

void ViewerSettingsFrame::renderModeComboBoxCurrentIndexChanged(int index)
{
    mSettings.renderMode = (index == 0) ? RenderMode::CLASSIC : RenderMode::PBR;
    emit settingsChanged(mSettings);
}

void ViewerSettingsFrame::exposureSpinBoxValueChanged(double value)
{
    mSettings.exposure = static_cast<float>(value);
    emit settingsChanged(mSettings);
}

void ViewerSettingsFrame::toneMappingComboBoxCurrentIndexChanged(int index)
{
    mSettings.toneMapping = static_cast<ViewerSettings::ToneMapping>(index);
    emit settingsChanged(mSettings);
}

void ViewerSettingsFrame::iblCheckBoxCheckStateChanged(Qt::CheckState state)
{
    mSettings.imageBasedLighting = (state == Qt::Checked);
    emit settingsChanged(mSettings);
}

void ViewerSettingsFrame::drawBackgroundPanoramaCheckBoxCheckStateChanged(
    Qt::CheckState state)
{
    mSettings.renderBackgroundPanorama = (state == Qt::Checked);
    emit settingsChanged(mSettings);
}

void ViewerSettingsFrame::loadPanoramaPushButtonClicked()
{
    // open a file dialog asking for a *.hdr file
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Load Panorama"),
        "",
        tr("HDR Images (*.hdr *.exr *.ktx *.dds)"));

    if (!fileName.isEmpty()) {
        mSettings.panoramaPath = fileName.toStdString();
        updatePanoramaLabel();
        emit settingsChanged(mSettings);
    }
}

void ViewerSettingsFrame::onResetDefaultClicked()
{
    mSettings.resetDefaults();
    setViewerSettings(mSettings);
    emit settingsChanged(mSettings);
}

} // namespace vcl::qt
