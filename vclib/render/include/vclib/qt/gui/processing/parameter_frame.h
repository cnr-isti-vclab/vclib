/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_QT_GUI_PROCESSING_PARAMETER_FRAME_H
#define VCL_QT_GUI_PROCESSING_PARAMETER_FRAME_H

#include "multi_parameter_frame.h"

namespace vcl::qt {

class ParameterFrame : public MultiParameterFrame
{
    using MultiParameterFrame::addSubFrame;
    using MultiParameterFrame::setHeaderButtonChecked;
    using MultiParameterFrame::setHeaderButtonVisible;
    using MultiParameterFrame::setHeaderFrameVisible;
    using MultiParameterFrame::setHeaderLabel;
    using MultiParameterFrame::setSubFrameHeaderButtonChecked;
    using MultiParameterFrame::setSubFrameHeaderButtonVisible;
    using MultiParameterFrame::setSubFrameHeaderVisible;
    using MultiParameterFrame::setSubFrameName;
    using MultiParameterFrame::setSubFramePatameters;
    using MultiParameterFrame::setSubFrameVisible;
    using MultiParameterFrame::subFramesNumber;

public:
    explicit ParameterFrame(QWidget* parent = nullptr);

    explicit ParameterFrame(
        const proc::ParameterVector& parameters,
        QWidget*                     parent = nullptr);

    proc::ParameterVector parameters() const;

    void setParameters(const proc::ParameterVector& parameters);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_PROCESSING_PARAMETER_FRAME_H
