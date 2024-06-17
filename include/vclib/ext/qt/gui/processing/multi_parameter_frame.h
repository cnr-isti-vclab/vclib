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

#include "parameter_frame/parameter_sub_frame.h"

namespace vcl::qt {

namespace Ui {
class MultiParameterFrame;
} // namespace Ui

class MultiParameterFrame : public QFrame
{
    Q_OBJECT

    Ui::MultiParameterFrame* mUI;

    std::vector<ParametersGridLayout*> mParamGrids;

public:
    explicit MultiParameterFrame(QWidget* parent = nullptr);
    ~MultiParameterFrame();

    uint addSubFrame(
        const std::string&           name,
        const proc::ParameterVector& parameters);

    proc::ParameterVector parameters(uint i) const;

    uint subFramesNumber() const { return mParamGrids.size(); }

    void setHeaderLabel(const std::string& label);

    void setHeaderFrameVisible(bool visible);

    void setHeaderButtonChecked(bool checked);

    void setSubFrameVisible(uint i, bool visible);

    void setSubFrameHeaderVisible(uint i, bool visible);

    void setSubFrameHeaderButtonVisible(uint i, bool visible);

    void setSubFrameHeaderButtonChecked(uint i, bool checked);

private slots:
    void showAllParametersButtonClicked(bool checked);

    void helpButtonClicked(bool checked);

private:
    uint addSubFrame(const std::string& name, ParametersGridLayout* layout);

    ParameterSubFrame* subFrame(uint i);
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_PROCESSING_MULTI_PARAMETER_FRAME_H
