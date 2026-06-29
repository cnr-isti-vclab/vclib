// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "drawers.h"

#include "canvas.h"
#include "directional_light.h"
#include "drawable_object.h"
#include "render_app.h"
#include "view_projection.h"
#include "window_manager.h"

int main()
{
    canvasStaticAsserts();
    directionalLightStaticAsserts();
    drawableObjectStaticAsserts();
    drawersStaticAsserts();
    renderAppStaticAsserts();
    viewProjectionStaticAsserts();
    windowManagerStaticAsserts();

    return 0;
}
