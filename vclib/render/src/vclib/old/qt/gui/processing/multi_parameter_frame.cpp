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

#include <vclib/qt/gui/processing/multi_parameter_frame.h>

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
        mUI->headerToolButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(showAllParametersButtonClicked(bool)));
}

MultiParameterFrame::~MultiParameterFrame()
{
    delete mUI;
}

uint MultiParameterFrame::addSubFrame(
    const std::string&           name,
    const proc::ParameterVector& parameters)
{
    ParametersGridLayout* layout = new ParametersGridLayout(this);
    layout->setParameters(parameters);
    return addSubFrameLayout(name, layout);
}

void MultiParameterFrame::setSubFrameName(uint i, const std::string& name)
{
    subFrame(i)->setTitleLabel(name);
}

void MultiParameterFrame::setSubFramePatameters(
    uint                         i,
    const proc::ParameterVector& parameters)
{
    ParametersGridLayout* layout = new ParametersGridLayout(this);
    layout->setParameters(parameters);
    setSubFrameLayout(i, layout);
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

void MultiParameterFrame::setHeaderButtonVisible(bool visible)
{
    mUI->headerToolButton->setVisible(visible);
}

void MultiParameterFrame::setHeaderButtonChecked(bool checked)
{
    mUI->headerToolButton->setChecked(checked);
    showAllParametersButtonClicked(checked);
}

void MultiParameterFrame::setSubFrameVisible(uint i, bool visible)
{
    subFrame(i)->setVisible(visible);
}

void MultiParameterFrame::setSubFrameHeaderVisible(uint i, bool visible)
{
    subFrame(i)->setHeaderFrameVisible(visible);
}

void MultiParameterFrame::setSubFrameHeaderButtonVisible(uint i, bool visible)
{
    subFrame(i)->setHeaderButtonVisible(visible);
}

void MultiParameterFrame::setSubFrameHeaderButtonChecked(uint i, bool checked)
{
    subFrame(i)->setHeaderButtonChecked(checked);
}

void MultiParameterFrame::showAllParametersButtonClicked(bool checked)
{
    mUI->parametersFrame->setVisible(checked);
    mUI->resetAllPushButton->setVisible(checked);
    mUI->helpPushButton->setVisible(checked);
    if (checked)
        mUI->headerToolButton->setArrowType(Qt::ArrowType::DownArrow);
    else
        mUI->headerToolButton->setArrowType(Qt::ArrowType::RightArrow);
}

void MultiParameterFrame::helpButtonClicked(bool checked)
{
    for (auto* grid : mParamGrids)
        grid->setHelpVisible(checked);
}

uint MultiParameterFrame::addSubFrameLayout(
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

void MultiParameterFrame::setSubFrameLayout(
    uint                  i,
    ParametersGridLayout* layout)
{
    subFrame(i)->setSubFrameLayout(layout);
    mParamGrids[i] = layout;
}

ParameterSubFrame* MultiParameterFrame::subFrame(uint i)
{
    return static_cast<ParameterSubFrame*>(
        mParamGrids[i]->parentWidget()->parentWidget());
}

} // namespace vcl::qt
