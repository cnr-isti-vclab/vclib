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

#include <vclib/qt/gui/viewer_render_settings_frame.h>

#include "ui_viewer_render_settings_frame.h"

#include <vclib/render/concepts/pbr_viewer.h>

#include <QColorDialog>
#include <QFileDialog>
#include <QStandardItemModel>

#define checkPtr(p) \
    if (!(p)) {        \
        return;        \
    }

namespace vcl::qt {

namespace detail {

template<typename V>
const PBRViewerSettings& pbrSettings(const V* v)
{
    static vcl::PBRViewerSettings sts;
    if constexpr (PBRViewerConcept<V>) {
        if (v)
            return v->pbrSettings();
        else
            return sts;
    }
    else {
        return sts;
    }
}

template<typename V>
void setPbrSettings(V* v, const PBRViewerSettings& settings)
{
    if constexpr (PBRViewerConcept<V>) {
        v->setPbrSettings(settings);
    }
}

template<typename V>
std::string panoramaFileName(V* v)
{
    if constexpr (PBRViewerConcept<V>) {
        return v->panoramaFileName();
    }
    else {
        return std::string();
    }
}

template<typename V>
void setPanorama(V* v, const std::string& panorama)
{
    if constexpr (PBRViewerConcept<V>) {
        v->setPanorama(panorama);
    }
}

} // namespace detail

ViewerRenderSettingsFrame::ViewerRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::ViewerRenderSettingsFrame)
{
    mUI->setupUi(this);

    // by default, render settings is classic, and PBR settings frame is hidden
    disableForm();

    using enum PBRViewerSettings::ToneMapping;

    for (uint i = 0; i < toUnderlying(COUNT); ++i) {
        mUI->toneMappingComboBox->addItem(
            PBRViewerSettings::TONE_MAPPING_STRINGS[i]);
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
}

ViewerRenderSettingsFrame::~ViewerRenderSettingsFrame()
{
    delete mUI;
}

void ViewerRenderSettingsFrame::setViewer(MeshViewerRenderApp* viewer)
{
    mViewer = viewer;

    if (!mViewer) {
        disableForm();
        return;
    }

    if constexpr (PBRViewerConcept<MeshViewerRenderApp>) {
        using enum RenderMode;

        auto settings = detail::pbrSettings(mViewer);

        mUI->pbrSettingsFrame->setVisible(settings.pbrMode);

        mUI->renderModeLabel->setEnabled(true);
        mUI->renderModeComboBox->setEnabled(true);

        mUI->renderModeComboBox->setCurrentIndex(
            settings.pbrMode ? toUnderlying(PBR) : toUnderlying(CLASSIC));
        mUI->exposureSpinBox->setValue(settings.exposure);
        mUI->iblCheckBox->setChecked(settings.imageBasedLighting);
        mUI->drawBackgroundPanoramaCheckBox->setChecked(
            settings.renderBackgroundPanorama);

        mUI->toneMappingComboBox->setCurrentIndex(
            toUnderlying(settings.toneMapping));

        updatePanormaLabel();
    }
}

void ViewerRenderSettingsFrame::setPbrSettings(
    const PBRViewerSettings& settings)
{
    checkPtr(mViewer);

    if constexpr (PBRViewerConcept<MeshViewerRenderApp>) {
        using enum RenderMode;
        mUI->renderModeComboBox->setCurrentIndex(
            settings.pbrMode ? toUnderlying(PBR) : toUnderlying(CLASSIC));
        mUI->exposureSpinBox->setValue(settings.exposure);
        mUI->iblCheckBox->setChecked(settings.imageBasedLighting);
        mUI->drawBackgroundPanoramaCheckBox->setChecked(
            settings.renderBackgroundPanorama);
        detail::setPbrSettings(mViewer, settings);
    }
}

void ViewerRenderSettingsFrame::setPanorama(const std::string& panorama)
{
    checkPtr(mViewer);

    detail::setPanorama(mViewer, panorama);

    updatePanormaLabel();
}

const PBRViewerSettings& ViewerRenderSettingsFrame::pbrSettings() const
{
    return detail::pbrSettings(mViewer);
}

void ViewerRenderSettingsFrame::disableForm()
{
    mUI->pbrSettingsFrame->setVisible(false);

    mUI->renderModeLabel->setEnabled(false);
    mUI->renderModeComboBox->setCurrentIndex(0);
    mUI->renderModeComboBox->setEnabled(false);
}

void ViewerRenderSettingsFrame::updatePanormaLabel()
{
    checkPtr(mViewer);

    std::string panoramaFileName = detail::panoramaFileName(mViewer);
    if (!panoramaFileName.empty()) {
        mUI->panormalaLabel->setText(
            QString("Panorma: ") + panoramaFileName.c_str());
    }
    else {
        mUI->panormalaLabel->setText("Panorma: None");
    }
}

void ViewerRenderSettingsFrame::renderModeComboBoxCurrentIndexChanged(int index)
{
    checkPtr(mViewer);

    bool pbr = index == toUnderlying(RenderMode::PBR);

    auto sts = detail::pbrSettings(mViewer);
    sts.pbrMode = pbr;
    detail::setPbrSettings(mViewer, sts);

    mUI->pbrSettingsFrame->setVisible(pbr);

    mViewer->update();
}

void ViewerRenderSettingsFrame::exposureSpinBoxValueChanged(double value)
{
    checkPtr(mViewer);

    auto sts = detail::pbrSettings(mViewer);

    sts.exposure = static_cast<float>(value);

    detail::setPbrSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::toneMappingComboBoxCurrentIndexChanged(
    int index)
{
    checkPtr(mViewer);

    auto sts = detail::pbrSettings(mViewer);

    sts.toneMapping = static_cast<PBRViewerSettings::ToneMapping>(index);

    detail::setPbrSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::iblCheckBoxCheckStateChanged(
    Qt::CheckState state)
{
    checkPtr(mViewer);

    auto sts = detail::pbrSettings(mViewer);

    sts.imageBasedLighting = (state == Qt::Checked);

    detail::setPbrSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::drawBackgroundPanoramaCheckBoxCheckStateChanged(
    Qt::CheckState state)
{
    checkPtr(mViewer);

    auto sts = detail::pbrSettings(mViewer);

    sts.renderBackgroundPanorama = (state == Qt::Checked);

    detail::setPbrSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::loadPanoramaPushButtonClicked()
{
    // open a file dialog asking for a *.hdr file
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Load Panorama"),
        "",
        tr("HDR Images (*.hdr *.exr *.ktx *.dds)"));

    if (!fileName.isEmpty()) {
        setPanorama(fileName.toStdString());
    }
}

} // namespace vcl::qt
