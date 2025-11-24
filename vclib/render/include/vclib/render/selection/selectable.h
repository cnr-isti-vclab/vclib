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

#ifndef VCL_SELECTABLE_H
#define VCL_SELECTABLE_H

#include <vclib/render/selection/selection_box.h>
#include <vclib/render/selection/selection_mode.h>
#include <vclib/render/drawable/draw_object_settings.h>

namespace vcl {

struct SelectionParameters
{
    uint drawViewId;
    uint pass1ViewId;
    uint pass2ViewId;
    SelectionBox box;
    SelectionMode mode;
    bool isTemporary;
};

class Selectable
{
public:
    virtual void calculateSelection(const SelectionParameters& params) = 0;
};

} // namespace vcl
#endif
