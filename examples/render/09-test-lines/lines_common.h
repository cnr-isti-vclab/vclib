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

void pushRandomLine(std::vector<float>& vertCoords, std::vector<vcl::uint>& vertColors)
{
    std::random_device rd;
    std::mt19937       gen(rd());
    std::uniform_real_distribution<float> disPoint(-2.0f, 2.0f);
    std::uniform_real_distribution<float> disColor(0.0f, 1.0f);

    float r = disColor(gen);
    float g = disColor(gen);
    float b = disColor(gen);

    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));

    vertColors.emplace_back(vcl::LinesVertex::COLORABGR(r, g, b, 1.0f));

    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));
    vertCoords.emplace_back(disPoint(gen));

    vertColors.emplace_back(vcl::LinesVertex::COLORABGR(r, g, b, 1.0f));
}

// return a vector that has a set of nLines lines with different types
vcl::DrawableObjectVector getDrawableLines(vcl::uint nLines)
{
    vcl::DrawableObjectVector vec;

    std::vector<float> vertCoords;
    std::vector<vcl::uint> vertColors;
    std::vector<float> vertNormals(nLines * 2 * 3, 0.0f); 
    
    for (vcl::uint i = 0; i < nLines; i++)
        pushRandomLine(vertCoords, vertColors);
    
    auto cpuLines = std::make_shared<vcl::DrawableCPULines>(vertCoords, vertColors, vertNormals);
    auto gpuLines = std::make_shared<vcl::DrawableGPULines>(vertCoords, vertColors, vertNormals);
    // auto instancingLines = std::make_shared<vcl::DrawableInstancingLines>(points);
    // auto indirectLines = std::make_shared<vcl::DrawableIndirectLines>(points);
    // auto textureLines = std::make_shared<vcl::DrawableTextureLines>(points);

    cpuLines->settings().setColorToUse(
        vcl::LineColorToUse::PER_VERTEX_COLOR);
    gpuLines->settings().setColorToUse(
        vcl::LineColorToUse::PER_VERTEX_COLOR);
    // instancingLines->settings().setColorToUse(
    //     vcl::LineColorToUse::PER_VERTEX_COLOR);
    // indirectLines->settings().setColorToUse(
    //     vcl::LineColorToUse::PER_VERTEX_COLOR);
    // textureLines->settings().setColorToUse(
    //     vcl::LineColorToUse::PER_VERTEX_COLOR);

    vec.pushBack(*cpuLines.get());
    vec.pushBack(*gpuLines.get());
    // vec.pushBack(*instancingLines.get());
    // vec.pushBack(*indirectLines.get());
    // vec.pushBack(*textureLines.get());

    for (vcl::uint i = 0; i < vec.size(); ++i) {
        vec.at(i)->setVisibility(true);
    }

    // vec.at(0)->setVisibility(true);

    return vec;
}

#endif // LINES_COMMON_H
