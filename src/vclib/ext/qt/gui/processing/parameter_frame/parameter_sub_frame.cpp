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

#include "ui_parameter_sub_frame.h"
#include <vclib/ext/qt/gui/processing/parameter_frame/parameter_sub_frame.h>

namespace vcl::qt {

ParameterSubFrame::ParameterSubFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::ParameterSubFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->toolButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(showPushButtonClicked(bool)));
}

ParameterSubFrame::~ParameterSubFrame()
{
    delete mUI;
}

void ParameterSubFrame::setTitleLabel(const std::string& title)
{
    mUI->titleLabel->setText(QString::fromStdString(title));
}

void ParameterSubFrame::setSubFrameLayout(QGridLayout* layout)
{
    mUI->subFrame->setLayout(layout);
}

void ParameterSubFrame::setHeaderFrameVisible(bool visible)
{
    mUI->headerFrame->setVisible(visible);
}

void ParameterSubFrame::setHeaderButtonVisible(bool visible)
{
    mUI->toolButton->setVisible(visible);
}

void ParameterSubFrame::setHeaderButtonChecked(bool checked)
{
    mUI->toolButton->setChecked(checked);
    showPushButtonClicked(checked);
}

void ParameterSubFrame::showPushButtonClicked(bool checked)
{
    mUI->subFrame->setVisible(checked);
    if (checked)
        mUI->toolButton->setArrowType(Qt::ArrowType::DownArrow);
    else
        mUI->toolButton->setArrowType(Qt::ArrowType::RightArrow);
}

} // namespace vcl::qt
