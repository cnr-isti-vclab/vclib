/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#ifndef MGP_ALGORITHMS_CLEAN_H
#define MGP_ALGORITHMS_CLEAN_H

namespace mgp {

template <typename MeshType>
unsigned int numberUnreferencedVertices(const MeshType& m);

template <typename MeshType>
unsigned int removeUnreferencedVertices(MeshType& m);

template <typename MeshType>
unsigned int removeDuplicatedVertices(MeshType& m);

template <typename MeshType>
unsigned int removeDuplicatedFaces(MeshType& m);

template <typename MeshType>
unsigned int removeDegeneratedVertices(MeshType& m,  bool deleteAlsoFaces = true);

}

#include "clean.cpp"

#endif // MGP_ALGORITHMS_CLEAN_H
