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

#ifndef VCL_AUTOMATION_ACTION_ALIASES_H
#define VCL_AUTOMATION_ACTION_ALIASES_H

#include "actions.h"

namespace vcl {

template<typename T>
using ScaleAdditiveAA = ChangeScaleAdditiveAutomationAction;

template<typename T>
using ScaleMultiplicativeAA = ChangeScaleMultiplicativeAutomationAction;

template<typename T>
using PFScaleAdditiveAA = PerFrameChangeScaleAdditiveAutomationAction;

template<typename T>
using PFScaleMultiplicativeAA =
    PerFrameChangeScaleMultiplicativeAutomationAction;

template<typename T>
using PFRotationAA = PerFrameRotataionAutomationAction;

template<typename T>
using RotationAA = RotationAutomationAction;

template<typename T>
using SequentialAAs = SequentialAutomationActions;

template<typename T>
using SimultaneousAAs = SimultaneousAutomationActions;

template<typename T>
using FDelayAA = FrameDelayAutomationAction;

template<typename T>
using FLimitedAA = FrameLimitedAutomationAction;

template<typename T>
using SCDelayAA = StartCountDelayAutomationAction;

template<typename T>
using SCLimitedAA = StartCountLimitedAutomationAction;

template<typename T>
using TDelayAA = TimeDelayAutomationAction;

template<typename T>
using TLimitedAA = TimeLimitedAutomationAction;

} // namespace vcl

#endif