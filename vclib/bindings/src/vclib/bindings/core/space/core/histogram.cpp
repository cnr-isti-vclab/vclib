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

#include <vclib/bindings/core/space/core/histogram.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initHistogram(pybind11::module& m)
{
    namespace py = pybind11;

    using Histd = vcl::Histogram<double>;

    py::class_<Histd> c(m, "Histogram" /*, py::buffer_protocol()*/);
    c.def(py::init<>());

    c.def(py::init(
        [](double minRange, double maxRange, uint nBins, double gamma = 1.0) {
            return Histd(minRange, maxRange, nBins, gamma);
        }));

    c.def("clear", &Histd::clear);
    c.def(
        "add_value",
        &Histd::addValue,
        py::arg("value"),
        py::arg("increment") = 1.0);
    c.def("min_range_value", &Histd::minRangeValue);
    c.def("max_range_value", &Histd::maxRangeValue);
    c.def("sum_values", &Histd::sumValues);
    c.def("number_values", &Histd::numberValues);
    c.def("min_value", &Histd::minValue);
    c.def("max_value", &Histd::maxValue);
    c.def("max_bin_count", &Histd::maxBinCount);
    c.def("max_bin_count_in_range", &Histd::maxBinCountInRange);
    c.def("bins_number", &Histd::binsNumber);
    c.def("bin_count", &Histd::binCount, py::arg("ind"));
    c.def("bin_lower_bound", &Histd::binLowerBound, py::arg("ind"));
    c.def("bin_upper_bound", &Histd::binUpperBound, py::arg("ind"));
    c.def("bin_of_value_count", [](Histd& h, double value, double width) {
        return h.binOfValueCount(value, width);
    });
    c.def("bin_of_value_count", [](Histd& h, double value) {
        return h.binOfValueCount(value);
    });
    c.def("bin_of_value_width", &Histd::binOfValueWidth, py::arg("value"));
    c.def(
        "range_count",
        &Histd::rangeCount,
        py::arg("rangeMin"),
        py::arg("rangeMax"));
    c.def(
        "value_at_percentile",
        &Histd::valueAtPercentile,
        py::arg("percentile"));
    c.def("average", &Histd::average);
    c.def("root_mean_square", &Histd::rootMeanSquare);
    c.def("variance", &Histd::variance);
    c.def("standard_deviation", &Histd::standardDeviation);
}

} // namespace vcl::bind
