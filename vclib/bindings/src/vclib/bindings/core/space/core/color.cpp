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

#include <vclib/bindings/core/space/core/color.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initColor(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<Color> c(m, "Color", py::buffer_protocol());
    c.def(py::init<>());
    c.def(py::init<uint8_t, uint8_t, uint8_t>());
    c.def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>());

    defCopy(c);

    c.def_buffer([](Color& p) -> py::buffer_info {
        return py::buffer_info(
            p.data(),                                 /* Pointer to buffer */
            sizeof(uint8_t),                          /* Size of one scalar */
            py::format_descriptor<uint8_t>::format(), /* Python struct-style
               format descriptor */
            1,                                        /* Number of dimensions */
            {4},                                      /* Buffer dimensions */
            {sizeof(uint8_t)} /* Strides (in bytes) for each index */
        );
    });

    c.def("red", py::overload_cast<>(&Color::red, py::const_));
    c.def("set_red", [](Color& c, uint v) {
        c.red() = v;
    });
    c.def("red_f", &Color::redF);
    c.def("set_red_f", &Color::setRedF);
    c.def("green", py::overload_cast<>(&Color::green, py::const_));
    c.def("set_green", [](Color& c, uint v) {
        c.green() = v;
    });
    c.def("green_f", &Color::greenF);
    c.def("set_green_f", &Color::setGreenF);
    c.def("blue", py::overload_cast<>(&Color::blue, py::const_));
    c.def("set_blue", [](Color& c, uint v) {
        c.blue() = v;
    });
    c.def("blue_f", &Color::blueF);
    c.def("set_blue_f", &Color::setBlueF);
    c.def("alpha", py::overload_cast<>(&Color::alpha, py::const_));
    c.def("set_alpha", [](Color& c, uint v) {
        c.alpha() = v;
    });
    c.def("alpha_f", &Color::alphaF);
    c.def("set_alpha_f", &Color::setAlphaF);

    // operators
    c.def("__call__", [](Color& p, uint i) { // operator()
        return p(i);
    });

    c.def("__getitem__", [](Color& p, uint i) { // operator[]
        return p(i);
    });

    c.def("__setitem__", [](Color& p, uint i, uint8_t v) { // operator[]
        p(i) = v;
    });

    defComparisonOperators(c);

    defRepr(c);

    // Format enum
    py::enum_<Color::Format> formatEnum(c, "Format");
    formatEnum.value("ARGB", Color::Format::ARGB);
    formatEnum.value("ABGR", Color::Format::ABGR);
    formatEnum.value("RGBA", Color::Format::RGBA);
    formatEnum.value("BGRA", Color::Format::BGRA);
    formatEnum.export_values();

    // ColorABGR enum
    py::enum_<Color::ColorABGR> colorEnum(c, "ColorABGR");
    colorEnum.value("BLACK", Color::ColorABGR::Black);
    colorEnum.value("DARK_GRAY", Color::ColorABGR::DarkGray);
    colorEnum.value("GRAY", Color::ColorABGR::Gray);
    colorEnum.value("LIGHT_GRAY", Color::ColorABGR::LightGray);
    colorEnum.value("WHITE", Color::ColorABGR::White);
    colorEnum.value("RED", Color::ColorABGR::Red);
    colorEnum.value("GREEN", Color::ColorABGR::Green);
    colorEnum.value("BLUE", Color::ColorABGR::Blue);
    colorEnum.value("YELLOW", Color::ColorABGR::Yellow);
    colorEnum.value("CYAN", Color::ColorABGR::Cyan);
    colorEnum.value("MAGENTA", Color::ColorABGR::Magenta);
    colorEnum.value("LIGHT_RED", Color::ColorABGR::LightRed);
    colorEnum.value("LIGHT_GREEN", Color::ColorABGR::LightGreen);
    colorEnum.value("LIGHT_BLUE", Color::ColorABGR::LightBlue);
    colorEnum.value("LIGHT_CYAN", Color::ColorABGR::LightCyan);
    colorEnum.value("LIGHT_YELLOW", Color::ColorABGR::LightYellow);
    colorEnum.value("LIGHT_MAGENTA", Color::ColorABGR::LightMagenta);
    colorEnum.value("DARK_RED", Color::ColorABGR::DarkRed);
    colorEnum.value("DARK_GREEN", Color::ColorABGR::DarkGreen);
    colorEnum.value("DARK_BLUE", Color::ColorABGR::DarkBlue);
    colorEnum.value("DARK_CYAN", Color::ColorABGR::DarkCyan);
    colorEnum.value("DARK_YELLOW", Color::ColorABGR::DarkYellow);
    colorEnum.value("DARK_MAGENTA", Color::ColorABGR::DarkMagenta);
    colorEnum.value("LIGHT_BROWN", Color::ColorABGR::LightBrown);
    colorEnum.value("DARK_BROWN", Color::ColorABGR::DarkBrown);
    colorEnum.value("BROWN", Color::ColorABGR::Brown);
    colorEnum.value("TRANSPARENT_BLACK", Color::ColorABGR::TransparentBlack);
    colorEnum.value("TRANSPARENT_WHITE", Color::ColorABGR::TransparentWhite);
    colorEnum.export_values();

    // ColorMap enum
    py::enum_<Color::ColorMap> colorMapEnum(c, "ColorMap");
    colorMapEnum.value("RED_BLUE", Color::ColorMap::RedBlue);
    colorMapEnum.value("PARULA", Color::ColorMap::Parula);
    colorMapEnum.value("GREY_SHADE", Color::ColorMap::GreyShade);
    colorMapEnum.export_values();
}

} // namespace vcl::bind
