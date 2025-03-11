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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_CONVERT_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_CONVERT_MANAGER_H

#include "id_action_container.h"

#include <vclib/processing/engine/action_aggregators.h>

namespace vcl::proc::detail {

class ConvertManager
{
    IDActionContainer<ConvertActions> mConvertActions;

protected:
    void add(const std::shared_ptr<ConvertActions>& action)
    {
        mConvertActions.add(action);
    }

public:
    // convert

    std::shared_ptr<ConvertActions> convertActions(const std::string& name) const
    {
        return mConvertActions.action(name);
    }

    template<typename MeshType>
    std::shared_ptr<ConvertActionT<MeshType>> convertAction(
        const std::string& name)
    {
        std::shared_ptr<ConvertActions> actions = convertActions(name);

        return actions->action<MeshType>();
    }

    auto convertActions() { return mConvertActions.actions(); }
};

} // namespace vcl::proc::detail

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_CONVERT_MANAGER_H
