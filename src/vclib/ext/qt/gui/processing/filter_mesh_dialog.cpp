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

#include <vclib/ext/qt/gui/processing/filter_mesh_dialog.h>
#include "ui_filter_mesh_dialog.h"

namespace vcl::qt {

FilterMeshDialog::FilterMeshDialog(
    const std::shared_ptr<proc::FilterMeshAction>& action,
    QWidget*                                       parent) :
        QDialog(parent),
        mUI(new Ui::FilterMeshDialog)
{
    mUI->setupUi(this);

    setWindowTitle(QString::fromStdString(action->name()));

    mUI->filterDescriptionLabel->setText(
        QString::fromStdString(action->description()));

    mUI->parameterFrame->setParameters(action->parameters());
}

FilterMeshDialog::~FilterMeshDialog()
{
    delete mUI;
}

} // namespace vcl::qt
