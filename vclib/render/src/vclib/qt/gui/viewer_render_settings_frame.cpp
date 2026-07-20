// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/viewer_render_settings_frame.h>

#include "ui_viewer_render_settings_frame.h"

#include <vclib/render/concepts/viewer.h>

#include <QColorDialog>
#include <QFileDialog>
#include <QStandardItemModel>

#define checkPtr(p) \
    if (!(p)) {     \
        return;     \
    }

namespace vcl::qt {

namespace detail {

template<typename V>
const ViewerSettings& viewerSettings(const V* v)
{
    static vcl::ViewerSettings sts;
    if constexpr (ViewerConcept<V>) {
        if (v)
            return v->viewerSettings();
        else
            return sts;
    }
    else {
        return sts;
    }
}

template<typename V>
void setViewerSettings(V* v, const ViewerSettings& settings)
{
    if constexpr (ViewerConcept<V>) {
        v->setViewerSettings(settings);
    }
}

template<typename V>
std::string panoramaFileName(V* v)
{
    if constexpr (ViewerConcept<V>) {
        return v->panoramaFileName();
    }
    else {
        return std::string();
    }
}

template<typename V>
void setPanorama(V* v, const std::string& panorama)
{
    if constexpr (ViewerConcept<V>) {
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

    if constexpr (ViewerConcept<MeshViewerRenderApp>) {
        using enum RenderMode;

        auto settings = detail::viewerSettings(mViewer);

        mUI->pbrSettingsFrame->setVisible(settings.renderMode == RenderMode::PBR);

        mUI->renderModeLabel->setEnabled(true);
        mUI->renderModeComboBox->setEnabled(true);

        mUI->renderModeComboBox->setCurrentIndex(toUnderlying(settings.renderMode));
        mUI->exposureSpinBox->setValue(settings.exposure);
        mUI->iblCheckBox->setChecked(settings.imageBasedLighting);
        mUI->drawBackgroundPanoramaCheckBox->setChecked(
            settings.renderBackgroundPanorama);

        mUI->toneMappingComboBox->setCurrentIndex(
            toUnderlying(settings.toneMapping));

        updatePanoramaLabel();
    }
}

void ViewerRenderSettingsFrame::setViewerSettings(
    const ViewerSettings& settings)
{
    checkPtr(mViewer);

    if constexpr (ViewerConcept<MeshViewerRenderApp>) {
        using enum RenderMode;
        mUI->renderModeComboBox->setCurrentIndex(toUnderlying(settings.renderMode));
        mUI->exposureSpinBox->setValue(settings.exposure);
        mUI->iblCheckBox->setChecked(settings.imageBasedLighting);
        mUI->drawBackgroundPanoramaCheckBox->setChecked(
            settings.renderBackgroundPanorama);
        detail::setViewerSettings(mViewer, settings);
    }
}

void ViewerRenderSettingsFrame::setPanorama(const std::string& panorama)
{
    checkPtr(mViewer);

    detail::setPanorama(mViewer, panorama);

    updatePanoramaLabel();
}

const ViewerSettings& ViewerRenderSettingsFrame::viewerSettings() const
{
    return detail::viewerSettings(mViewer);
}

void ViewerRenderSettingsFrame::disableForm()
{
    mUI->pbrSettingsFrame->setVisible(false);

    mUI->renderModeLabel->setEnabled(false);
    mUI->renderModeComboBox->setCurrentIndex(0);
    mUI->renderModeComboBox->setEnabled(false);
}

void ViewerRenderSettingsFrame::updatePanoramaLabel()
{
    checkPtr(mViewer);

    std::string panoramaFileName = detail::panoramaFileName(mViewer);
    if (!panoramaFileName.empty()) {
        mUI->panoramaLabel->setText(
            QString("Panorama: ") + panoramaFileName.c_str());
    }
    else {
        mUI->panoramaLabel->setText("Panorama: None");
    }
}

void ViewerRenderSettingsFrame::renderModeComboBoxCurrentIndexChanged(int index)
{
    checkPtr(mViewer);

    RenderMode mode = static_cast<RenderMode>(index);

    auto sts    = detail::viewerSettings(mViewer);
    sts.renderMode = mode;
    detail::setViewerSettings(mViewer, sts);

    mUI->pbrSettingsFrame->setVisible(mode == RenderMode::PBR);

    mViewer->update();
}

void ViewerRenderSettingsFrame::exposureSpinBoxValueChanged(double value)
{
    checkPtr(mViewer);

    auto sts = detail::viewerSettings(mViewer);

    sts.exposure = static_cast<float>(value);

    detail::setViewerSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::toneMappingComboBoxCurrentIndexChanged(
    int index)
{
    checkPtr(mViewer);

    auto sts = detail::viewerSettings(mViewer);

    sts.toneMapping = static_cast<ViewerSettings::ToneMapping>(index);

    detail::setViewerSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::iblCheckBoxCheckStateChanged(
    Qt::CheckState state)
{
    checkPtr(mViewer);

    auto sts = detail::viewerSettings(mViewer);

    sts.imageBasedLighting = (state == Qt::Checked);

    detail::setViewerSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::drawBackgroundPanoramaCheckBoxCheckStateChanged(
    Qt::CheckState state)
{
    checkPtr(mViewer);

    auto sts = detail::viewerSettings(mViewer);

    sts.renderBackgroundPanorama = (state == Qt::Checked);

    detail::setViewerSettings(mViewer, sts);
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
