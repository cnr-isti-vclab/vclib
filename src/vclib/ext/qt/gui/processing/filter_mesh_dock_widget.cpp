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

#include <vclib/ext/qt/gui/processing/filter_mesh_dock_widget.h>
#include "ui_filter_mesh_dock_widget.h"

#include <QPushButton>

namespace vcl::qt {

FilterMeshDockWidget::FilterMeshDockWidget(
    const std::shared_ptr<proc::FilterMeshAction>& action,
    QWidget*                                       parent) :
        QDockWidget(parent),
        mUI(new Ui::FilterMeshDockWidget),
        mAction(action)
{
    mUI->setupUi(this);

    setWindowTitle(QString::fromStdString(action->name()));

    mUI->filterDescriptionLabel->setText(
        QString::fromStdString(action->description()));

    mUI->parameterFrame->setParameters(action->parameters());

    QPushButton* applyButton = mUI->buttonBox->button(QDialogButtonBox::Apply);
    QPushButton* cancelButton = mUI->buttonBox->button(QDialogButtonBox::Cancel);

    connect(
        applyButton,
        &QPushButton::clicked,
        this,
        &FilterMeshDockWidget::onApplyButtonClicked);

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &FilterMeshDockWidget::onCancelButtonClicked);
}

FilterMeshDockWidget::~FilterMeshDockWidget()
{
    delete mUI;
}

void FilterMeshDockWidget::onApplyButtonClicked()
{
    emit applyFilter(mAction, mUI->parameterFrame->parameters());
}

void FilterMeshDockWidget::onCancelButtonClicked()
{
    close();
}

} // namespace vcl::qt
