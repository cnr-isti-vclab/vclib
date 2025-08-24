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

#include <vclib/bindings/core/base/logger/console_logger.h>
#include <vclib/bindings/utils.h>

#include <vclib/base.h>

#include <pybind11/operators.h>

namespace vcl::bind {

void initConsoleLogger(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<ConsoleLogger, AbstractLogger> c(m, "ConsoleLogger");

    c.def(py::init<>());

    c.def("start_timer", &ConsoleLogger::startTimer);
    c.def("stop_timer", &ConsoleLogger::stopTimer);
    c.def("time", &ConsoleLogger::time);

    c.def("percentage", &ConsoleLogger::percentage);

    c.def(
        "log",
        [](ConsoleLogger& self, const std::string& msg) {
            self.log(msg);
        },
        py::arg("msg"));
    c.def(
        "log",
        [](ConsoleLogger&          self,
           const std::string&      msg,
           ConsoleLogger::LogLevel lvl) {
            self.log(msg, lvl);
        },
        py::arg("msg"),
        py::arg("lvl"));

    c.def(
        "log",
        [](ConsoleLogger& self, uint perc, const std::string& msg) {
            self.log(perc, msg);
        },
        py::arg("perc"),
        py::arg("msg"));

    c.def(
        "log",
        [](ConsoleLogger&          self,
           uint                    perc,
           const std::string&      msg,
           ConsoleLogger::LogLevel lvl) {
            self.log(perc, msg, lvl);
        },
        py::arg("perc"),
        py::arg("msg"),
        py::arg("lvl"));
}

} // namespace vcl::bind
