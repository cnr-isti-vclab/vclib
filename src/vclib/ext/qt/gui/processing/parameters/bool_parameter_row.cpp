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

#include <vclib/ext/qt/gui/processing/parameters/bool_parameter_row.h>

namespace vcl::qt {

BoolParameterRow::BoolParameterRow(const proc::BoolParameter& param) :
        ParameterRow(param), mParam(param)
{
    mCheckBox = new QCheckBox("");
    mCheckBox->setToolTip(param.tooltip().c_str());
    mCheckBox->setChecked(param.boolValue());

    QObject::connect(
        mDescriptionLabel, SIGNAL(clicked()), mCheckBox, SLOT(toggle()));
}

QWidget* BoolParameterRow::parameterWidget()
{
    return mCheckBox;
}

std::shared_ptr<proc::Parameter> BoolParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setBoolValue(mCheckBox->isChecked());
    return p;
}

} // namespace vcl::qt
