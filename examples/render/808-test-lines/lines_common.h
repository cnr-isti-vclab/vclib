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

#include <vclib/bgfx/drawable/drawable_lines.h>
#include <vclib/render/drawable/drawable_object_vector.h>

#include <random>

void pushRandomLine(std::vector<float>& vertCoords, std::vector<vcl::uint>& vertColors, std::vector<vcl::uint>& lineColors)
{
    std::random_device rd;
    std::mt19937       gen(rd());
    std::uniform_real_distribution<float> disPoint(-2.0f, 2.0f);
    std::uniform_real_distribution<float> disColor(0, 255);

    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));

    vcl::Color c0 = vcl::Color(
        static_cast<uint8_t>(disColor(gen)), 
        static_cast<uint8_t>(disColor(gen)), 
        static_cast<uint8_t>(disColor(gen))
    );
    vertColors.emplace_back(c0.abgr());

    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));

    vcl::Color c1 = vcl::Color(
        static_cast<uint8_t>(disColor(gen)), 
        static_cast<uint8_t>(disColor(gen)), 
        static_cast<uint8_t>(disColor(gen))
    );
    vertColors.emplace_back(c1.abgr());

    vcl::Color lineColor = vcl::Color(
        static_cast<uint8_t>(disColor(gen)), 
        static_cast<uint8_t>(disColor(gen)), 
        static_cast<uint8_t>(disColor(gen))
    );
    lineColors.emplace_back(lineColor.abgr());
}

// return a vector that has a set of nLines lines with different types
vcl::DrawableLines getDrawableLines(vcl::uint nLines)
{
    vcl::DrawableObjectVector vec;
    
    std::vector<float> vertCoords;
    std::vector<float> vertNormals;
    std::vector<vcl::uint> vertColors;
    std::vector<vcl::uint> lineColors;
    
    for (vcl::uint i = 0; i < nLines; i++)
        pushRandomLine(vertCoords, vertColors, lineColors);
    
    auto lines = vcl::DrawableLines(vertCoords, vertNormals, vertColors, lineColors);
    lines.thickness() = 10;
    lines.colorToUse() = vcl::Lines::ColorToUse::GENERAL;

    return lines;
}

#endif // LINES_COMMON_H
