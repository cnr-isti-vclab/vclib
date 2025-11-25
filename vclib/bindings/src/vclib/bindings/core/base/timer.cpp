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
