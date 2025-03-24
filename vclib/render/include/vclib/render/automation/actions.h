#ifndef VCL_AUTOMATION_ACTIONS_H
#define VCL_AUTOMATION_ACTIONS_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/automation/actions/mesh_changer_automation_action.h>
#include <vclib/render/automation/actions/metric_changer_automation_action.h>
#include <vclib/render/automation/actions/per_frame_rotation_automation_action.h>
#include <vclib/render/automation/actions/per_frame_scale_automation_action.h>
#include <vclib/render/automation/actions/rotation_automation_action.h>
#include <vclib/render/automation/actions/scale_automation_action.h>
#include <vclib/render/automation/actions/sequential_automation_actions.h>
#include <vclib/render/automation/actions/simultaneous_automation_actions.h>

#include <vclib/render/automation/actions/wrappers/frame_delay_automation_action.h>
#include <vclib/render/automation/actions/wrappers/frame_limited_automation_action.h>
#include <vclib/render/automation/actions/wrappers/time_delay_automation_action.h>
#include <vclib/render/automation/actions/wrappers/time_limited_automation_action.h>
#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>
#include <vclib/render/automation/actions/wrappers/start_count_delay_automation_action.h>
#include <vclib/render/automation/actions/wrappers/start_count_limited_automation_action.h>

#endif