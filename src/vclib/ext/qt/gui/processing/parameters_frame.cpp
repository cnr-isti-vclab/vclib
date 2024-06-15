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

#include "ui_parameters_frame.h"
#include <vclib/ext/qt/gui/processing/parameters_frame.h>

namespace vcl::qt {

ParametersFrame::ParametersFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::ParametersFrame)
{
    mUI->setupUi(this);
    mParamGridLayout = new ParametersGridLayout(this);
    mUI->parametersLayout->addLayout(mParamGridLayout, 0, 0);

    connect(
        mUI->helpRushButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(helpButtonClicked(bool)));
}

ParametersFrame::~ParametersFrame()
{
    delete mUI;
}

void ParametersFrame::setParameters(const proc::ParameterVector& parameters)
{
    setVisible(parameters.size() != 0);
    mParamGridLayout->setParameters(parameters);
}

proc::ParameterVector ParametersFrame::parameters() const
{
    return mParamGridLayout->parameters();
}

void ParametersFrame::helpButtonClicked(bool checked)
{
    mParamGridLayout->setHelpVisible(checked);
}

} // namespace vcl::qt
