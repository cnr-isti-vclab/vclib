/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_ALGORITHMS_CLEAN_H
#define MGP_ALGORITHMS_CLEAN_H

namespace mgp {

template <typename MeshType>
unsigned int numberUnreferencedVertices(const MeshType& m);

template <typename MeshType>
unsigned int removeUnreferencedVertices(MeshType& m);

}

#include "clean.cpp"

#endif // MGP_ALGORITHMS_CLEAN_H
