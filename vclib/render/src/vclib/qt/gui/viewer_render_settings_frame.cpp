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
#include <QStandardItemModel>

namespace vcl::qt {

namespace detail {

template<typename V>
const PBRViewerSettings& pbrSettings(const V* v)
{
    if constexpr (PBRViewerConcept<V>) {
        return v->pbrSettings();
    }
    else {
        static vcl::PBRViewerSettings sts;
        return sts;
    }
}

template<typename V>
void setPbrSettings(V* v, const PBRViewerSettings& settings)
{
    if constexpr (PBRViewerConcept<V>) {
        return v->setPbrSettings(settings);
    }
}

} // namespace detail

ViewerRenderSettingsFrame::ViewerRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::ViewerRenderSettingsFrame)
{
    mUI->setupUi(this);

    // by default, render settings is classic, and PBR settings frame is hidden
    mUI->pbrSettingsFrame->setVisible(false);

    mUI->renderModeLabel->setEnabled(false);
    mUI->renderModeComboBox->setEnabled(false);

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
}

ViewerRenderSettingsFrame::~ViewerRenderSettingsFrame()
{
    delete mUI;
}

void ViewerRenderSettingsFrame::setViewer(MeshViewerRenderApp* viewer)
{
    mViewer = viewer;

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
    }
}

void ViewerRenderSettingsFrame::setPbrSettings(
    const PBRViewerSettings& settings)
{
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

const PBRViewerSettings& ViewerRenderSettingsFrame::pbrSettings() const
{
    return detail::pbrSettings(mViewer);
}

template<typename V>
void setPBRModef(V* v, bool b)
{
    if constexpr (PBRViewerConcept<V>) {
        auto s    = v->pbrSettings();
        s.pbrMode = b;
        return v->setPbrSettings(s);
    }
}

void ViewerRenderSettingsFrame::renderModeComboBoxCurrentIndexChanged(int index)
{
    bool pbr = index == toUnderlying(RenderMode::PBR);

    setPBRModef(mViewer, pbr);
    mUI->pbrSettingsFrame->setVisible(pbr);

    mViewer->update();
}

void ViewerRenderSettingsFrame::exposureSpinBoxValueChanged(double value)
{
    auto sts = detail::pbrSettings(mViewer);

    sts.exposure = static_cast<float>(value);

    detail::setPbrSettings(mViewer, sts);
    mViewer->update();
}

void ViewerRenderSettingsFrame::toneMappingComboBoxCurrentIndexChanged(
    int index)
{
    auto sts = detail::pbrSettings(mViewer);

    sts.toneMapping = static_cast<PBRViewerSettings::ToneMapping>(index);

    detail::setPbrSettings(mViewer, sts);
    mViewer->update();
}

} // namespace vcl::qt
