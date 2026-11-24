// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "elements/edge.h"
#include "elements/element.h"
#include "elements/face.h"
#include "elements/vertex.h"

void elementsStaticAsserts()
{
    elementStaticAsserts();

    edgeStaticAsserts();
    faceStaticAsserts();
    vertexStaticAsserts();
}

#endif // ELEMENTS_H
