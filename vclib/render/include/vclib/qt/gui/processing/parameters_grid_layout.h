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

#ifndef VCL_QT_GUI_PROCESSING_PARAMETERS_GRID_LAYOUT_H
#define VCL_QT_GUI_PROCESSING_PARAMETERS_GRID_LAYOUT_H

#include <QGridLayout>

#include <vclib/processing/parameter_vector.h>

#include "parameters/parameter_row.h"

namespace vcl::qt {

class ParametersGridLayout : public QGridLayout
{
    proc::ParameterVector mParameters;

    std::vector<std::shared_ptr<ParameterRow>> mRows;

public:
    ParametersGridLayout(QWidget* parent);

    void setParameters(const proc::ParameterVector& parameters);

    proc::ParameterVector parameters() const;

    void setHelpVisible(bool b);

private:
    void clear();

    static void clear(QLayout* layout);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_PROCESSING_PARAMETERS_GRID_LAYOUT_H
