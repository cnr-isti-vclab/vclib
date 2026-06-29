// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
