/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#ifndef MGP_ALGORITHMS_FLAG_H
#define MGP_ALGORITHMS_FLAG_H

namespace mgp {

template <typename MeshType>
void updateBorder(MeshType& m);

}

#include "flag.cpp"

#endif // MGP_ALGORITHMS_FLAG_H
