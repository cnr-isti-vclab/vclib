// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MESHES_H
#define MESHES_H

#include "meshes/edgemesh.h"
#include "meshes/pointcloud.h"
#include "meshes/polymesh.h"
#include "meshes/trimesh.h"

void meshesStaticAsserts()
{
    // meshes
    edgemeshStaticAsserts();
    pointcloudStaticAsserts();
    polymeshStaticAsserts();
    trimeshStaticAsserts();
}

#endif // MESHES_H
