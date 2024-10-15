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

#include <vclib/qt/gui/processing/parameters/scalar_parameter_row.h>

#include <QDoubleValidator>

namespace vcl::qt {

ScalarParameterRow::ScalarParameterRow(const proc::ScalarParameter& param) :
        ParameterRow(param), mParam(param)
{
    mLineEdit = new QLineEdit();
    mLineEdit->setToolTip(param.tooltip().c_str());
    mLineEdit->setValidator(new QDoubleValidator(
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::max(),
        4,
        mLineEdit));
    mLineEdit->setText(QString::number(param.scalarValue()));
}

QWidget* ScalarParameterRow::parameterWidget()
{
    return mLineEdit;
}

std::shared_ptr<proc::Parameter> ScalarParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setScalarValue(mLineEdit->text().toDouble());
    return p;
}

} // namespace vcl::qt
