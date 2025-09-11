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

#ifndef VCL_SELECTION_TRACKBALL_EVENT_DRAWER_H
#define VCL_SELECTION_TRACKBALL_EVENT_DRAWER_H

#include "trackball_event_drawer.h"

#include <algorithm>
#include <functional>
#include <map>

namespace vcl {

enum class ToolSets { DEFAULT, SELECTION };
enum class SelectionMode {
    VERTEX_REGULAR,
    VERTEX_ADD,
    VERTEX_SUBTRACT,
    VERTEX_ALL,
    VERTEX_NONE,
    VERTEX_INVERT
};

template<typename Scalar, typename DerivedRenderApp>
class SelectionTrackBallEventDrawerT :
        public TrackBallEventDrawerT<Scalar, DerivedRenderApp>
{
    using Base = TrackBallEventDrawerT<Scalar, DerivedRenderApp>;
    using SelectionBox =
        std::pair<std::optional<Scalar>, std::optional<Scalar>>;

    // Comparison does not work properly...
    SelectionBox  mPrevSelectionBox      = {std::nullopt, std::nullopt};
    SelectionBox  mSelectionBox          = {std::nullopt, std::nullopt};
    ToolSets      mCurrentToolset        = ToolSets::DEFAULT;
    SelectionMode mCurrentSelectionMode  = SelectionMode::VERTEX_REGULAR;
    bool          mSelectionCalcRequired = false;
    bool          mLMBHeld               = false;
    bool          mOtherMouseButtonsHeld = false;

    bool areSelectionBoxesEqual(SelectionBox box1, SelectionBox box2)
    {
        SelectionBox inverted2 = {box2.second, box2.first};
        return (box1 == box2 || box1 == inverted2);
    }

protected:
    bool selectionCalculationRequired()
    {
        // We cannot assume that if the selection box is the same then the selection is the same:
        // The view matrix, projection matrix or existing meshes may have changed since the last time
        return (mSelectionCalcRequired && !mLMBHeld);
    }

    void selectionCalculated()
    {
        mSelectionCalcRequired = false;
        mPrevSelectionBox      = mSelectionBox;
        mSelectionBox          = {std::nullopt, std::nullopt};
    }

    SelectionMode selectionMode() { return mCurrentSelectionMode; }

public:
    using Base::Base;

    // {{Key::S, {KeyModifier::ALT}}, [&]() {
    //  if (mLMBHeld || mOtherMouseButtonsHeld) {
    //  return;
    //  }
    //  mCurrentToolset = (mCurrentToolset == ToolSets::SELECTION ?
    //  ToolSets::DEFAULT : ToolSets::SELECTION);
    // }},

    std::map<std::pair<Key::Enum, KeyModifiers>, std::function<void()>>
        mKeyMap = {
            {{Key::A, {KeyModifier::NO_MODIFIER}},
             [&]() {
                 mCurrentSelectionMode = SelectionMode::VERTEX_ADD;
             }},
            {{Key::A, {KeyModifier::SHIFT}}, [&](){
                mCurrentSelectionMode = SelectionMode::VERTEX_ALL;
            }}
    };
};

} // namespace vcl

#endif
