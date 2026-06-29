// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "components/adjacent_edges.h"
#include "components/adjacent_faces.h"
#include "components/adjacent_vertices.h"
#include "components/bit_flags.h"
#include "components/bounding_box.h"
#include "components/color.h"
#include "components/component.h"
#include "components/custom_components.h"
#include "components/mark.h"
#include "components/materials.h"
#include "components/name.h"
#include "components/normal.h"
#include "components/position.h"
#include "components/principal_curvature.h"
#include "components/quality.h"
#include "components/tex_coord.h"
#include "components/transform_matrix.h"
#include "components/vertex_references.h"
#include "components/wedge_colors.h"
#include "components/wedge_tex_coords.h"

void componentsStaticAsserts()
{
    componentStaticAsserts();

    adjacentEdgesComponentStaticAsserts();
    adjacentFacesComponentStaticAsserts();
    adjacentVerticesComponentStaticAsserts();
    bitFlagsComponentStaticAsserts();
    boundingBoxComponentStaticAsserts();
    colorComponentStaticAsserts();
    customComponentsComponentStaticAsserts();
    markComponentStaticAsserts();
    materialsComponentStaticAsserts();
    nameComponentStaticAsserts();
    normalComponentStaticAsserts();
    principalCurvatureComponentStaticAsserts();
    positionComponentStaticAsserts();
    qualityComponentStaticAsserts();
    texCoordComponentStaticAsserts();
    transformMatrixComponentStaticAsserts();
    vertexReferencesComponentStaticAsserts();
    wedgeColorsComponentStaticAsserts();
    wedgeTexCoordsComponentStaticAsserts();
}

#endif // COMPONENTS_H
