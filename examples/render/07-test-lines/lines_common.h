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

#ifndef LINES_COMMON_H
#define LINES_COMMON_H

#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include <vclib/render/drawable/drawable_object_vector.h>

#include <random>

void pushRandomLine(std::vector<vcl::lines::LinesVertex>& points)
{
    std::random_device rd;
    std::mt19937       gen(rd());
    std::uniform_real_distribution<float> disPoint(-2.0f, 2.0f);
    std::uniform_real_distribution<float> disColor(0.0f, 1.0f);

    float r = disColor(gen);
    float g = disColor(gen);
    float b = disColor(gen);

    points.emplace_back(
        disPoint(gen), disPoint(gen), disPoint(gen),
        vcl::lines::LinesVertex::COLOR(r, g, b, 1.0f));
    points.emplace_back(
        disPoint(gen), disPoint(gen), disPoint(gen),
        vcl::lines::LinesVertex::COLOR(r, g, b, 1.0f));
}

// return a vector that has a set of nLines lines with different types
vcl::DrawableObjectVector getDrawableLines(vcl::uint nLines)
{
    vcl::DrawableObjectVector vec;

    std::vector<vcl::lines::LinesVertex> points;

    for (vcl::uint i = 0; i < nLines * 2; i++)
        pushRandomLine(points);

    for (vcl::uint i = 0; i < 5; ++i) {
        vcl::lines::LinesTypes type = static_cast<vcl::lines::LinesTypes>(
            vcl::toUnderlying(vcl::lines::LinesTypes::CPU_GENERATED) + i);

        auto line = vcl::lines::DrawableLines::create(
            points, type);
        line->getSettings()->setColorToUse(
            vcl::lines::ColorToUse::PER_VERTEX_COLOR);

        vec.pushBack(*line.get());

        vec.at(i)->setVisibility(false);
    }

    vec.at(0)->setVisibility(true);

    return vec;
}

#endif // LINES_COMMON_H
