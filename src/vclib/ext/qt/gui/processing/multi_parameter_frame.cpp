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

#include <vclib/ext/qt/gui/processing/parameter_frame/parameter_sub_frame.h>

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

    connect(
        mUI->showAllParametersToolButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(showAllParametersButtonClicked(bool)));
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
    mParamGrids[i]->parentWidget()->parentWidget()->setVisible(visible);
}

proc::ParameterVector MultiParameterFrame::parameters(uint i) const
{
    return mParamGrids.at(i)->parameters();
}

void MultiParameterFrame::setHeaderLabel(const std::string& label)
{
    mUI->headerLabel->setText(QString::fromStdString(label));
}

void MultiParameterFrame::setHeaderFrameVisible(bool visible)
{
    mUI->headerFrame->setVisible(visible);
}

void MultiParameterFrame::setShowAllParametersButtonChecked(bool checked)
{
    mUI->showAllParametersToolButton->setChecked(checked);
    showAllParametersButtonClicked(checked);
}

void MultiParameterFrame::showAllParametersButtonClicked(bool checked)
{
    for (uint i = 0; i < mParamGrids.size(); ++i)
        setFrameVisible(i, checked);
    mUI->resetAllPushButton->setVisible(checked);
    mUI->helpPushButton->setVisible(checked);
    if (checked)
        mUI->showAllParametersToolButton->setArrowType(
            Qt::ArrowType::DownArrow);
    else
        mUI->showAllParametersToolButton->setArrowType(
            Qt::ArrowType::RightArrow);
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
    ParameterSubFrame* frame = new ParameterSubFrame(this);

    frame->setTitleLabel(name);
    frame->setSubFrameLayout(layout);

    mUI->parametersLayout->addWidget(frame);
    mParamGrids.push_back(layout);
    return mParamGrids.size() - 1;
}

} // namespace vcl::qt
