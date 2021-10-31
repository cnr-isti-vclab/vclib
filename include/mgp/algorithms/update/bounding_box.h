/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_ALGORITHMS_BOUNDING_BOX_H
#define MGP_ALGORITHMS_BOUNDING_BOX_H

namespace mgp {

template<typename MeshType>
void updateBoundingBox(MeshType& m);

}

#include "bounding_box.cpp"

#endif // MGP_ALGORITHMS_BOUNDING_BOX_H
