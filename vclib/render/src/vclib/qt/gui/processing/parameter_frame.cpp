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

#include <vclib/qt/gui/processing/parameter_frame.h>

namespace vcl::qt {

ParameterFrame::ParameterFrame(QWidget* parent) : MultiParameterFrame(parent)
{
    setHeaderLabel("Parameters:");
    setHeaderFrameVisible(true);
    setHeaderButtonChecked(true);
    setHeaderButtonVisible(false);
}

ParameterFrame::ParameterFrame(
    const proc::ParameterVector& parameters,
    QWidget*                     parent) :
        ParameterFrame(parent)
{
    setParameters(parameters);
}

proc::ParameterVector ParameterFrame::parameters() const
{
    return MultiParameterFrame::parameters(0);
}

void ParameterFrame::setParameters(const proc::ParameterVector& parameters)
{
    if (subFramesNumber() == 0) {
        addSubFrame("", parameters);

        setSubFrameHeaderVisible(0, false);
        setSubFrameVisible(0, true);
    }
    else {
        setSubFramePatameters(0, parameters);
    }
}

} // namespace vcl::qt
