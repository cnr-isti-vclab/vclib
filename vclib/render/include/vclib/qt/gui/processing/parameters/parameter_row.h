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

#ifndef VCL_QT_GUI_PROCESSING_PARAMETERS_PARAMETER_ROW_H
#define VCL_QT_GUI_PROCESSING_PARAMETERS_PARAMETER_ROW_H

#include <memory>

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include <vclib/processing/parameters.h>
#include <vclib/qt/gui/q_clickable_label.h>

namespace vcl::qt {

class ParameterRow
{
    QLabel* mHelpLabel;

    bool mVisible;
    bool mHelpVisible;

    bool mHasBeenModified;

protected:
    QClickableLabel* mDescriptionLabel;

public:
    ParameterRow(const proc::Parameter& param);

    virtual ~ParameterRow();

    virtual QWidget* parameterWidget() = 0;

    virtual std::shared_ptr<proc::Parameter> parameterFromWidget() const = 0;

    void addRowToGridLayout(QGridLayout* lay, const int row);

    bool hasBeenModified() const;

    void setVisible(bool b);

    void setHelpVisible(bool b);

protected:
    void setModified(bool b);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_PROCESSING_PARAMETERS_PARAMETER_ROW_H
