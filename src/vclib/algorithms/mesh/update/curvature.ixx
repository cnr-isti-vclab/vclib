/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

module;

#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include <mutex>
#include <vector>

export module vclib.algorithms.mesh.update.curvature;

import vclib.algorithms.core.bounding_box;
import vclib.algorithms.core.polygon;
import vclib.algorithms.core.stat;
import vclib.algorithms.mesh.intersection;
import vclib.algorithms.mesh.stat;
import vclib.algorithms.mesh.update.normal;
import vclib.concepts;
import vclib.math;
import vclib.mesh.requirements;
import vclib.mesh.utils.mesh_pos;
import vclib.misc;
import vclib.space.grid;
import vclib.space.matrix;
import vclib.space.sphere;
import vclib.views;

export {
#include <vclib/algorithms/mesh/update/curvature.h>
}
