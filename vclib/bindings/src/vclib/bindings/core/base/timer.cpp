// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/base/timer.h>
#include <vclib/bindings/utils.h>

#include <vclib/base.h>

namespace vcl::bind {

void initBaseTimer(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    py::class_<Timer> c(m, "Timer");
    c.def(py::init<bool>(), "start"_a = true);
    c.def(py::init<std::string, bool>(), "caption"_a, "start"_a = true);

    c.def("start", &Timer::start);
    c.def("stop", &Timer::stop);
    c.def("stop_and_print", &Timer::stopAndPrint);
    c.def("print", &Timer::print);
    c.def("delay", &Timer::delay);
    c.def("set_caption", &Timer::setCaption, "caption"_a);
}

} // namespace vcl::bind
