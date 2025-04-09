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

#ifndef VCL_AUTOMATION_ACTIONS_H
#define VCL_AUTOMATION_ACTIONS_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/automation/actions/change_scale_absolute_automation_action.h>
#include <vclib/render/automation/actions/per_frame_change_scale_absolute_automation_action.h>
#include <vclib/render/automation/actions/per_frame_rotation_automation_action.h>
#include <vclib/render/automation/actions/rotation_automation_action.h>
#include <vclib/render/automation/actions/sequential_automation_actions.h>
#include <vclib/render/automation/actions/simultaneous_automation_actions.h>

#include <vclib/render/automation/actions/wrappers/frame_delay_automation_action.h>
#include <vclib/render/automation/actions/wrappers/frame_limited_automation_action.h>
#include <vclib/render/automation/actions/wrappers/start_count_delay_automation_action.h>
#include <vclib/render/automation/actions/wrappers/start_count_limited_automation_action.h>
#include <vclib/render/automation/actions/wrappers/time_delay_automation_action.h>
#include <vclib/render/automation/actions/wrappers/time_limited_automation_action.h>
#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

#endif // VCL_AUTOMATION_ACTIONS_H
