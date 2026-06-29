// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/histogram.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initHistogram(pybind11::module& m)
{
    namespace py = pybind11;

    using Histd = vcl::Histogram<double>;

    py::class_<Histd> c(m, "Histogram" /*, py::buffer_protocol()*/);
    c.def(py::init<>());

    c.def(
        py::init([](double minRange,
                    double maxRange,
                    uint   nBins,
                    double gamma = 1.0) {
            return Histd(minRange, maxRange, nBins, gamma);
        }));

    c.def("clear", &Histd::clear);
    c.def(
        "add_value",
        &Histd::addValue,
        py::arg("value"),
        py::arg("increment") = 1.0);
    c.def("range_value_min", &Histd::rangeValueMin);
    c.def("range_value_max", &Histd::rangeValueMax);
    c.def("value_sum", &Histd::valueSum);
    c.def("value_count", &Histd::valueCount);
    c.def("value_min", &Histd::valueMin);
    c.def("value_max", &Histd::valueMax);
    c.def("bin_values_count_max", &Histd::binValuesCountMax);
    c.def("bin_values_count_in_range_max", &Histd::binValuesCountInRangeMax);
    c.def("bin_count", &Histd::binCount);
    c.def("bin_values_count", &Histd::binValuesCount, py::arg("ind"));
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
