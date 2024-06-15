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

#include "ui_multi_parameter_frame.h"

#include <vclib/ext/qt/gui/processing/multi_parameter_frame.h>

namespace vcl::qt {

MultiParameterFrame::MultiParameterFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::MultiParameterFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->helpPushButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(helpButtonClicked(bool)));
}

MultiParameterFrame::~MultiParameterFrame()
{
    delete mUI;
}

uint MultiParameterFrame::addParameters(
    const std::string&           name,
    const proc::ParameterVector& parameters)
{
    ParametersGridLayout* layout = new ParametersGridLayout(this);
    layout->setParameters(parameters);
    return addLayout(name, layout);
}

void MultiParameterFrame::setFrameVisible(uint i, bool visible)
{
    mParamGrids[i]->parentWidget()->setVisible(visible);
}

proc::ParameterVector MultiParameterFrame::parameters(uint i) const
{
    return mParamGrids.at(i)->parameters();
}

void MultiParameterFrame::helpButtonClicked(bool checked)
{
    for (auto* grid : mParamGrids)
        grid->setHelpVisible(checked);
}

uint MultiParameterFrame::addLayout(
    const std::string&    name,
    ParametersGridLayout* layout)
{
    QFrame* frame = new QFrame(this);
    frame->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    QGridLayout* grid = new QGridLayout();
    grid->setContentsMargins(0, 0, 0, 0);
    auto* l = new QLabel(this);
    l->setText(name.c_str());
    grid->addWidget(l, 0, 0);

    grid->addLayout(layout, 1, 0);
    frame->setLayout(grid);

    mUI->parametersLayout->addWidget(frame);
    mParamGrids.push_back(layout);
    return mParamGrids.size() - 1;
}

} // namespace vcl::qt
