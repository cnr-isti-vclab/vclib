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

#include <vclib/render/selection/selection_box.h>
#include <vclib/render/selection/selection_mode.h>

#include <functional>
#include <map>

namespace vcl {

enum class ToolSets { DEFAULT, SELECTION };

/**
 * @brief Adds the keybinds to choose selection modes. It is worth noting that
 * when using the 'Selection Toolset' no key or mouse events are propagated to
 * the parent class, meaning that while selecting you may not interact with the
 * TrackBall in ways that would conflict with the controls for the selection
 */
template<typename Scalar, typename DerivedRenderApp>
class SelectionTrackBallEventDrawerT :
        public TrackBallEventDrawerT<Scalar, DerivedRenderApp>
{
    using Base = TrackBallEventDrawerT<Scalar, DerivedRenderApp>;

    SelectionBox  mSelectionBox;
    ToolSets      mCurrentToolset       = ToolSets::DEFAULT;
    SelectionMode mCurrentSelectionMode = SelectionMode::VERTEX_REGULAR;
    SelectionMode mPreviousNonAtomicSelectionMode =
        SelectionMode::VERTEX_REGULAR;
    bool mSelectionCalcRequired = false;
    bool mLMBHeld               = false;
    bool mLMBPressPositionTaken = false;
    bool mRMBHeld               = false;
    bool mMMBHeld               = false;

    void setPrevModIfNonAtomic()
    {
        if (mCurrentSelectionMode.isAtomicMode()) {
            return;
        }
        mPreviousNonAtomicSelectionMode = mCurrentSelectionMode;
    }

    std::map<std::pair<Key::Enum, KeyModifiers>, std::function<void()>>
        mKeyMap = {
            {{Key::A, {KeyModifier::NO_MODIFIER}},
             [&]() {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::VERTEX_ADD;
             }},
            {{Key::A, {KeyModifier::SHIFT}},
             [&]() {
                 setPrevModIfNonAtomic();
                 mSelectionCalcRequired = true;
                 mCurrentSelectionMode  = SelectionMode::VERTEX_ALL;
             }},
            {{Key::D, {KeyModifier::NO_MODIFIER}},
             [&]() {
                 setPrevModIfNonAtomic();
                 mSelectionCalcRequired = true;
                 mCurrentSelectionMode  = SelectionMode::VERTEX_NONE;
             }},
            {{Key::S, {KeyModifier::NO_MODIFIER}},
             [&]() {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::VERTEX_SUBTRACT;
             }},
            {{Key::I, {KeyModifier::NO_MODIFIER}},
             [&]() {
                 setPrevModIfNonAtomic();
                 mSelectionCalcRequired = true;
                 mCurrentSelectionMode  = SelectionMode::VERTEX_INVERT;
             }},
            {{Key::R, {KeyModifier::NO_MODIFIER}},
             [&]() {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::VERTEX_REGULAR;
             }},
            {{Key::R, {KeyModifier::CONTROL}},
             [&]() {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::FACE_REGULAR;
             }},
            {{Key::A, {KeyModifier::CONTROL}},
             [&] {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::FACE_ADD;
             }},
            {{Key::A, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [&] {
                 setPrevModIfNonAtomic();
                 mSelectionCalcRequired = true;
                 mCurrentSelectionMode  = SelectionMode::FACE_ALL;
             }},
            {{Key::D, {KeyModifier::CONTROL}},
             [&] {
                 setPrevModIfNonAtomic();
                 mSelectionCalcRequired = true;
                 mCurrentSelectionMode  = SelectionMode::FACE_NONE;
             }},
            {{Key::S, {KeyModifier::CONTROL}},
             [&] {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::FACE_SUBTRACT;
             }},
            {{Key::I, {KeyModifier::CONTROL}},
             [&] {
                 setPrevModIfNonAtomic();
                 mSelectionCalcRequired = true;
                 mCurrentSelectionMode  = SelectionMode::FACE_INVERT;
             }},
            {{Key::R, {KeyModifier::CONTROL, KeyModifier::ALT}},
             [&] {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::FACE_VISIBLE_REGULAR;
             }},
            {{Key::A, {KeyModifier::CONTROL, KeyModifier::ALT}},
             [&] {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::FACE_VISIBLE_ADD;
             }},
            {{Key::S, {KeyModifier::CONTROL, KeyModifier::ALT}},
             [&] {
                 setPrevModIfNonAtomic();
                 mCurrentSelectionMode = SelectionMode::FACE_VISIBLE_SUBTRACT;
             }},
    };

    std::map<std::pair<Key::Enum, KeyModifiers>, bool> mPressActionExecuted =
        {};

protected:
    // Needs to be checked each frame and needs to be managed IMMEDIATELY!!!
    bool selectionCalculationRequired()
    {
        // We cannot assume that if the selection box is the same then the
        // selection is the same: The view matrix, projection matrix or existing
        // meshes may have changed since the last time
        return mSelectionCalcRequired;
    }

    // To signal to this class that the selection has been calculated
    void selectionCalculated()
    {
        mSelectionCalcRequired = false;
        if (!isSelectionTemporary()) {
            mSelectionBox.nullAll();
        }
        if (mCurrentSelectionMode.isAtomicMode()) {
            mCurrentSelectionMode = mPreviousNonAtomicSelectionMode;
        }
    }

    bool isSelectionTemporary() { return mLMBHeld; }

    SelectionMode selectionMode() { return mCurrentSelectionMode; }

    SelectionBox selectionBox() { return mSelectionBox; }

    ToolSets getCurrentToolset() { return mCurrentToolset; }

public:
    using Base::Base;

    // Cannot swap back to "trackball mode" until the latest pending selection
    // has been successfully calculated and until all the mouse buttons are
    // released. Also, cannot swap selection mode while defining the selection
    // area (dragging the mouse with LMB held down)
    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        if (key == Key::S && modifiers == KeyModifiers {KeyModifier::ALT} &&
            !mRMBHeld && !mMMBHeld && !mLMBHeld && !mSelectionCalcRequired) {
            mCurrentToolset =
                (mCurrentToolset == ToolSets::DEFAULT ? ToolSets::SELECTION :
                                                        ToolSets::DEFAULT);
            if (mCurrentToolset == ToolSets::DEFAULT) {
                for (auto& it : mPressActionExecuted) {
                    it.second = false;
                }
            }
        }
        if (mCurrentToolset == ToolSets::DEFAULT) {
            Base::onKeyPress(key, modifiers);
            return;
        }
        if (mLMBHeld) {
            return;
        }
        auto pair  = std::make_pair(key, modifiers);
        auto found = mKeyMap.find(pair);
        if (found == mKeyMap.end()) {
            return;
        }
        auto found2 = mPressActionExecuted.find(pair);
        if (found2 == mPressActionExecuted.end()) {
            mPressActionExecuted.insert({
                {pair, false}
            });
            found2 = mPressActionExecuted.find(pair);
        }
        if (found2->second) {
            return;
        }
        found->second();
        mPressActionExecuted[pair] = true;
    }

    void onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) override
    {
        if (mCurrentToolset == ToolSets::DEFAULT) {
            Base::onKeyRelease(key, modifiers);
            return;
        }
        auto pair  = std::make_pair(key, modifiers);
        auto found = mPressActionExecuted.find(pair);
        if (found == mPressActionExecuted.end()) {
            return;
        }
        found->second = false;
    }

    void onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        switch (button) {
        case MouseButton::RIGHT: mRMBHeld = true; break;
        case MouseButton::LEFT: mLMBHeld = true; break;
        case MouseButton::MIDDLE: mMMBHeld = true; break;
        default: break;
        }
        if (mCurrentToolset == ToolSets::DEFAULT) {
            Base::onMousePress(button, x, y, modifiers);
            return;
        }
        if (mLMBHeld && !mLMBPressPositionTaken) {
            mSelectionBox.set1({x, y});
            mSelectionBox.set2({x, y});
            mLMBPressPositionTaken = true;
        }
    }

    void onMouseMove(double x, double y, const KeyModifiers& modifiers) override
    {
        if (mCurrentToolset == ToolSets::DEFAULT) {
            Base::onMouseMove(x, y, modifiers);
            return;
        }
        if (mLMBHeld) {
            mSelectionBox.set2({x, y});
            mSelectionCalcRequired = true;
        }
    }

    void onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        switch (button) {
        case MouseButton::RIGHT: mRMBHeld = false; break;
        case MouseButton::LEFT: mLMBHeld = false; break;
        case MouseButton::MIDDLE: mMMBHeld = false; break;
        default: break;
        }
        if (mCurrentToolset == ToolSets::DEFAULT) {
            Base::onMouseRelease(button, x, y, modifiers);
            return;
        }
        if (mLMBHeld) {
            return;
        }
        mLMBPressPositionTaken = false;
        mSelectionCalcRequired = true;
    }
};

template<typename DerivedRenderApp>
using SelectionTrackBallEventDrawer =
    SelectionTrackBallEventDrawerT<float, DerivedRenderApp>;

} // namespace vcl

#endif
