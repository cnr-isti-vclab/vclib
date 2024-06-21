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

#ifndef VCL_EXT_QT_GUI_PROCESSING_PARAMETERS_UINT_PARAMETER_ROW_H
#define VCL_EXT_QT_GUI_PROCESSING_PARAMETERS_UINT_PARAMETER_ROW_H

#include <QLineEdit>

#include "parameter_row.h"

namespace vcl::qt {

class UintParameterRow : public ParameterRow
{
    proc::UintParameter mParam;

    QLineEdit* mLineEdit = nullptr;

public:
    UintParameterRow(const proc::UintParameter& param);

    // ParameterRow interface
    QWidget* parameterWidget() override;

    std::shared_ptr<proc::Parameter> parameterFromWidget() override;
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_PROCESSING_PARAMETERS_UINT_PARAMETER_ROW_H
