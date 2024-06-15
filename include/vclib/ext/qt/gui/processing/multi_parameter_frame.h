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

#ifndef VCL_EXT_QT_GUI_PROCESSING_MULTI_PARAMETER_FRAME_H
#define VCL_EXT_QT_GUI_PROCESSING_MULTI_PARAMETER_FRAME_H

#include <QFrame>

#include "parameters_grid_layout.h"

namespace vcl::qt {

namespace Ui {
class MultiParameterFrame;
} // namespace Ui

class MultiParameterFrame : public QFrame
{
    Q_OBJECT

    Ui::MultiParameterFrame*  mUI;

    std::vector<ParametersGridLayout*> mParamGrids;

public:
    explicit MultiParameterFrame(QWidget* parent = nullptr);
    ~MultiParameterFrame();

    uint addParameters(
        const std::string&           name,
        const proc::ParameterVector& parameters);

    void setFrameVisible(uint i, bool visible);

    proc::ParameterVector parameters(uint i) const;

    uint numberParameters() const { return mParamGrids.size(); }

private slots:
    void helpButtonClicked(bool checked);

private:
    uint addLayout(const std::string& name, ParametersGridLayout* layout);
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_PROCESSING_MULTI_PARAMETER_FRAME_H
