// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
