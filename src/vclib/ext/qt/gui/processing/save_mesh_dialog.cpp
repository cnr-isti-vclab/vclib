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

#include <vclib/ext/qt/gui/processing/save_mesh_dialog.h>

#include <QGridLayout>
#include <QLabel>

namespace vcl::qt {

SaveMeshDialog::SaveMeshDialog(
    proc::ActionManager& actionManager,
    const QString&       caption,
    const QString&       directory,
    const QString&       filter,
    QWidget*             parent) :
        QFileDialog(parent, caption, directory, filter),
        mActionManager(&actionManager)
{
    setOption(QFileDialog::DontUseNativeDialog);
    setAcceptMode(QFileDialog::AcceptSave);
    QGridLayout* layout = qobject_cast<QGridLayout*>(this->layout());

    QWidget* w20 = layout->itemAtPosition(2, 0)->widget();
    QWidget* w21 = layout->itemAtPosition(2, 1)->widget();
    QWidget* w22 = layout->itemAtPosition(2, 2)->widget();
    QWidget* w30 = layout->itemAtPosition(3, 0)->widget();
    QWidget* w31 = layout->itemAtPosition(3, 1)->widget();

    layout->removeWidget(w20);
    layout->removeWidget(w21);
    layout->removeWidget(w22);
    layout->removeWidget(w30);
    layout->removeWidget(w31);

    mParameterFrame = new ParametersFrame(this);

    mParameterFrame->setParameters(
        actionManager.saveMeshAction("ply")->parameters());

    layout->addWidget(mParameterFrame, 2, 0, 1, 3);

    uint startingRow = 3;
    layout->addWidget(w20, 3, 0);
    layout->addWidget(w21, 3, 1);
    layout->addWidget(w22, 3, 2, 2, 1);
    layout->addWidget(w30, 4, 0);
    layout->addWidget(w31, 4, 1);
}

SaveMeshDialog::~SaveMeshDialog()
{
}

proc::ParameterVector SaveMeshDialog::parameters() const
{
    return mParameterFrame->parameters();
}

} // namespace vcl::qt
