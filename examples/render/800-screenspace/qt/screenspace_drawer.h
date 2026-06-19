/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef SCREENSPACE_DRAWER_H
#define SCREENSPACE_DRAWER_H

#include <vclib/bgfx/screenspace/overlay/screenspace_box.h>
#include <vclib/bgfx/screenspace/primitives/screenspace_lines.h>
#include <vclib/bgfx/screenspace/primitives/screenspace_points.h>
#include <vclib/render/drawers/plain_drawer.h>

template<typename DerivedRenderApp>
class ScreenSpaceDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{
    vcl::ScreenSpacePoints mPointsGCS;
    vcl::ScreenSpacePoints mPointsGCC;
    vcl::ScreenSpacePoints mPointsVC;

    vcl::ScreenSpaceBox mBox;

    vcl::ScreenSpaceLines mLinesGC;
    vcl::ScreenSpaceLines mLinesVC;
    vcl::ScreenSpaceLines mLinesLC;
    vcl::ScreenSpaceLines mLinesLSVC;
    vcl::ScreenSpaceLines mLinesLSLC;

public:
    ScreenSpaceDrawer(vcl::uint width = 1024, vcl::uint height = 768)
    {
        // points - general color - square

        std::vector<vcl::Point2f> pts = {
            {100, 100},
            {200, 150},
            {300, 200},
            {400, 250},
            {500, 300}
        };

        mPointsGCS.setWidth(10);
        mPointsGCS.setColorSetting(
            vcl::ScreenSpacePoints::ColorSetting::GENERAL);
        mPointsGCS.setShapeSetting(vcl::ScreenSpacePoints::Shape::SQUARE);

        mPointsGCS.setVertices(pts);

        // points - general color - circle

        for (auto& p : pts)
            p += vcl::Point2f {100, 0};

        mPointsGCC.setWidth(12);
        mPointsGCC.setColorSetting(
            vcl::ScreenSpacePoints::ColorSetting::GENERAL);
        mPointsGCC.setGeneralColor(vcl::Color::Blue);
        mPointsGCC.setShapeSetting(vcl::ScreenSpacePoints::Shape::CIRCLE);

        mPointsGCC.setVertices(pts);

        // points - vertex color - circle

        for (auto& p : pts)
            p += vcl::Point2f {100, 0};

        std::vector<vcl::Color> colors = {
            vcl::Color::Red,
            vcl::Color::Green,
            vcl::Color::Blue,
            vcl::Color::Yellow,
            vcl::Color::Magenta};

        mPointsVC.setWidth(15);
        mPointsVC.setColorSetting(
            vcl::ScreenSpacePoints::ColorSetting::PER_VERTEX);
        mPointsVC.setShapeSetting(vcl::ScreenSpacePoints::Shape::CIRCLE);

        mPointsVC.setVertices(pts);
        mPointsVC.setVertexColors(colors);

        mBox.setColor(vcl::Color::DarkYellow);

        mBox.setBox(vcl::Box2d({800, 50}, {1000, 150}));

        // lines

        // general color
        std::vector<vcl::Point2f> lpts {
            {20, 300},
            {120, 400},
            {20, 400},
            {120, 300}
        };

        mLinesGC.setVertices(lpts);
        mLinesGC.setWidth(15);
        mLinesGC.setColorSetting(vcl::ScreenSpaceLines::ColorSetting::GENERAL);
        mLinesGC.setGeneralColor(vcl::Color::Magenta);

        // per vertex color
        for (auto& p : lpts)
            p += vcl::Point2f {0, 150};

        std::vector<vcl::Color> lvcols {
            vcl::Color::Red,
            vcl::Color::Green,
            vcl::Color::Blue,
            vcl::Color::Yellow
        };

        mLinesVC.setVertices(lpts);
        mLinesVC.setVertexColors(lvcols);

        mLinesVC.setWidth(10);
        mLinesVC.setColorSetting(vcl::ScreenSpaceLines::ColorSetting::PER_VERTEX);

        // per line color
        auto lpts2 = lpts;

        for (auto& p : lpts2)
            p += vcl::Point2f {0, 150};

        std::vector<vcl::Color> llcols {vcl::Color::Blue, vcl::Color::Yellow};

        mLinesLC.setVertices(lpts2);
        mLinesLC.setLineColors(llcols);
        mLinesLC.setWidth(8);
        mLinesLC.setColorSetting(vcl::ScreenSpaceLines::ColorSetting::PER_LINE);

        // line strip per vertex color
        for (auto& p : lpts)
            p += vcl::Point2f {150, 0};

        mLinesLSVC.setVertices(lpts);
        mLinesLSVC.setVertexColors(lvcols);
        mLinesLSVC.setWidth(12);
        mLinesLSVC.setTopology(vcl::ScreenSpaceLines::Topology::LINE_STRIP);
        mLinesLSVC.setColorSetting(vcl::ScreenSpaceLines::ColorSetting::PER_VERTEX);

        // line strip per line color
        for (auto& p : lpts2)
            p += vcl::Point2f {150, 0};

        std::vector<vcl::Color> lscols {
            vcl::Color::Blue, vcl::Color::Red, vcl::Color::Yellow};

        mLinesLSLC.setVertices(lpts2);
        mLinesLSLC.setLineColors(lscols);
        mLinesLSLC.setWidth(5);
        mLinesLSLC.setTopology(vcl::ScreenSpaceLines::Topology::LINE_STRIP);
        mLinesLSLC.setColorSetting(vcl::ScreenSpaceLines::ColorSetting::PER_LINE);
    }

    void onInit(vcl::uint viewId) override
    {
        vcl::Color backgroundColor = vcl::Color::Gray;

        DerivedRenderApp::DRW::setCanvasDefaultClearColor(
            static_cast<DerivedRenderApp*>(this), backgroundColor);

        mBox.init();
    }

    void onDrawContent(vcl::uint viewId) override
    {
        mPointsGCS.draw(viewId);
        mPointsGCC.draw(viewId);
        mPointsVC.draw(viewId);

        mBox.draw(viewId);

        mLinesGC.draw(viewId);
        mLinesVC.draw(viewId);
        mLinesLC.draw(viewId);
        mLinesLSVC.draw(viewId);
        mLinesLSLC.draw(viewId);
    }
};

#endif // SCREENSPACE_DRAWER_H
