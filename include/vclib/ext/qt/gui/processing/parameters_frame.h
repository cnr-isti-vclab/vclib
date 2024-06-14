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

#ifndef VCL_EXT_QT_GUI_PROCESSING_PARAMETERS_FRAME_H
#define VCL_EXT_QT_GUI_PROCESSING_PARAMETERS_FRAME_H

#include <QFrame>

#include "parameters_grid_layout.h"

namespace vcl::qt {

namespace Ui {
class ParametersFrame;
} // namespace Ui

class ParametersFrame : public QFrame
{
    Q_OBJECT

    Ui::ParametersFrame* mUI;
    ParametersGridLayout* mParamGridLayout = nullptr;

public:
    explicit ParametersFrame(QWidget* parent = nullptr);
    ~ParametersFrame();

    void setParameters(const proc::ParameterVector& parameters);

    proc::ParameterVector parameters() const;

private slots:
    void helpButtonClicked(bool checked);
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_PROCESSING_PARAMETERS_FRAME_H
