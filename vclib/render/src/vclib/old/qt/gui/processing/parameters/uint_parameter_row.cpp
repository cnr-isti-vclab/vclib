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

#include <vclib/qt/gui/processing/parameters/uint_parameter_row.h>

#include <QIntValidator>

namespace vcl::qt {

UintParameterRow::UintParameterRow(const proc::UintParameter& param) :
        ParameterRow(param), mParam(param)
{
    mLineEdit = new QLineEdit();
    mLineEdit->setToolTip(param.tooltip().c_str());
    mLineEdit->setValidator(
        new QIntValidator(0, std::numeric_limits<int>::max(), mLineEdit));
    mLineEdit->setText(QString::number(param.uintValue()));
}

QWidget* UintParameterRow::parameterWidget()
{
    return mLineEdit;
}

std::shared_ptr<proc::Parameter> UintParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setUintValue(mLineEdit->text().toUInt());
    return p;
}

} // namespace vcl::qt
