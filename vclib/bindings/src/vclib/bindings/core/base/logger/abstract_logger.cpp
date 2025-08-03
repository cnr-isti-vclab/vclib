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

#include <vclib/bindings/core/base/logger/abstract_logger.h>
#include <vclib/bindings/utils.h>

#include <vclib/base.h>

#include <pybind11/operators.h>

namespace vcl::bind {

void initAbstractLogger(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<AbstractLogger> c(m, "AbstractLogger");

    // define LogLevel enum
    py::enum_<AbstractLogger::LogLevel> llEnum(c, "LogLevel");
    llEnum.value("ERROR_LOG", AbstractLogger::ERROR_LOG);
    llEnum.value("WARNING_LOG", AbstractLogger::WARNING_LOG);
    llEnum.value("MESSAGE_LOG", AbstractLogger::MESSAGE_LOG);
    llEnum.value("PROGRESS_LOG", AbstractLogger::PROGRESS_LOG);
    llEnum.value("DEBUG_LOG", AbstractLogger::DEBUG_LOG);
    llEnum.export_values();
}

} // namespace vcl::bind
